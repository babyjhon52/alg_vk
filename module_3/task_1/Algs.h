#pragma once
#include "IGraph.h"
#include <functional>
#include <vector>
#include <deque>

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func);
void mainBFS(const IGraph &graph, const std::function<void(int)> &func);
void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func);
void mainDFS(const IGraph &graph, const std::function<void(int)> &func);
std::deque<int> topologicalSort(const IGraph &graph);
