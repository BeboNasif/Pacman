#include <cmath>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <algorithm>
#include <array>
#include "include.h"
#include "Ghost.h"
#include "Sounds.h"
#include "Player.h"


unordered_map<int, unordered_map<int, vector<int>>> Ghost::allPaths;
float Ghost::timer = 0;

Ghost::Ghost(int startNode, string texturePath, unordered_map<int, vector<int>> adjList, vector<pair<int, int>> pos) {
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture);
    sprite.setOrigin(8, 8);
    sprite.setScale(3.5f, 3.5f);
    currentNode = startNode;
    pathIndex = 0;
    elapsedTime = 0;
    speed = 150.f;

    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, 16, 16));

    animationFrame = 0;
    animationTimer = 0.f;
    animationSpeed = 0.1f;
    lastDir = { 1.f, 0.f };

    sprite.setPosition(pos[startNode].first, pos[startNode].second);

}

void Ghost::update(float deltaTime, vector<pair<int, int>>& pos, int pacmanNode)
{
    if (waitingAfterReturn) {
        if (returnClock.getElapsedTime().asSeconds() >= 3.f)
            waitingAfterReturn = false;
        else
            return; 

    }

    Vector2f currentPos = sprite.getPosition();
    Vector2f nodePos(pos[currentNode].first, pos[currentNode].second);

    float minDist = 0.1f;

    // Only update path when standing on a node
    if (abs(currentPos.x - nodePos.x) < minDist && abs(currentPos.y - nodePos.y) < minDist)
    {
        path = allPaths[currentNode][pacmanNode];
        pathIndex = 0;
        
    }

    if (pathIndex + 1 >= path.size())
        return;

    Vector2f targetPos(pos[path[pathIndex + 1]].first, pos[path[pathIndex + 1]].second);

    Vector2f dir = targetPos - currentPos;
    float distance = sqrt(dir.x * dir.x + dir.y * dir.y);



    if (isDead) 
    {
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

    if (isPoisoned) 
    {
        float timerA = poisonedClock.getElapsedTime().asSeconds();

        if (timerA > poisonedDuration)
        {
            isPoisoned = false;
            return; 
        }

        int frame = 0;
        if (timerA < 5.f) 
            frame = (int)((timerA * 4)) % 2; // alternate between frame 1 and 2

        else 
            frame = (int)((timerA - 5.f) * 4) % 4; // frames 1, 2, 3, 4

        poisonedSprite.setTextureRect(IntRect(frame * 16, 0, 16, 16));
        poisonedSprite.setPosition(sprite.getPosition());
    }



    if (distance < speed * deltaTime) 
    {
        sprite.setPosition(targetPos);
        currentNode = path[pathIndex + 1];
        pathIndex++;
        if (isDead and (currentNode == pacmanNode)) //base
        {
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

    else 
    {
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

bool Ghost::shouldUpdate(int i) {
    return (isPoisoned || ghostOut || (i == 0) || (i == 1 && timer > 5) || (i == 2 && timer > 10) || (i == 3 && timer > 15));
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

int Ghost::getCurrentNode() {
    return currentNode;
}


unordered_map<int, unordered_map<int, vector<int>>> Ghost::precomputeAllPaths(unordered_map<int, vector<int>> adjList, int map_num)
{
    if (map_num) adjList[91].clear(), adjList[92].clear();

    unordered_map<int, unordered_map<int, vector<int>>> allPaths;

        for (auto& start_pair : adjList)
        {
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


// up : 0, down : 1, right : 2, left : 3

int dfs(int node,int par, int limit, int steps, unordered_map<int, vector<int>>& adjList,vector <int> &takenNodes) {
    if (steps == limit) return node;
    for (auto child : adjList[node]) {
        bool skip=0;
        for (auto taken : takenNodes) {
            if (child == taken) skip = 1;
        }
        if (skip) continue;
        takenNodes.push_back(child);
        return dfs(child,node, limit, steps + 1, adjList,takenNodes);
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


// Ghost Algorithms functions


// red one --> chase you always 
int Ghost::Blinky(int pacmanNode) {
    return pacmanNode;
}


// orange one --> the confused one
int Ghost::Clyde(int pacmanNode,int map_num) {
    vector<int> corners[2] = { {1,11,45,55} ,{ 1, 10, 81, 90 } };
    if (allPaths[currentNode][pacmanNode].size() > 6)
        return pacmanNode;
    else {
        int mn = 100;
        int target = pacmanNode;
        int mnToPacNode = 100;
        int pacmanclosestnode = pacmanNode;
        for (auto x : corners[map_num]) {
            if (allPaths[pacmanNode][x].size() < mnToPacNode) {
                mnToPacNode = allPaths[pacmanNode][x].size(), pacmanclosestnode = x;
            }
        }
        int pacmanclosest2 = pacmanNode;
        mnToPacNode = 100;

        for (auto x : corners[map_num]) {
            if (allPaths[pacmanNode][x].size() < mnToPacNode and x != pacmanclosestnode) {
                mnToPacNode = allPaths[pacmanNode][x].size(), pacmanclosest2 = x;
            }
        }

        for (auto x : corners[map_num])
            if (allPaths[currentNode][x].size() < mn and x != pacmanclosest2 and x != currentNode and x != pacmanclosestnode)
                mn = allPaths[currentNode][x].size(), target = x;

        return target;
    }
}


// pinky --> 3 steps ahead of pacman
int Ghost::Pinky(int pacmanNode, int dir, unordered_map<int, vector<int>>& adjList,int BlinkyNode) {
    
    if (allPaths[pacmanNode][BlinkyNode].size() > allPaths[pacmanNode][currentNode].size()) {
        return pacmanNode;
    }
    // node -> the node pacman is directed to 
    int node = getNext(pacmanNode, dir);
    int limit = 3;
    vector <int> takenNodes = { pacmanNode };
    return dfs(node, pacmanNode, limit, 0, adjList,takenNodes);

}

// a* with distance between pinky and pacman, with empty nodes weghited 1 and the rest 0
int Ghost::HardMove(int pacmanNode, CircleShape nodes[], unordered_map<int, vector<int>>& adjList, int pent) {
    cerr << "PinkyHard started from: " << currentNode << " to: " << pacmanNode << endl;

    priority_queue<pair<array<int, 3>, vector<int>>> pq;

    int start_g_cost = 0;
    int start_h_cost = (allPaths.count(currentNode) && allPaths[currentNode].count(pacmanNode)) ? allPaths[currentNode][pacmanNode].size() : 10000; // Large cost if no path
    int start_f_cost = start_g_cost + start_h_cost;

    array<int, 3> start_arr = {-start_f_cost, start_g_cost, currentNode};
    pq.push(make_pair(start_arr, vector<int>())); // Initial path is empty

    vector<bool> visited(100, false); // Assuming max node ID < 100 for now

    while(!pq.empty()) {
        // cerr << "pq size : " << pq.size() << endl;
        array<int,3> current_state_arr = pq.top().first;
        vector<int> current_path = pq.top().second;
        pq.pop();

        int current_g_cost = current_state_arr[1];
        int nodeNum = current_state_arr[2];

        if (visited[nodeNum]) {   continue;
        }
        visited[nodeNum] = true;

        if (nodeNum == pacmanNode) {
            if (current_path.empty()) {
                 if (find(adjList[currentNode].begin(), adjList[currentNode]. end(), pacmanNode)
                 !=adjList[currentNode].end() && current_path.empty()) return pacmanNode;
                 return currentNode;
            }
            cerr <<"Path found. First step: "<< current_path[0] << endl;
            return current_path[0];
        }

        if (!adjList.count(nodeNum)) continue;

        for(auto ch : adjList[nodeNum]) {
            if (visited[ch]) { continue;
            }

            int edge_cost;
            if (nodes[ch].getScale().x == 0) {
                edge_cost = pent;
            } else {  edge_cost = 0;
            }

            int new_g_cost = current_g_cost + edge_cost;
            int h_cost = (allPaths.count(ch) && allPaths[ch].count(pacmanNode)) ? allPaths[ch][pacmanNode].size() : 10000; // Large cost if no path info
            int new_f_cost = new_g_cost + h_cost;

            vector<int> new_path = current_path;
            if (new_path.empty()) { new_path.push_back(ch);
            } else { 
                new_path.push_back(ch); }
            
            vector<int> path_to_child;
            if (current_path.empty() && nodeNum == currentNode) { 
                path_to_child.push_back(ch);
            } else if (!current_path.empty()) {
                path_to_child = current_path; 
                
            } else if (!current_path.empty() && nodeNum != currentNode) {
                 path_to_child = current_path; }
            vector<int> first_step_vector;
            if (nodeNum == Ghost::currentNode) { first_step_vector.push_back(ch);
            } else {
                first_step_vector = current_path;  }


            array<int, 3> child_arr = {-new_f_cost, new_g_cost, ch};
            // cerr << "Pushing to PQ: node " << ch << " g:" << new_g_cost << " h:" << h_cost << " f:" << new_f_cost << " first_step: " << (first_step_vector.empty() ? -1 : first_step_vector[0]) << endl;
            pq.push(make_pair(child_arr, first_step_vector));
        }
    }
    cerr << "PinkyHard: No path found to pacmanNode: " << pacmanNode << " from " << currentNode << endl;
    return currentNode;
}


// inky --> 2 steps ahead of pacman + blinky distance
int Ghost::Inky(int pacmanNode, int dir, int BlinkyNode, unordered_map<int, vector<int>>& adjList)
{
    if (allPaths[pacmanNode][currentNode].size() <= 2) {
        return pacmanNode;
    }
    int node = getNext(pacmanNode, dir);
    int dist = allPaths[BlinkyNode][pacmanNode].size();
    int limit = dist + 2;
    vector <int> takenNodes = { pacmanNode };
    return dfs(node, pacmanNode, limit, 0, adjList,takenNodes);

}





Sprite& Ghost::getSprite() {
    return sprite;
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