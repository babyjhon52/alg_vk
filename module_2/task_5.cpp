// В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных.
// Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8 - битный символ.

#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <cstdint>

using byte = unsigned char;

#include "Huffman.h"

struct Node
{
    byte symbol;
    size_t frequency;
    Node *left, *right;

    // Конструктор для листа
    Node(byte s, size_t f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}

    // Конструктор для узла
    Node(Node *l, Node *r) : symbol(0), frequency(l->frequency + r->frequency), left(l), right(r) {}
};

// Компоратор
struct NodeCmp
{
    bool operator()(Node *a, Node *b) const { return a->frequency > b->frequency; }
};

void SerializeTree(Node *n, std::vector<bool> &bits)
{
    if (!n->left && !n->right) // Если лист
    {
        bits.push_back(1); // Маркер листа
        for (int i = 7; i >= 0; --i)
            bits.push_back((n->symbol >> i) & 1); // Запись листа
    }
    else
    {
        bits.push_back(0); // Маркер узла, затем обходим его потомков

        SerializeTree(n->left, bits);

        SerializeTree(n->right, bits);
    }
}

Node *DeserializeTree(const std::vector<bool> &bits, size_t &pos)
{
    bool leaf = bits[pos++];
    if (leaf) // Если лист
    {
        byte sym = 0;
        for (int i = 0; i < 8; ++i)
            sym = (sym << 1) | bits[pos++]; // Восстанавливаем 8 бит
        return new Node(sym, 0);
    }

    // Обходим также потомков
    Node *l = DeserializeTree(bits, pos);

    Node *r = DeserializeTree(bits, pos);

    return new Node(l, r);
}

// Получаем коды
void BuildCodes(Node *n, std::vector<bool> &cur, std::array<std::vector<bool>, 256> &codes)
{
    if (!n->left && !n->right) // Если лист
    {
        codes[n->symbol] = cur; // Записываем код
        return;
    }

    cur.push_back(0); // Идем влево + 0
    BuildCodes(n->left, cur, codes);

    cur.back() = 1; // Идем вправо + 1
    BuildCodes(n->right, cur, codes);

    cur.pop_back();
}

// Очистка дерева
void FreeTree(Node *n)
{
    if (!n)
        return;

    FreeTree(n->left);

    FreeTree(n->right);

    delete n;
}

// Класс для записи бит
struct BitWriter
{
    IOutputStream &out; // Выход
    byte buf = 0;       // Буфер для накопления бит
    int cnt = 0;        // Счетчик бит для буфера

    BitWriter(IOutputStream &o) : out(o) {}

    void WriteBit(bool b)
    {
        buf = (buf << 1) | b; // Добавляем бит в буфер
        if (++cnt == 8)
            Flush(); // Если заполнен, то сбрасываем
    }

    void WriteBits(const std::vector<bool> &v)
    {
        for (bool b : v)
            WriteBit(b);
    }

    // Сброс буфера
    void Flush()
    {
        if (cnt)
        {
            buf <<= (8 - cnt);
            out.Write(buf);
            buf = 0;
            cnt = 0;
        }
    }
};

// Класс для чтения
struct BitReader
{
    IInputStream &in; // Вход
    byte buf = 0;
    int cnt = 0;
    BitReader(IInputStream &i) : in(i) {}

    bool ReadBit(bool &b)
    {
        if (cnt == 0) // Если буфер пуст
        {
            if (!in.Read(buf)) // Читаем новый буфер (байт)
                return false;
            cnt = 8;
        }

        b = (buf >> 7) & 1;
        buf <<= 1; // Двигаем буфер
        --cnt;

        return true;
    }

    // Пропускиоставшихся байт
    void Skip()
    {
        while (cnt--)
            buf <<= 1;
    }
};

struct ConsoleInputStream : IInputStream
{
    bool Read(byte &v) override
    {
        char ch;
        if (!std::cin.get(ch) || ch == '.')
            return false;

        v = static_cast<byte>(ch);

        return true;
    }
};

struct ConsoleOutputStream : IOutputStream
{
    void Write(byte v) override { std::cout << static_cast<char>(v); }
};

struct VectorInputStream : IInputStream
{
    const std::vector<byte> &buf;
    size_t pos = 0;
    VectorInputStream(const std::vector<byte> &b) : buf(b) {}

    bool Read(byte &v) override
    {
        if (pos >= buf.size())
            return false;
        v = buf[pos++];
        return true;
    }
};

