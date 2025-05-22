#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
using namespace sf;
using namespace std;

class Ghost {
private:
    int animationFrame;
    float animationTimer;
    float animationSpeed;
    Vector2f lastDir;
    void updateAnimation();
    Texture texture;
    Sprite sprite;
    float speed;
    float elapsedTime;
    int pathIndex;
    vector<int> path;
    Texture poisonedTexture;
    Sprite poisonedSprite;
    Clock poisonedClock;
    float poisonedDuration = 8.f;
    Texture deadTexture;
    Sprite deadSprite;
    Clock returnClock;
    bool waitingAfterReturn = false;

public:
    int currentNode;
    static float timer;
    bool isDead = 0;
    bool isPoisoned = false;
    bool ghostOut = 0;
    bool shouldUpdate(int i);
    static unordered_map<int, unordered_map<int, vector<int>>> allPaths;
    Ghost(int startNode, string texturePath, unordered_map<int, vector<int>> adjList, vector<pair<int, int>> pos);
    void update(float deltaTime, vector<pair<int, int>>& pos, int pacmanNode);
    void draw(RenderWindow& window);
    int getCurrentNode();
    static unordered_map<int, unordered_map<int, vector<int>>> precomputeAllPaths(unordered_map<int, vector<int>> adjList, int map_num);
    Sprite& getSprite();
    void reset(int startNode, vector<pair<int, int>>& pos);

    int Pinky(int pacmanNode, int dir, unordered_map<int, vector<int>>& adjList,int BlinkyNode); // pacmannode + 3
    int HardMove(int pacmanNode, CircleShape nodes[], unordered_map<int, vector<int>> &adjList, int pent);
    int Inky(int pacmanNode, int dir, int BlinkyNode, unordered_map<int, vector<int>> &adjList); // pacmannode + Blinky distance
    int Clyde(int pacmanNode,int map_num); //afraid one (runs to corners when near pacman) 
    int Blinky(int pacmanNode); // red (  Blinky >:)  )
    void poisoned(const string& poisonedTexturePath);
    void die(const string& deadTexturePath, int& score);
};