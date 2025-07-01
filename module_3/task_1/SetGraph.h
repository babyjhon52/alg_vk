#pragma once
#include "IGraph.h"
#include <unordered_set>
#include <utility>
#include <vector>

struct PairHash
{
    size_t operator()(const std::pair<int, int> &p) const
    {
        return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
    }
};

struct PairEqual
{
    bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b) const
    {
        return a.first == b.first && a.second == b.second;
    }
};

class SetGraph : public IGraph
{
public:
    SetGraph(int size);
    SetGraph(const IGraph &graph);

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::unordered_set<std::pair<int, int>, PairHash, PairEqual> setLists;
    int verticesCounts;
};
