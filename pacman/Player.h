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
    Vector2f velocity ;
    Vector2f initial_position;
    int ImageCounter ;
    int maximagecounter;
    float AnimationCounter ;
    bool animation_running ;
    int live;
    State curr_state ;
    int cur_node;
    enum Dir { up, down, right, left };
    Dir cur_dir;
    

    Texture Idle;
    unordered_map<State, vector<Texture>> animationTextures;

public:
    Vector2f frozenPosition;
    bool gameOver = false;
    bool isDead;
    Player(Sprite& p);
    void setDeltaTime(float dt);
    void handleInput();
    void updateMovement();
    void updatePlace(Vector2f window);
    void updateAnimation();
    void die();
  
private:
    void resetAfterDeath();
    void updateAnimationCounter(float speedThreshold);
};

