#pragma once
#include "IGraph.h"
#include <vector>

class MatrixGraph : public IGraph
{
public:
    MatrixGraph(int size);
    MatrixGraph(const IGraph &graph);

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<bool>> matrixLists;
    int verticesCounts;
};
