#include "Ghost.h"
#include <cmath>
#include <queue>
#include <unordered_set>
#include <algorithm>

Ghost::Ghost(int startNode, const std::string& texturePath,
    const std::unordered_map<int, std::vector<int>>& adjList,
    const std::vector<std::pair<int, int>>& pos)
{
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
    if (path.empty()) {
        if (allPaths.count(currentNode) && allPaths.at(currentNode).count(pacmanNode)) {
            path = allPaths[currentNode][pacmanNode];
            pathIndex = 0;
        }
    }

    if (pathIndex + 1 < path.size()) {
        int nextNode = path[pathIndex + 1];
        if (nextNode >= 0 && nextNode < (int)pos.size()) {
            Vector2f currentPos = sprite.getPosition();
            Vector2f targetPos(pos[nextNode].first, pos[nextNode].second);

            Vector2f dir = targetPos - currentPos;
            float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (distance < speed * deltaTime) {
                sprite.setPosition(targetPos);
                currentNode = nextNode;
                pathIndex++;

                if (allPaths.count(currentNode) && allPaths.at(currentNode).count(pacmanNode)) {
                    path = allPaths[currentNode][pacmanNode];
                    pathIndex = 0;
                }
            }
            else {
                dir /= distance;
                sprite.move(dir * speed * deltaTime);
            }
        }
    }
    else {
        // Path ended - recalc
        if (allPaths.count(currentNode) && allPaths.at(currentNode).count(pacmanNode)) {
            path = allPaths[currentNode][pacmanNode];
            pathIndex = 0;
        }
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