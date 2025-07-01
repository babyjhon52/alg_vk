// Постройте B-дерево минимального порядка t и выведите его по слоям.
// В качестве ключа используются числа, лежащие в диапазоне [0..232-1]

#include <iostream>
#include <queue>
#include <vector>
#include <functional>

template <typename T, typename Comparator = std::less<T> >
class BTree 
{
    struct Node 
    {
        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;

        Node(bool leaf) : leaf(leaf) {}
        ~Node() 
        {
            for (Node* node : children)
                delete node;
        }
    };

private:
    Node* root;
    Comparator cmp;
    int t;

public:
    BTree(int t) : root(nullptr), cmp(Comparator()), t(t) {}

    ~BTree() 
    {
        if (root)
            delete root;
    }

    void insert(T key) 
    {
        if (root == nullptr) 
        {
            root = new Node(true);
            root->keys.push_back(key);
            return;
        }

        if (root->keys.size() == 2 * t - 1) 
        {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }

        insertNonFull(root, key);
    }

    void insertNonFull(Node* node, T key) 
    {
        int i = node->keys.size() - 1;

        if (node->leaf) 
        {
            node->keys.resize(node->keys.size() + 1);
            while (i >= 0 && cmp(key, node->keys[i])) 
            {
                node->keys[i + 1] = node->keys[i];
                i--;
            }

            node->keys[i + 1] = key;
        }
        else 
        {
            while (i >= 0 && cmp(key, node->keys[i]))
                i--;
            i++;

            if (node->children[i]->keys.size() == 2 * t - 1) 
            {
                splitChild(node, i);
                if (cmp(node->keys[i], key))
                    i++;
            }

            insertNonFull(node->children[i], key);
        }
    }

    void splitChild(Node* parent, int index) 
    {
        Node* child = parent->children[index];
        Node* newNode = new Node(child->leaf);

        newNode->keys.assign(child->keys.begin() + t, child->keys.end());

        if (!child->leaf) 
        {
            newNode->children.assign(child->children.begin() + t, child->children.end());
            child->children.resize(t);
        }

        T middleKey = child->keys[t - 1];
        child->keys.resize(t - 1);

        parent->keys.insert(parent->keys.begin() + index, middleKey);
        parent->children.insert(parent->children.begin() + index + 1, newNode);
    }

    void printLevelOrder() 
    {
        if (!root) return;

        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) 
        {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; i++) 
            {
                Node* current = q.front();
                q.pop();
                for (int j = 0; j < current->keys.size(); j++) 
                {
                    std::cout << current->keys[j];
                    if (j != current->keys.size() - 1)
                        std::cout << " ";
                }
                if (!current->leaf) 
                {
                    for (Node* child : current->children)
                        q.push(child);
                }

                if (i != levelSize - 1)
                    std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    int t, n;
    std::cin >> t;

    BTree<int> tree(t);

    while (std::cin >> n)
        tree.insert(n);

    tree.printLevelOrder();
    return 0;
}