#include "Algs.h"
#include <queue>

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    std::queue<int> q;
    visited[vertex] = true;
    q.push(vertex);

    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        func(v);

        for (int next : graph.GetNextVertices(v))
        {
            if (!visited[next])
            {
                visited[next] = true;
                q.push(next);
            }
        }
    }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i)
        if (!visited[i])
            BFS(graph, i, visited, func);
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);
    for (int next : graph.GetNextVertices(vertex))
        if (!visited[next])
            DFS(graph, next, visited, func);
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i)
        if (!visited[i])
            DFS(graph, i, visited, func);
}

static void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;
    for (int next : graph.GetNextVertices(vertex))
        if (!visited[next])
            topologicalSortInternal(graph, next, visited, sorted);
    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::deque<int> sorted;
    for (int i = 0; i < graph.VerticesCount(); ++i)
        if (!visited[i])
            topologicalSortInternal(graph, i, visited, sorted);
    return sorted;
}
