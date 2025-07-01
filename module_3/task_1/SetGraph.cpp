#include "SetGraph.h"

SetGraph::SetGraph(int size) : verticesCounts(size) {}

SetGraph::SetGraph(const IGraph &graph) : verticesCounts(graph.VerticesCount())
{
    for (int from = 0; from < verticesCounts; ++from)
        for (int to : graph.GetNextVertices(from))
            setLists.emplace(from, to);
}

void SetGraph::AddEdge(int from, int to)
{
    setLists.emplace(from, to);
}

int SetGraph::VerticesCount() const
{
    return verticesCounts;
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const
{
    std::vector<int> result;
    for (const auto &edge : setLists)
        if (edge.first == vertex)
            result.push_back(edge.second);
    return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (const auto &edge : setLists)
        if (edge.second == vertex)
            result.push_back(edge.first);
    return result;
}
