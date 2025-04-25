#include "Player.h"
#include "include.h"

using namespace sf;
Player::Player(Sprite& p) : player(p) {
    velocity = { 0.f, 0.f };
    walk_speed = 150.f;
    player_scale = 2.5;
    initial_position = { 100,100 };
    State curr_state = idle;
    player.setScale(player_scale, player_scale);
    player.setPosition(initial_position);
    Idle.loadFromFile("Assets/Textures/pacman/neutral.png");
    live = 3;
    animation_running = false;
    isDead = false;
    ImageCounter = 0;
    maximagecounter = 0;
    AnimationCounter = 0;

    wAnimation.resize(2);
    sAnimation.resize(2);
    aAnimation.resize(2);
    dAnimation.resize(2);
    DeathAnimation.resize(11);
    for (int i = 0; i < 2; i++) {
        wAnimation[i].loadFromFile("Assets/Textures/pacman/up_" + to_string(i) + ".png");
        sAnimation[i].loadFromFile("Assets/Textures/pacman/down_" + to_string(i) + ".png");
        aAnimation[i].loadFromFile("Assets/Textures/pacman/left_" + to_string(i) + ".png");
        dAnimation[i].loadFromFile("Assets/Textures/pacman/right_" + to_string(i) + ".png");
    }
    for (int i = 0; i < 11; i++)
        DeathAnimation[i].loadFromFile("Assets/Textures/pacman/d-" + to_string(i) + ".png");
}

void Player::setDeltaTime(float dt) {
    playerdeltatime = dt;
}

void Player::handleInput() {
    if (!animation_running) {
        if (Keyboard::isKeyPressed(Keyboard::A)|| Keyboard::isKeyPressed(Keyboard::Left))
            curr_state = amove;
        else if (Keyboard::isKeyPressed(Keyboard::D)|| Keyboard::isKeyPressed(Keyboard::Right))
            curr_state = dmove;
        else if (Keyboard::isKeyPressed(Keyboard::S)|| Keyboard::isKeyPressed(Keyboard::Down))
            curr_state = smove;
        else if (Keyboard::isKeyPressed(Keyboard::W)|| Keyboard::isKeyPressed(Keyboard::Up))
            curr_state = wmove;
       
    }
    
}
void Player::updateMovement() {
    velocity = { 0.f, 0.f };
    
    switch(curr_state) {
            case wmove: velocity.y = -walk_speed * playerdeltatime; break;
            case smove: velocity.y = walk_speed * playerdeltatime; break;
            case amove: velocity.x = -walk_speed * playerdeltatime; break;
            case dmove: velocity.x = walk_speed * playerdeltatime; break;
            default: break;
    }

    player.move(velocity);
}

void Player::updatePlace(Vector2f window) {

    if (player.getPosition().x > window.x) {
        player.setPosition(0, player.getPosition().y);
    }
    else if (player.getPosition().x < 0) {
        player.setPosition(window.x, player.getPosition().y);
    }

    if (player.getPosition().y > window.y) {
        player.setPosition(player.getPosition().x, 0);
    }
    else if (player.getPosition().y < 0) {
        player.setPosition(player.getPosition().x, window.y);
    }


}
void Player::updateAnimation() {
    switch (curr_state) {
    case idle:  player.setTexture(Idle); ImageCounter = 0; break;
    case wmove: maximagecounter = 2; player.setTexture(wAnimation[ImageCounter]); updateAnimationCounter(0.08f); break;
    case dmove: maximagecounter = 2; player.setTexture(dAnimation[ImageCounter]); updateAnimationCounter(0.09f); break;
    case smove: maximagecounter = 2; player.setTexture(sAnimation[ImageCounter]); updateAnimationCounter(0.08f); break;
    case amove: maximagecounter = 2; player.setTexture(aAnimation[ImageCounter]); updateAnimationCounter(0.09f); break;
    case dead:  maximagecounter = 11; player.setTexture(DeathAnimation[ImageCounter]); updateAnimationCounter(0.07f); break;
    }
}


void Player::updateAnimationCounter(float speedThreshold) {
    AnimationCounter += playerdeltatime;
    if (AnimationCounter >= speedThreshold) {
        AnimationCounter = 0;
        ImageCounter++;
        if (ImageCounter >= maximagecounter) {
            ImageCounter = 0;
            if (curr_state == dead  ) {
                if (live > 0) {
                    cout << live<<endl;
                    live--;
                    resetAfterDeath();
                }
                else {
                    cout << "Game Over"<<endl;
                    gameOver = true;
                    return ;
                }
            }
            
        }
    }
}
void Player::die() {
    if (!isDead && !animation_running) {
        curr_state = dead;
        animation_running = true;
        ImageCounter = 0;
        AnimationCounter = 0;
        isDead = true;
    }
}

void Player::resetAfterDeath() {
    isDead = false;
    animation_running = false;
    curr_state = idle;
    ImageCounter = 0;
    AnimationCounter = 0;
    player.setTexture(Idle);
    player.setPosition(initial_position);
}


