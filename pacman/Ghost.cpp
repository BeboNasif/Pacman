#include <cmath>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <algorithm>
#include "include.h"
#include "Ghost.h"
#include "Sounds.h"
#include "Player.h"
unordered_map<int, unordered_map<int, vector<int>>> Ghost::allPaths;
float Ghost::timer = 0;
Ghost::Ghost(int startNode, string texturePath, unordered_map<int, vector<int>> adjList, vector<pair<int, int>> pos) {
    timer = 0;
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture);
    sprite.setOrigin(8, 8);
    sprite.setScale(3.5f, 3.5f);
    currentNode = startNode;
    pathIndex = 0;
    elapsedTime = 0;
    speed = 150.f;

    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, 16, 16)); // initial frame

    animationFrame = 0;
    animationTimer = 0.f;
    animationSpeed = 0.1f;
    lastDir = { 1.f, 0.f }; // default direction (right)

    sprite.setPosition(pos[startNode].first, pos[startNode].second);

}

void Ghost::update(float deltaTime, vector<pair<int, int>>& pos, int pacmanNode)
{
    if (waitingAfterReturn) {
        if (returnClock.getElapsedTime().asSeconds() >= 3.f) {
            waitingAfterReturn = false; // Done waiting, continue as normal
        }
        else {
            return; // Still waiting at base
        }
    }

    Vector2f currentPos = sprite.getPosition();
    Vector2f nodePos(pos[currentNode].first, pos[currentNode].second);

    // Check if ghost is exactly on a node (within small epsilon margin)
    float epsilon = 0.1f;
    if (abs(currentPos.x - nodePos.x) < epsilon && abs(currentPos.y - nodePos.y) < epsilon) {
        sprite.setPosition(nodePos); // Snap exactly to node
        // Only update path when standing on a node
        if (allPaths.count(currentNode) && allPaths[currentNode].count(pacmanNode)) {
            path = allPaths[currentNode][pacmanNode];
            pathIndex = 0;
        }
    }

    // Prevent crash if no further movement in path
    if (pathIndex + 1 >= path.size())
        return;

    Vector2f targetPos(pos[path[pathIndex + 1]].first, pos[path[pathIndex + 1]].second);

    Vector2f dir = targetPos - currentPos;
    float distance = sqrt(dir.x * dir.x + dir.y * dir.y);



    if (isDead) {
        int frameIndex = 0;

        if (abs(lastDir.x) > abs(lastDir.y)) {
            frameIndex = (lastDir.x > 0) ? 0 : 1; // right : left
        }
        else {
            frameIndex = (lastDir.y < 0) ? 2 : 3; // up : down
        }

        int frameX = frameIndex * 16;
        deadSprite.setTextureRect(IntRect(frameX, 0, 16, 16));

        deadSprite.setPosition(sprite.getPosition());
        speed = 250;

    }

    if (isPoisoned) {
        float elapsed = poisonedClock.getElapsedTime().asSeconds();

        if (elapsed > poisonedDuration) {
            isPoisoned = false;
            return; // go back to normal update logic
        }

        int frame = 0;
        if (elapsed < 5.f) {
            frame = static_cast<int>((elapsed * 4)) % 2; // alternate between frame 0 and 1
        }
        else {
            frame = static_cast<int>((elapsed - 5.f) * 4) % 4; // frames 2, 3, 4
        }

        poisonedSprite.setTextureRect(sf::IntRect(frame * 16, 0, 16, 16));
        poisonedSprite.setPosition(sprite.getPosition());
    }



    if (distance < speed * deltaTime) {
        sprite.setPosition(targetPos);
        currentNode = path[pathIndex + 1];
        pathIndex++;
        if (isDead and (currentNode == pacmanNode)) {
            ghostOut = 0;
            isDead = false;
            isPoisoned = false;
            sprite.setTexture(texture);
            sprite.setScale(3.5f, 3.5f);

            waitingAfterReturn = true;
            returnClock.restart();
            return;
        }
    }
    else {
        dir /= distance;
        lastDir = dir;
        sprite.move(dir * speed * deltaTime);
    }


    updateAnimation();
}

void Ghost::updateAnimation()
{
    speed = 150.f;
    animationTimer += animationSpeed;
    if (animationTimer >= 1.f) {
        animationTimer = 0.f;
        animationFrame = (animationFrame + 1) % 2;
    }

    int row = 0;
    if (abs(lastDir.x) > abs(lastDir.y)) {
        row = lastDir.x > 0 ? 0 : 1;
    }
    else {
        row = lastDir.y < 0 ? 2 : 3;
    }

    int frameIndex = row * 2 + animationFrame;
    int frameX = (frameIndex % 8) * 16;

    sprite.setTextureRect(IntRect(frameX, 0, 16, 16));
}

void Ghost::draw(RenderWindow& window) {
    if (isDead) {
        window.draw(deadSprite);
    }
    else if (isPoisoned) {
        window.draw(poisonedSprite);
    }
    else {
        window.draw(sprite);
    }
}

int Ghost::getCurrentNode() {
    return currentNode;
}

