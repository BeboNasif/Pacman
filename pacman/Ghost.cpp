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
    sprite.setOrigin(8, 8);
    sprite.setScale(3.5f, 3.5);
    currentNode = startNode;
    pathIndex = 0;
    elapsedTime = 0;
    speed = 150.f;

    //texture.setSmooth(false);

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 16, 16)); // initial frame

    animationFrame = 0;
    animationTimer = 0.f;
    animationSpeed = 0.1f;
    lastDir = { 1.f, 0.f }; // default direction (right)


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
    }
    else {
        dir /= distance;
        lastDir = dir; // track direction
        sprite.move(dir * speed * deltaTime);
    }
    updateAnimation(); // 👈 Add this here

}

void Ghost::updateAnimation() 
{
    animationTimer += animationSpeed;
    if (animationTimer >= 1.f) {
        animationTimer = 0.f;
        animationFrame = (animationFrame + 1) % 2;
    }

    int row = 0;
    if (std::abs(lastDir.x) > std::abs(lastDir.y)) {
        row = lastDir.x > 0 ? 0 : 1;
    }
    else {
        row = lastDir.y < 0 ? 2 : 3;
    }

    int frameIndex = row * 2 + animationFrame;
    int frameX = (frameIndex % 8) * 16;

    sprite.setTextureRect(sf::IntRect(frameX, 0, 16, 16));
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

int Ghost::Amoor(int pacmanNode) {
    vector<int> corners = { 1,10,81,90 };
    if (allPaths[currentNode][pacmanNode].size() > 6)
        return pacmanNode;
    else {
        int mn = 100;
        int target = pacmanNode;
        for (auto x : corners) 
            if (allPaths[currentNode][x].size() < mn and currentNode != x)
                mn = allPaths[currentNode][x].size(), target = x;
        
        return target;
    }
}

int Ghost::Ad3k(int pacmanNode) {
    return pacmanNode;
}

// up : 0, down : 1, right : 2, left : 3

int dfs(int node, int start,int limit,int steps, unordered_map<int, std::vector<int>>& adjList) {
    if (steps == limit) return node;
    for (auto child : adjList[node]) {
        if (child == start) continue;
        return dfs(child, start, limit, steps + 1, adjList);
        break;
    }
}
int getNext(int pacmanNode, int dir) {
    int node = pacmanNode;
    if (dir == 0) node = pacmanNode - 10;
    if (dir == 1) node = pacmanNode + 10;
    if (dir == 2) node = pacmanNode + 1;
    if (dir == 3) node = pacmanNode - 1;
    return node;
}

int Ghost::EL7okooma(int pacmanNode,int dir, unordered_map<int, std::vector<int>>& adjList) {
    // node -> the node pacman is directed to 
    int node = getNext(pacmanNode, dir);
    int limit = 3;
    return dfs(node, pacmanNode, limit, 0,adjList);

}
int Ghost::ELSaad(int pacmanNode,int dir, int Ad3kNode, unordered_map<int, std::vector<int>>& adjList)
{
    int node = getNext(pacmanNode, dir);
    int dist = allPaths[Ad3kNode][pacmanNode].size();
    int limit = dist + 2;
    return dfs(node, pacmanNode, limit, 0, adjList);
    
}
