// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
// Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
// i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

#include <iostream>
#include <functional>
#include <algorithm>

template <typename T, typename Hasher = std::hash<T> >
class HashTable
{
private:
    int curSize;
    Hasher hasher;
    std::vector<std::string> table;

public:
    HashTable(size_t initalSize = 8) : curSize(0), table(initalSize, "NUL") {}

    ~HashTable() = default;

    bool has(std::string key)
    {
        size_t h = hasher(key) % table.size();

        for (int i = 0; i < table.size(); i++)
        {
            if (key == table[h])
                return true;
            else if (table[h] == "NUL")
                return false;
            h = (h + i) % table.size();
        }
        return false;
    }

    bool del(std::string key)
    {
        size_t h = hasher(key) % table.size();
        for (int i = 0; i < table.size(); i++)
        {
            if (key == table[h])
            {
                table[h] = "DEL";
                curSize--;
                return true;
            }
            else if (table[h] == "NUL")
                return false;
            h = (h + i) % table.size();
        }
        return false;
    }

    bool add(std::string key)
    {
        if (curSize >= 0.75 * table.size())
            resize();

        size_t h = hasher(key) % table.size();
        int idxDel = -1;

        for (int i = 0; i < table.size(); i++)
        {
            if (key == table[h])
                return false;
            else if (table[h] == "DEL" && idxDel == -1)
                idxDel = h;
            else if (table[h] == "NUL")
            {
                if (idxDel != -1)
                    table[idxDel] = key;
                else
                    table[h] = key;
                curSize++;

                return true;
            }
            h = (h + i) % table.size();
        }
        table[idxDel] = key;
        return true;
    }

    void resize()
    {
        std::vector<std::string> newTable(2 * table.size(), "NUL");
        size_t h;

        for (int i = 0; i < table.size(); i++)
        {
            if (table[i] != "DEL" && table[i] != "NUL")
            {
                h = hasher(table[i]) % newTable.size();

                for (int j = 0; j < newTable.size(); j++)
                {
                    if (newTable[h] == "NUL")
                    {
                        newTable[h] = table[i];
                        break;
                    }
                    h = (h + j) % newTable.size();
                }
            }
        }
        std::swap(table, newTable);

    }
};

class StringHasher
{
private:
    size_t prime;
public:
    StringHasher(size_t prime = 137) : prime(prime) {}

    size_t operator()(const std::string& str)
    {
        size_t hash = 0;
        for (char s : str)
        {
            hash = hash * prime + s;
        }
        return hash;
    }
};

int main()
{
    HashTable<std::string, StringHasher> table;

    char op;
    std::string key;

    while (std::cin >> op >> key)
    {
        switch (op)
        {
        case '?':
            std::cout << ((table.has(key)) ? "OK" : "FAIL") << std::endl;
            break;
        case '+':
            std::cout << ((table.add(key)) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            std::cout << ((table.del(key)) ? "OK" : "FAIL") << std::endl;
            break;
        }
    }
    return 0;
}