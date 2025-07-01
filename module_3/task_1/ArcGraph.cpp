#include "ArcGraph.h"

ArcGraph::ArcGraph(int size) : verticesCounts(size) {}

ArcGraph::ArcGraph(const IGraph &graph) : verticesCounts(graph.VerticesCount())
{
    for (int from = 0; from < verticesCounts; ++from)
        for (int to : graph.GetNextVertices(from))
            pairLists.emplace_back(from, to);
}

void ArcGraph::AddEdge(int from, int to)
{
    pairLists.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const
{
    return verticesCounts;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    std::vector<int> result;
    for (auto &p : pairLists)
        if (p.first == vertex)
            result.push_back(p.second);
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (auto &p : pairLists)
        if (p.second == vertex)
            result.push_back(p.first);
    return result;
}
