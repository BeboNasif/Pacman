#pragma once
#include "include.h"
#include <unordered_map>
#include "menu.h" 

class Player
{

private:
    enum State { idle, wmove, smove, amove, dmove, dead };
    float playerdeltatime;
    float player_scale;
    Vector2f velocity;
    int ImageCounter;
    int maximagecounter;
    float AnimationCounter;
    State tmp_state;
    Texture Idle;
    unordered_map<State, vector<Texture>> animationTextures;
    void resetAfterDeath();
    void updateAnimationCounter(float speedThreshold, vector<Ghost*>& ghosts, Map& mp, int map_num);

public:
    Sprite& player;
    int live;
    Vector2f initial_position;
    static string playerName;
    static map<string, string> settings;
    float walk_speed;
    CircleShape nodes[93];
    enum Dir { up, down, right, left };
    Dir cur_dir;
    Vector2f frozenPosition;
    bool gameOver = false;
    State curr_state;
    int cur_node;
    int next_node;
    bool reachedNode;
    Player(Sprite& p, const Vector2f& startPos);
    void setInitPos(Vector2f pos);
    static void initSettings(int map_num);
    void setDeltaTime(float dt);
    void handleInput(unordered_map<int, vector<int>>& adj, vector<pair<int, int>>& pos, int map_num);
    void updateMovement(unordered_map<int, vector<int>> adj, vector<pair<int, int>>& pos, int map_num);
    void updatePlace(Vector2f window);
    void updateAnimation(vector<Ghost*>& ghosts, Map& mp, int map_num);
    void die();
    int getCurrentNode(std::vector<std::pair<int, int>>& pos, sf::Vector2f playerPosition, int map_num);
};
