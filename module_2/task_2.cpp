// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево поиска, заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Выведите элементы в порядке level-order (по слоям, “в ширину”).

#include <iostream>
#include <queue>
#include <stack>
#include <functional>

template <typename T, typename Comparator = std::less<T>>
class Tree
{
    struct Node
    {
        T data;
        Node *left;
        Node *right;

        Node(T data) : data(data), left(nullptr), right(nullptr) {}
    };

private:
    Node *root;
    Comparator cmp;
    void visit(Node *node)
    {
        std::cout << node->data << " ";
    }

public:
    Tree() : root(nullptr), cmp(Comparator()) {}

    ~Tree()
    {
        std::stack<Node *> nodeStack;
        std::stack<Node *> deleteStack;
        nodeStack.push(root);

        while (!nodeStack.empty())
        {
            Node *tmp = nodeStack.top();
            nodeStack.pop();
            deleteStack.push(tmp);

            if (tmp->right)
                nodeStack.push(tmp->right);
            if (tmp->left)
                nodeStack.push(tmp->left);
        }

        while (!deleteStack.empty())
        {
            Node *node = deleteStack.top();
            deleteStack.pop();
            delete node;
        }
    }

    void add(T data)
    {
        if (root == nullptr)
        {
            root = new Node(data);
            return;
        }

        Node *tmp = root;
        while (tmp)
        {
            if (cmp(data, tmp->data))
            {
                if (tmp->left == nullptr)
                {
                    tmp->left = new Node(data);
                    return;
                }
                tmp = tmp->left;
            }
            else
            {
                if (tmp->right == nullptr)
                {
                    tmp->right = new Node(data);
                    return;
                }
                tmp = tmp->right;
            }
        }
    }

    void level_order()
    {
        Node *tmp = root;
        std::queue<Node *> queue;
        queue.push(tmp);

        while (!queue.empty())
        {
            Node *node = queue.front();
            queue.pop();
            visit(node);

            if (node->left)
                queue.push(node->left);
            if (node->right)
                queue.push(node->right);
        }
    }
};

int main()
{
    int n = 0, x = 0;
    std::cin >> n;
    Tree<int> tree;
    for (int i = 0; i < n; i++)
    {
        std::cin >> x;
        tree.add(x);
    }
    tree.level_order();
    return 0;
}