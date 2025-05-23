#include "pch.h"
#include "Graph.h"
#include <sstream>

Graph::Graph() {
    log("Graph instance created", "INFO");
}

void Graph::log(const std::string& message, const std::string& level) {
    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open()) {
        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now);
        dt[strlen(dt) - 1] = '\0';
        logFile << "[" << dt << "] [" << level << "] " << message << std::endl;
        logFile.close();
    }
    else {
        std::cerr << "Failed to write to log file" << std::endl;
    }
}

void Graph::addEdge(int v1, int v2) {
    adjacencyList[v1].push_back(v2);
    adjacencyList[v2].push_back(v1);
    log("Edge added: " + std::to_string(v1) + " - " + std::to_string(v2), "INFO");
}

void Graph::loadFromFile(const std::string& filename) {
    adjacencyList.clear();
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("�� ������� ������� ����: " + filename);

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        int vertex;
        if (!(iss >> vertex))
            throw std::runtime_error("�������� ������ ������� � ������: " + line);

        std::vector<int> neighbors;
        int adjVertex;
        while (iss >> adjVertex) {
            if (adjVertex == vertex) {
                log("���������� ����� � ������� " + std::to_string(vertex), "WARNING");
                continue;
            }
            if (std::find(neighbors.begin(), neighbors.end(), adjVertex) != neighbors.end()) {
                log("��������� ����� " + std::to_string(vertex) + "-" + std::to_string(adjVertex), "WARNING");
                continue;
            }
            neighbors.push_back(adjVertex);
        }
        adjacencyList[vertex] = neighbors;
    }

    if (adjacencyList.empty())
        throw std::runtime_error("���� �� �������� ������ � �����");
}

void Graph::saveToFile(const std::string& filename, const std::vector<std::pair<int, int>>& spanningTree) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        log("Failed to save to file: " + filename, "ERROR");
        throw std::runtime_error("Failed to save to file");
    }

    for (const auto& edge : spanningTree) {
        file << edge.first << " " << edge.second << "\n";
    }
    log("Spanning tree saved to file: " + filename, "INFO");
}

std::vector<std::pair<int, int>> Graph::findSpanningTree() {
    std::vector<std::pair<int, int>> spanningTree;
    std::set<int> visited;
    std::queue<int> q;
    int componentCount = 0;

    if (adjacencyList.empty()) {
        log("Empty graph, spanning tree is not possible", "WARNING");
        return spanningTree;
    }

    for (const auto& [vertex, _] : adjacencyList) {
        if (visited.find(vertex) == visited.end()) {
            ++componentCount;
            q.push(vertex);
            visited.insert(vertex);

            while (!q.empty()) {
                int current = q.front();
                q.pop();

                for (int neighbor : adjacencyList[current]) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        spanningTree.push_back({ current, neighbor });
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    log("Spanning tree found with " + std::to_string(componentCount) + " components", "INFO");
    return spanningTree;
}

const std::map<int, std::vector<int>>& Graph::getAdjacencyList() const {
    return adjacencyList;
}

std::map<int, std::vector<int>>& Graph::getAdjacencyList() {
    return adjacencyList;
}