struct VectorOutputStream : IOutputStream
{
    std::vector<byte> &buf;
    VectorOutputStream(std::vector<byte> &b) : buf(b) {}
    void Write(byte v) override { buf.push_back(v); }
};

// Кодирование
void Encode(IInputStream &in, IOutputStream &out)
{
    std::vector<byte> data;
    std::array<size_t, 256> freq{};
    byte x;

    while (in.Read(x))
    {
        data.push_back(x);
        freq[x]++;
    }
    if (data.empty())
        return;

    // Строим дерево Хаффмана
    std::priority_queue<Node *, std::vector<Node *>, NodeCmp> pq;
    for (int i = 0; i < 256; ++i)
        if (freq[i])
            pq.push(new Node((byte)i, freq[i]));

    if (pq.size() == 1) // Если 1 символ, то добавляем фиктивный лист
        pq.push(new Node((byte)0, 0));

    // Обьединяем узлы с меньшими частотами
    while (pq.size() > 1)
    {
        auto l = pq.top();
        pq.pop();
        auto r = pq.top();
        pq.pop();
        pq.push(new Node(l, r));
    }

    Node *root = pq.top();

    // Строим коды
    std::array<std::vector<bool>, 256> codes;
    std::vector<bool> cur;
    BuildCodes(root, cur, codes);

    std::vector<bool> treeBits;
    SerializeTree(root, treeBits);

    // Кодирование
    std::vector<bool> dataBits;
    for (auto c : data)
        dataBits.insert(dataBits.end(), codes[c].begin(), codes[c].end());

    // Вычисляем общий размер и необходимое дополнение
    size_t total = 1 + 16 + 3 + 32 + treeBits.size() + dataBits.size();
    byte pad = (8 - (total % 8)) % 8;

    size_t originalBits = data.size() * 8;
    if (total >= originalBits)
    {
        BitWriter writer(out);
        writer.WriteBit(0); // flag - данные не сжаты
        writer.Flush();

        for (byte b : data)
            out.Write(b);
    }
    else
    {
        BitWriter writer(out);
        writer.WriteBit(1); // flag - данные сжаты

        // Записываем дерево
        for (int i = 15; i >= 0; --i)
            writer.WriteBit((treeBits.size() >> i) & 1);

        // Записываем паддинг биты
        for (int i = 1; i >= 0; --i)
            writer.WriteBit((pad >> i) & 1);

        uint32_t sz = data.size();
        for (int i = 31; i >= 0; --i)
            writer.WriteBit((sz >> i) & 1);

        // Записываем дерево и данные
        writer.WriteBits(treeBits);
        writer.WriteBits(dataBits);

        for (int i = 0; i < pad; ++i)
            writer.WriteBit(0);

        writer.Flush();
    }

    FreeTree(root);
}

// Декодирование
void Decode(IInputStream &in, IOutputStream &out)
{
    BitReader reader(in);
    bool flag;

    if (!reader.ReadBit(flag)) // Если данные не сжаты, то просто переписываем их
        return;

    if (!flag)
    {
        byte v;
        while (in.Read(v))
            out.Write(v);
        return;
    }

    // Чтение данных
    size_t treeSize = 0;
    for (int i = 0; i < 16; ++i)
    {
        bool b;
        reader.ReadBit(b);
        treeSize = (treeSize << 1) | b;
    }

    byte pad = 0;
    for (int i = 0; i < 2; ++i)
    {
        bool b;
        reader.ReadBit(b);
        pad = (pad << 1) | b;
    }

    uint32_t orig = 0;
    for (int i = 0; i < 32; ++i)
    {
        bool b;
        reader.ReadBit(b);
        orig = (orig << 1) | b;
    }

    std::vector<bool> treeBits;
    bool bit;

    for (size_t i = 0; i < treeSize && reader.ReadBit(bit); ++i)
        treeBits.push_back(bit);

    size_t pos = 0;
    Node *root = DeserializeTree(treeBits, pos);

    Node *curr = root;
    uint32_t count = 0;
    while (count < orig && reader.ReadBit(bit))
    {
        curr = bit ? curr->right : curr->left;
        if (!curr->left && !curr->right) // Если лист, то выводим символ, и возвращаемся в корень
        {
            out.Write(curr->symbol);
            curr = root;
            ++count;
        }
    }

    reader.Skip();
    FreeTree(root); // Чистим за собой
}

// Затупил, добавил флаг для проверки сжат или нет, но не добавил саму проверку, также уменьшил pad на 1 бит