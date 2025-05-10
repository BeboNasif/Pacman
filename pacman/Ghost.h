#pragma once
#include "include.h"

class Ghost
{
public:
    Ghost(sf::Texture& texture, sf::Vector2f startPos);

    void draw(sf::RenderWindow& window);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    static std::unordered_map<int, std::unordered_map<int, std::vector<int>>> precomputeAllPaths(std::unordered_map<int, std::vector<int>>& adjList);

    // Method for moving the ghost along a path
    void moveAlongPath(float deltaTime, const std::vector<int>& path, int& pathIndex, float ghostSpeed, const std::unordered_map<int, sf::Vector2f>& nodes);

private:
    sf::Sprite sprite;
};
