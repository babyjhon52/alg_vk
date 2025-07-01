#include <iostream>
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"
#include "SetGraph.h"
#include "Algs.h"

int main()
{
    ListGraph listGraph(7);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);

    auto runAlgorithms = [](const IGraph &graph, const std::string &name)
    {
        std::cout << "BFS " << name << ": ";
        mainBFS(graph, [](int v)
                { std::cout << v << " "; });
        std::cout << "\nDFS " << name << ": ";
        mainDFS(graph, [](int v)
                { std::cout << v << " "; });
        std::cout << "\nTopologSort " << name << ": ";
        for (int v : topologicalSort(graph))
            std::cout << v << " ";
        std::cout << "\n\n";
    };

    runAlgorithms(listGraph, "ListGraph");
    MatrixGraph matrixGraph(listGraph);
    runAlgorithms(matrixGraph, "MatrixGraph");
    ArcGraph arcGraph(matrixGraph);
    runAlgorithms(arcGraph, "ArcGraph");
    SetGraph setGraph(arcGraph);
    runAlgorithms(setGraph, "SetGraph");

    return 0;
}