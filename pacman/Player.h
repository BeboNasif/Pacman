#pragma once
#include "include.h"
#include <unordered_map>

class Player
{

private:
    enum State { idle, wmove, smove, amove, dmove, dead };
    Sprite& player;
    float playerdeltatime;
    float walk_speed;
    float player_scale;
    Vector2f velocity;
    Vector2f initial_position;
    int ImageCounter;
    int maximagecounter;
    float AnimationCounter;
    bool animation_running;
    int live;

    State tmp_state;


    Texture Idle;
    unordered_map<State, vector<Texture>> animationTextures;

public:
    CircleShape nodes[93];
    enum Dir { up, down, right, left };
    Dir cur_dir;
    Vector2f frozenPosition;
    bool gameOver = false;
    State curr_state;
    int cur_node;
    int next_node;
    bool isDead;
    bool reachedNode;
    Player(Sprite& p);
    void setDeltaTime(float dt);
    void handleInput(unordered_map<int, vector<int>>& adj, vector<pair<int, int>>& pos);
    void updateMovement(unordered_map<int, vector<int>> adj,vector<pair<int, int>>& pos);
    void updatePlace(Vector2f window);
    void updateAnimation();
    void die();
    int getCurrentNode(std::vector<std::pair<int, int>>& pos, sf::Vector2f playerPosition);

private:
    void resetAfterDeath();
    void updateAnimationCounter(float speedThreshold);
};
