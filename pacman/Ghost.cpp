#include "Ghost.h"
#include <cmath>
#include <queue>
#include <unordered_set>
#include <algorithm>

Ghost::Ghost(int startNode, const std::string& texturePath,
    unordered_map<int, std::vector<int>> adjList,
    const std::vector<std::pair<int, int>>& pos)
{
    adjList[91].clear();
    adjList[92].clear();
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture);
    sprite.setOrigin(11, 11);
    sprite.setScale(2.7f, 2.7f);
    currentNode = startNode;
    pathIndex = 0;
    elapsedTime = 0;
    speed = 100.0f;

    sprite.setPosition(pos[startNode].first, pos[startNode].second);

    allPaths = precomputeAllPaths(adjList);
}

void Ghost::update(float deltaTime,
    const std::unordered_map<int, std::vector<int>>& adjList,
    const std::vector<std::pair<int, int>>& pos,
    int pacmanNode)
{
    if (path.empty() || pathIndex + 1 >= path.size()) {
        // Recalculate path if possible
        if (allPaths.count(currentNode) && allPaths.at(currentNode).count(pacmanNode)) {
            path = allPaths[currentNode][pacmanNode];
            pathIndex = 0;
        }
        return; // Don't try to move if path is invalid
    }

    Vector2f currentPos = sprite.getPosition();
    Vector2f targetPos(pos[path[pathIndex + 1]].first, pos[path[pathIndex + 1]].second);

    Vector2f dir = targetPos - currentPos;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distance < speed * deltaTime) {
        sprite.setPosition(targetPos);
        currentNode = path[pathIndex + 1];
        pathIndex++;

        if (allPaths.count(currentNode) && allPaths.at(currentNode).count(pacmanNode)) {
            path = allPaths[currentNode][pacmanNode];
            pathIndex = 0;
        }
    }
    else {
        dir /= distance;
        float invert = 1;
        if ((currentNode == 23 and pacmanNode == 30) or (pacmanNode == 30 and currentNode == 23))
            invert = -1;
        sprite.move(invert * dir * speed * deltaTime);
    }
}



void Ghost::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

int Ghost::getCurrentNode() const {
    return currentNode;
}

std::unordered_map<int, std::unordered_map<int, std::vector<int>>>
Ghost::precomputeAllPaths(const std::unordered_map<int, std::vector<int>>& adjList)
{
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> allPaths;

    for (const auto& [start, _] : adjList) {
        std::queue<int> q;
        std::unordered_map<int, int> parent;
        std::unordered_set<int> visited;

        q.push(start);
        visited.insert(start);
        parent[start] = -1;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbor : adjList.at(current)) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        for (const auto& [end, _] : parent) {
            std::vector<int> path;
            int cur = end;
            while (cur != -1) {
                path.push_back(cur);
                cur = parent[cur];
            }
            std::reverse(path.begin(), path.end());
            allPaths[start][end] = path;
        }
    }

    return allPaths;
}
const sf::Sprite& Ghost::getSprite() const {
    return sprite;
}

void Ghost::reset(int startNode, const std::vector<std::pair<int, int>>& pos) {
    path.clear();
    currentNode = startNode;
    pathIndex = 0;
    sprite.setPosition(pos[startNode].first, pos[startNode].second);
}