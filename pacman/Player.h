#pragma once
#include "include.h"

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
    int live;
    bool isDead ;
    State curr_state ;
    

    Texture Idle;
    vector <Texture> wAnimation, sAnimation, aAnimation, dAnimation, DeathAnimation;

public:
    bool gameOver = false;
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

