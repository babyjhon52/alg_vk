
// В одной военной части решили построить в одну шеренгу по росту.Т.к.часть была далеко не образцовая, то солдаты часто приходили не вовремя,
// а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие,
// а в конце – самые низкие.За расстановку солдат отвечал прапорщик, который заметил интересную особенность – в се солдаты в части разного роста.Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
// а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.Требуемая скорость выполнения команды - O(log n).

#include <iostream>
#include <algorithm>
#include <functional>

template <typename Compare = std::greater<int>>
class AvlTree
{
private:
    struct Soldier
    {
        int height;
        Soldier *left;
        Soldier *right;
        int count;
        size_t h;

        Soldier(int height) : height(height), left(nullptr), right(nullptr), count(1), h(1) {}
    };

    Soldier *root;
    Compare comp;

    void updateCount(Soldier *node)
    {
        if (!node)
            return;
        node->count = 1 + (node->left ? node->left->count : 0) + (node->right ? node->right->count : 0);
    }

    size_t getH(Soldier *node)
    {
        return node ? node->h : 0;
    }

    void fixH(Soldier *node)
    {
        node->h = std::max(getH(node->left), getH(node->right)) + 1;
    }

    int getBalance(Soldier *node)
    {
        return node ? int(getH(node->right)) - int(getH(node->left)) : 0;
    }

    Soldier *rotateRight(Soldier *node)
    {
        Soldier *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        fixH(node);
        fixH(tmp);
        updateCount(node);
        updateCount(tmp);
        return tmp;
    }

    Soldier *rotateLeft(Soldier *node)
    {
        Soldier *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        fixH(node);
        fixH(tmp);
        updateCount(node);
        updateCount(tmp);
        return tmp;
    }

    Soldier *balance(Soldier *node)
    {
        if (!node)
            return nullptr;

        fixH(node);
        updateCount(node);

        int bf = getBalance(node);
        if (bf == 2)
        {
            if (getBalance(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        if (bf == -2)
        {
            if (getBalance(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        return node;
    }

    Soldier *addRecursive(Soldier *node, int data)
    {
        if (!node)
            return new Soldier(data);

        if (comp(data, node->height))
            node->left = addRecursive(node->left, data);
        else
            node->right = addRecursive(node->right, data);

        node = balance(node);
        updateCount(node);
        return node;
    }

    Soldier *delRecursive(Soldier *node, int index)
    {
        if (!node)
            return nullptr;

        int leftCount = node->left ? node->left->count : 0;

        if (index < leftCount)
            node->left = delRecursive(node->left, index);

        else if (index > leftCount)
            node->right = delRecursive(node->right, index - leftCount - 1);

        else
        {
            if (!node->left || !node->right)
            {
                Soldier *tmp = node->left ? node->left : node->right;
                delete node;
                return tmp;
            }
            else
            {
                Soldier *maxNode = node->left;
                while (maxNode->right)
                    maxNode = maxNode->right;

                node->height = maxNode->height;

                int idx = (node->left ? node->left->count : 0) - 1;
                node->left = delRecursive(node->left, idx);
            }
        }

        node = balance(node);
        updateCount(node);
        return node;
    }

    int findStat(int data)
    {
        int k = 0;
        Soldier *cur = root;
        while (cur)
        {
            if (comp(cur->height, data))
            {
                k += 1 + (cur->left ? cur->left->count : 0);
                cur = cur->right;
            }
            else if (comp(data, cur->height))
                cur = cur->left;

            else
            {
                k += (cur->left ? cur->left->count : 0);
                break;
            }
        }

        return k;
    }

    void deleteTree(Soldier *node)
    {
        if (!node)
            return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    AvlTree() : root(nullptr) {}
    ~AvlTree()
    {
        deleteTree(root);
    }

    void add(int data)
    {
        std::cout << findStat(data) << "\n";
        root = addRecursive(root, data);
    }

    void del(int index)
    {
        root = delRecursive(root, index);
    }
};

int main()
{
    int n;
    std::cin >> n;

    AvlTree<> tree;
    for (int i = 0; i < n; i++)
    {
        int flag, x;
        std::cin >> flag >> x;

        if (flag == 1)
            tree.add(x);

        else if (flag == 2)
            tree.del(x);
    }
    return 0;
}
