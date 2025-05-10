#include "Map.h"

void Map::addEdge(int u, int v) {
    adjList[u].push_back(v);
    adjList[v].push_back(u);
}

void Map::printAdjList() {
    for (const auto& [node, neighbors] : adjList) {
        cout << node << ": ";
        for (int neighbor : neighbors) {
            cout << neighbor << " ";
        }
        cout << "\n";
    }
}

void Map::init() {
    pos = vector<pair<int, int>>(70);
    for (auto [u, v] : edges) {
        addEdge(u, v);
    }
}

pair<int, int> Map::getPos(int u) {
    return pos[u];
}
