#pragma once
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <string>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <iostream>

class Graph {
public:
    Graph();
    void addEdge(int v1, int v2);
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename, const std::vector<std::pair<int, int>>& spanningTree);
    std::vector<std::pair<int, int>> findSpanningTree();
    const std::map<int, std::vector<int>>& getAdjacencyList() const;
    std::map<int, std::vector<int>>& getAdjacencyList();

private:
    std::map<int, std::vector<int>> adjacencyList;
    void log(const std::string& message, const std::string& level);
};