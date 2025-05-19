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
    sf::Vector2f lastDir;
    void updateAnimation();

    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    int currentNode;
    float elapsedTime;
    int pathIndex;
    std::vector<int> path;
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> allPaths;

public:
    Ghost(int startNode, const std::string& texturePath, unordered_map<int, std::vector<int>> adjList, const std::vector<std::pair<int, int>>& pos);
    void update(float deltaTime, const std::unordered_map<int, std::vector<int>>& adjList, const std::vector<std::pair<int, int>>& pos, int pacmanNode);
    void draw(sf::RenderWindow& window);
    int getCurrentNode() const;
    static std::unordered_map<int, std::unordered_map<int, std::vector<int>>> precomputeAllPaths(const std::unordered_map<int, std::vector<int>>& adjList);
    const sf::Sprite& getSprite() const;
    void reset(int startNode, const std::vector<std::pair<int, int>>& pos);

    int EL7okooma(int pacmanNode,int dir, unordered_map<int, std::vector<int>>& adjList); //pacmannode + 3
    int ELSaad(int pacmanNode, int dir, int Ad3kdist, unordered_map<int, std::vector<int>>& adjList); // pacmannode + ad3ak distance 
    int Amoor(int pacmanNode); //afraid one (runs to corners when near pacman) 
    int Ad3k(int pacmanNode); // red (  ad3k >:)  )
};


