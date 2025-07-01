#include "ListGraph.h"

ListGraph::ListGraph(int size) : adjacencyLists(size), verticesCounts(size) {}

ListGraph::ListGraph(const IGraph &graph) : verticesCounts(graph.VerticesCount())
{
    adjacencyLists.resize(verticesCounts);
    for (int i = 0; i < verticesCounts; ++i)
        adjacencyLists[i] = graph.GetNextVertices(i);
}

void ListGraph::AddEdge(int from, int to)
{
    adjacencyLists[from].push_back(to);
}

int ListGraph::VerticesCount() const
{
    return verticesCounts;
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const
{
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (int from = 0; from < verticesCounts; ++from)
        for (int to : adjacencyLists[from])
            if (to == vertex)
                result.push_back(from);
    return result;
}
