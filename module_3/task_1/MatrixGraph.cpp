#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int size)
    : matrixLists(size, std::vector<bool>(size, false)), verticesCounts(size) {}

MatrixGraph::MatrixGraph(const IGraph &graph)
    : verticesCounts(graph.VerticesCount())
{
    matrixLists.resize(verticesCounts, std::vector<bool>(verticesCounts, false));
    for (int from = 0; from < verticesCounts; ++from)
        for (int to : graph.GetNextVertices(from))
            matrixLists[from][to] = true;
}

void MatrixGraph::AddEdge(int from, int to)
{
    matrixLists[from][to] = true;
}

int MatrixGraph::VerticesCount() const
{
    return verticesCounts;
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < verticesCounts; ++i)
        if (matrixLists[vertex][i])
            result.push_back(i);
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < verticesCounts; ++i)
        if (matrixLists[i][vertex])
            result.push_back(i);
    return result;
}
