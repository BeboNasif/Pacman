#include "Player.h"
#include "include.h"

using namespace sf;
Player::Player(Sprite& p) : player(p) {
    velocity = { 0.f, 0.f };
    walk_speed = 150.f;
    player_scale = 2.0;
    reachedNode = 1;
    initial_position = { 880+ 20,789 + 20};
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
    cur_dir = left;
    cur_node = 45;
    animationTextures[wmove] = vector<Texture>(2);
    animationTextures[smove] = vector<Texture>(2);
    animationTextures[amove] = vector<Texture>(2);
    animationTextures[dmove] = vector<Texture>(2);
    animationTextures[dead] = vector<Texture>(11);
   
    for (int i = 0; i < 2; i++) {
        animationTextures[wmove][i].loadFromFile("Assets/Textures/pacman/up_" + to_string(i) + ".png");
        animationTextures[smove][i].loadFromFile("Assets/Textures/pacman/down_" + to_string(i) + ".png");
        animationTextures[amove][i].loadFromFile("Assets/Textures/pacman/left_" + to_string(i) + ".png");
        animationTextures[dmove][i].loadFromFile("Assets/Textures/pacman/right_" + to_string(i) + ".png");
    }
    for (int i = 0; i < 11; i++) {
        animationTextures[dead][i].loadFromFile("Assets/Textures/pacman/d-" + to_string(i) + ".png");
    }
}

void Player::setDeltaTime(float dt) {
    playerdeltatime = dt;
}


void Player::handleInput(unordered_map<int, vector<int>>& adj, vector<pair<int, int>>& pos) {
    if (!animation_running) {
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) {
            cur_dir = left;
            curr_state = amove;
        }
        else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
            cur_dir = right;
            curr_state = dmove;
        }
        else if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
            cur_dir = down;
            curr_state = smove;
        }
        else if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
            cur_dir = up;
            curr_state = wmove;
        }

        // Update the current node based on the player's position
        cur_node = getCurrentNode(pos, player.getPosition());
    }
}




void Player::updateMovement(vector<pair<int,int>> &pos) {
    //velocity = { 0,0 };


    for (int i = 1; i <= 64; i++) {
        if (abs(pos[i].first - player.getPosition().x) < 3 and abs(pos[i].second- player.getPosition().y) < 3) {
            switch (curr_state) {
                case wmove: velocity.y = -walk_speed * playerdeltatime, velocity.x = 0; 
                break;
            case smove: velocity.y = walk_speed * playerdeltatime, velocity.x = 0;
                break;
            case amove: velocity.x = -walk_speed * playerdeltatime ,velocity.y = 0;
                break;
            case dmove: velocity.x = walk_speed * playerdeltatime, velocity.y = 0;
                break;
            default: 
                break;  
            }
        }
    }
    

     player.move(velocity);
}

void Player::updatePlace(Vector2f window) {

    if (player.getPosition().x > 1475) {
        player.setPosition(400, player.getPosition().y);
    }
    else if (player.getPosition().x < 400) {
        player.setPosition(1475, player.getPosition().y);
    }

    if (player.getPosition().y > window.y) {
        player.setPosition(player.getPosition().x, 0);
    }
    else if (player.getPosition().y < 0) {
        player.setPosition(player.getPosition().x, window.y);
    }


}
void Player::updateAnimation() {
    if (curr_state == idle) {
        player.setTexture(Idle);
        ImageCounter = 0;
    }
    else {
        maximagecounter = animationTextures[curr_state].size();

        player.setTexture(animationTextures[curr_state][ImageCounter]);

        if (curr_state == dead)
            updateAnimationCounter(0.07f);
        else
            updateAnimationCounter(0.08f);
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

int Player::getCurrentNode(std::vector<std::pair<int, int>>& pos, sf::Vector2f playerPosition) {
    int closestNode = -1;
    float minDistance = FLT_MAX;

    for (int i = 1; i <= 64; i++) {
        float distance = sqrt(pow(pos[i].first - playerPosition.x, 2) + pow(pos[i].second - playerPosition.y, 2));
        if (distance < minDistance) {
            minDistance = distance;
            closestNode = i;
        }
    }
    return closestNode;
}
