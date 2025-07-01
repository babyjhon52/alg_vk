// Дан невзвешенный неориентированный граф.
// В графе может быть несколько кратчайших путей между какими - то вершинами.
// Найдите количество различных кратчайших путей между заданными вершинами.Требуемая сложность O(V + E).

#include <iostream>
#include <vector>
#include <queue>

class ListGraph
{
private:
    std::vector<int> *listVertices;
    int verticesCount;

public:
    ListGraph(int vertices) : verticesCount(vertices)
    {
        listVertices = new std::vector<int>[vertices];
    }

    ~ListGraph()
    {
        delete[] listVertices;
    }

    int getVerticesCounts()
    {
        return verticesCount;
    }

    void addEdge(int from, int to)
    {
        listVertices[from].push_back(to);
        listVertices[to].push_back(from);
    }

    std::vector<int> nextVertices(int from)
    {
        return listVertices[from];
    }

    int findMinPath(int from, int to) const
    {
        if (from == to)
            return 1;

        std::vector<int> distances(verticesCount, -1);
        std::vector<int> pathCounts(verticesCount, 0);
        std::queue<int> queue;

        distances[from] = 0;
        pathCounts[from] = 1;
        queue.push(from);

        while (!queue.empty())
        {
            int current = queue.front();
            queue.pop();

            for (int neighbor : listVertices[current])
            {
                if (distances[neighbor] == -1)
                {
                    distances[neighbor] = distances[current] + 1;
                    pathCounts[neighbor] = pathCounts[current];
                    queue.push(neighbor);
                }
                else if (distances[neighbor] == distances[current] + 1)
                {
                    pathCounts[neighbor] += pathCounts[current];
                }
            }
        }

        return pathCounts[to];
    }
};

int main()
{
    int vertices = 0, edges = 0, from = 0, to = 0;

    std::cin >> vertices >> edges;

    ListGraph myGraph(vertices);

    for (int i = 0; i < edges; i++)
    {
        std::cin >> from >> to;
        myGraph.addEdge(from, to);
    }

    std::cin >> from >> to;
    std::cout << myGraph.findMinPath(from, to);

    return 0;
}

// 4 5
//  0 1
//  0 2
//  1 2
//  1 3
//  2 3
// 0 3