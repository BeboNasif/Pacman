#include "Map.h"
#include "include.h"
using namespace sf;
void Map::addEdge(int u, int v, int map_number) {
    adjList[map_number][u].push_back(v);
    adjList[map_number][v].push_back(u);
}

void Map::printAdjList(int map_number) {
    for (int node = 1; node <= 90;node++) {
        cout << node << ": ";
        for (int neighbor : adjList[map_number][node]) {
            cout << neighbor << " ";
        }
        cout << "\n";
    }
}

void Map::init() {
    for(int i = 0; i < 2;i++) 
        for (auto [u, v] : edges[i]) {
            addEdge(u, v, i);
        }
}

pair<int, int> Map::getPos(int &u, int map_number) {
    return pos[map_number][u];
}