unordered_map<int, unordered_map<int, vector<int>>> Ghost::precomputeAllPaths(unordered_map<int, vector<int>> adjList)
{
    adjList[91].clear();
    adjList[92].clear();
    unordered_map<int, unordered_map<int, vector<int>>> allPaths;

    for (auto& start_pair : adjList) {
        int start = start_pair.first;

        queue<int> q;
        unordered_map<int, int> parent;
        unordered_set<int> visited;

        q.push(start);
        visited.insert(start);
        parent[start] = -1;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbor : adjList[current]) {
                if (!visited.count(neighbor)) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        for (auto& end_pair : parent) {
            int end = end_pair.first;
            vector<int> path;
            int cur = end;
            while (cur != -1) {
                path.push_back(cur);
                cur = parent[cur];
            }
            reverse(path.begin(), path.end());
            allPaths[start][end] = path;
        }
    }

    return allPaths;
}

Sprite& Ghost::getSprite() {
    return sprite;
}

void Ghost::reset(int startNode, vector<pair<int, int>>& pos) {
    path.clear();
    currentNode = startNode;
    isPoisoned = 0;
    pathIndex = 0;
    sprite.setPosition(pos[startNode].first, pos[startNode].second);
    ghostOut = 0;
    isDead = 0;
    timer = 0;
}

int Ghost::Amoor(int pacmanNode) {
    vector<int> corners = { 1,10,81,90 };
    if (allPaths[currentNode][pacmanNode].size() > 6)
        return pacmanNode;
    else {
        int mn = 100;
        int target = pacmanNode;
        int mnToPacNode = 100;
        int pacmanclosestnode = pacmanNode;
        for (auto x : corners) {
            if (allPaths[pacmanNode][x].size() < mnToPacNode) {
                mnToPacNode = allPaths[pacmanNode][x].size(), pacmanclosestnode = x;
            }
        }
        int pacmanclosest2 = pacmanNode;
        mnToPacNode = 100;

        for (auto x : corners) {
            if (allPaths[pacmanNode][x].size() < mnToPacNode and x != pacmanclosestnode) {
                mnToPacNode = allPaths[pacmanNode][x].size(), pacmanclosest2 = x;
            }
        }

        for (auto x : corners)
            if (allPaths[currentNode][x].size() < mn and x != pacmanclosest2 and x != currentNode and x != pacmanclosestnode)
                mn = allPaths[currentNode][x].size(), target = x;

        return target;
    }
}

int Ghost::Ad3k(int pacmanNode) {
    return pacmanNode;
}

// up : 0, down : 1, right : 2, left : 3

int dfs(int node, int start, int limit, int steps, unordered_map<int, std::vector<int>>& adjList) {
    if (steps == limit) return node;
    for (auto child : adjList[node]) {
        if (child == start) continue;
        return dfs(child, start, limit, steps + 1, adjList);
    }
    return 0;
}
int getNext(int pacmanNode, int dir) {
    int node = pacmanNode;
    if (dir == 0) node = pacmanNode - 10;
    if (dir == 1) node = pacmanNode + 10;
    if (dir == 2) node = pacmanNode + 1;
    if (dir == 3) node = pacmanNode - 1;
    return node;
}

int Ghost::EL7okooma(int pacmanNode, int dir, unordered_map<int, std::vector<int>>& adjList) {
    // node -> the node pacman is directed to 
    int node = getNext(pacmanNode, dir);
    int limit = 3;
    return dfs(node, pacmanNode, limit, 0, adjList);

}
int Ghost::ELSaad(int pacmanNode, int dir, int Ad3kNode, unordered_map<int, std::vector<int>>& adjList)
{
    int node = getNext(pacmanNode, dir);
    int dist = allPaths[Ad3kNode][pacmanNode].size();
    int limit = dist + 2;
    return dfs(node, pacmanNode, limit, 0, adjList);

}

void Ghost::poisoned(const string& poisonedTexturePath) {
    if (waitingAfterReturn) return;
    poisonedTexture.loadFromFile(poisonedTexturePath);
    poisonedSprite.setTexture(poisonedTexture);
    poisonedSprite.setTextureRect(IntRect(0, 0, 16, 16)); // start with first frame
    poisonedSprite.setScale(3.5f, 3.5f);
    poisonedSprite.setOrigin(8, 8);
    poisonedSprite.setPosition(sprite.getPosition());
    isPoisoned = true;
    poisonedClock.restart();
}

bool Ghost::shouldUpdate(int i) {
    return (isPoisoned || ghostOut || (i == 0) || (i == 1 && timer > 5) || (i == 2 && timer > 10) || (i == 3 && timer > 15));
}

void Ghost::die(const string& deadTexturePath, int& score) {
    isDead = 1;
    score += 50;
    deadTexture.loadFromFile(deadTexturePath);
    deadSprite.setTexture(deadTexture);
    deadSprite.setTextureRect(IntRect(0, 0, 16, 16));
    deadSprite.setScale(3.5f, 3.5f);
    deadSprite.setOrigin(8, 8);
    deadSprite.setPosition(sprite.getPosition());

}