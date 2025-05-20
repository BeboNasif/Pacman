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
    int currentNode;
    float elapsedTime;
    int pathIndex;
    vector<int> path;
    sf::Texture poisonedTexture;
    sf::Sprite poisonedSprite;
    sf::Clock poisonedClock;
    float poisonedDuration = 8.f;
    sf::Texture deadTexture;
    sf::Sprite deadSprite;
    Clock returnClock;
    bool waitingAfterReturn = false;

public:
    bool isDead = 0;
    bool isPoisoned = false;
    bool ghostOut = 0;
    bool shouldUpdate(int i,float timer);
    static unordered_map<int, unordered_map<int, vector<int>>> allPaths;
    Ghost(int startNode, string texturePath, unordered_map<int, vector<int>> adjList, vector<pair<int, int>> pos);
    void update(float deltaTime, vector<pair<int, int>>& pos, int pacmanNode);
    void draw(RenderWindow& window);
    int getCurrentNode();
    static unordered_map<int, unordered_map<int, vector<int>>> precomputeAllPaths(unordered_map<int, vector<int>> adjList);
    Sprite& getSprite();
    void reset(int startNode, vector<pair<int, int>>& pos);

    int EL7okooma(int pacmanNode, int dir, unordered_map<int, vector<int>>& adjList); //pacmannode + 3
    int ELSaad(int pacmanNode, int dir, int Ad3kdist, unordered_map<int, vector<int>>& adjList); // pacmannode + ad3ak distance 
    int Amoor(int pacmanNode); //afraid one (runs to corners when near pacman) 
    int Ad3k(int pacmanNode); // red (  ad3k >:)  )
    void poisoned(const string& poisonedTexturePath);
    void die(const string& deadTexturePath);
};