#include "Ghost.h"
#include "include.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

// Precompute paths from all nodes to all other nodes using BFS
std::unordered_map<int, std::unordered_map<int, std::vector<int>>> Ghost::precomputeAllPaths(std::unordered_map<int, std::vector<int>>& adjList)
{
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> allPaths;

    // Loop through each node as the start node
    for (const auto& [start, _] : adjList)
    {
        std::queue<int> q;
        std::unordered_map<int, int> parent;  // To reconstruct paths
        std::unordered_set<int> visited;      // To track visited nodes
        q.push(start);
        visited.insert(start);
        parent[start] = -1;

        // BFS to find the shortest path from start to all other nodes
        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            // Visit neighbors
            for (int neighbor : adjList.at(current))
            {
                if (visited.find(neighbor) == visited.end())
                {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        // Reconstruct the path from start to all other nodes
        for (auto& [end, _] : parent)
        {
            std::vector<int> path;
            int cur = end;

            // Backtrack from 'end' to 'start' to build the path
            while (cur != -1)
            {
                path.push_back(cur);
                cur = parent[cur];
            }

            // Reverse the path to make it from start to end
            std::reverse(path.begin(), path.end());
            allPaths[start][end] = path;
        }
    }

    return allPaths;
}

// Constructor for Ghost
Ghost::Ghost(sf::Texture& texture, sf::Vector2f startPos)
{
    sprite.setTexture(texture);
    sprite.setPosition(startPos);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);  // Set origin to center for rotation/positioning
    sprite.setScale(2.8f, 2.8f);  // Set scale for appropriate ghost size
}

// Draw the ghost on the screen
void Ghost::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

// Set the ghost's position
void Ghost::setPosition(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}

// Get the ghost's position
sf::Vector2f Ghost::getPosition() const
{
    return sprite.getPosition();
}
