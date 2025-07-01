// Требуется отыскать самый короткий маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город.
// Требуемое время работы O((N + M) log N), где N – количество городов, M – известных дорог между ними.

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

int Dijkstra(int start, int end, const std::vector<std::vector<std::pair<int, int>>> &graph)
{
    int n = graph.size();
    std::vector<int> dist(n, INT_MAX);
    dist[start] = 0;

    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<>>
        pq;

    pq.push({0, start});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;

        for (auto &[v, w] : graph[u])
        {
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist[end] == INT_MAX ? -1 : dist[end];
}

int main()
{
    int countNodes, countEdges;
    std::cin >> countNodes >> countEdges;

    int u, v, w;

    std::vector<std::vector<std::pair<int, int>>> graph(countNodes);

    for (int i = 0; i < countEdges; i++)
    {
        std::cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    int start, end;
    std::cin >> start >> end;

    int shortest = Dijkstra(start, end, graph);
    std::cout << shortest;

    return 0;
}