// Дан неориентированный связный граф.Требуется найти вес минимального остовного дерева в этом графе.
// Алгоритм Крускала

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>

class DSU
{
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    DSU(int size) : parent(size), rank(size, 1)
    {
        for (int i = 0; i < size; i++)
            parent[i] = i;
    }

    int FindSet(int u)
    {
        if (parent[u] != u)
            parent[u] = FindSet(parent[u]);
        return parent[u];
    }

    void UnionSet(int u, int v)
    {
        u = FindSet(u);
        v = FindSet(v);

        if (u != v)
        {
            if (rank[u] < rank[v])
                std::swap(u, v);
            parent[v] = u;
            rank[u] += rank[v];
        }
    }
};

using edge = std::pair<int, std::pair<int, int>>;

std::vector<edge> Kruskal(int n, std::vector<edge> &edges)
{
    std::vector<edge> mst;
    DSU dsu(n);
    std::sort(edges.begin(), edges.end());

    for (const auto &e : edges)
    {
        int u = e.second.first;
        int v = e.second.second;
        if (dsu.FindSet(u) != dsu.FindSet(v))
        {
            dsu.UnionSet(u, v);
            mst.push_back(e);
        }
    }

    return mst;
}

// std::vector<std::vector<int>> buildGraph(int n, const std::vector<edge> &mst)
// {
//     std::vector<std::vector<int>> graph(n);
//     for (const auto &e : mst)
//     {
//         int u = e.second.first;
//         int v = e.second.second;
//         graph[u].push_back(v);
//         graph[v].push_back(u);
//     }
//     return graph;
// }

// void dfs(int u, std::vector<bool> &visited, const std::vector<std::vector<int>> &graph, std::vector<int> &tour)
// {
//     visited[u] = true;
//     tour.push_back(u);
//     for (int v : graph[u])
//     {
//         if (!visited[v])
//             dfs(v, visited, graph, tour);
//     }
// }

int main()
{
    int nodeCount, edgeCount;
    std::cin >> nodeCount >> edgeCount;

    std::vector<edge> edges;
    // std::map<std::pair<int, int>, int> dist;

    for (int i = 0; i < edgeCount; ++i)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        --u;
        --v;
        edges.push_back({w, {u, v}});
        // dist[{u, v}] = w;
        // dist[{v, u}] = w;
    }

    std::vector<edge> mst = Kruskal(nodeCount, edges);

    int totalWeight = 0;
    for (const auto &edge : mst)
    {
        int u = edge.second.first;
        int v = edge.second.second;
        int w = edge.first;
        totalWeight += w;
    }

    std::cout << totalWeight << std::endl;
    // auto graph = buildGraph(nodeCount, mst);

    // std::vector<bool> visited(nodeCount, false);
    // std::vector<int> tour;
    // dfs(0, visited, graph, tour);

    // tour.push_back(tour[0]);

    // int tspWeight = 0;
    // for (int i = 0; i + 1 < tour.size(); ++i)
    // {
    //     int u = tour[i];
    //     int v = tour[i + 1];
    //     tspWeight += dist[{u, v}];
    // }

    // std::cout << tspWeight;

    return 0;
}
