#include "Player.h"
#include "include.h"

using namespace sf;

Keyboard::Key moveLeftKey = Keyboard::Left;
Keyboard::Key moveRightKey = Keyboard::Right;
Keyboard::Key moveUpkey = Keyboard::Up;
Keyboard::Key moveDownkey = Keyboard::Down;

Player::Player(Sprite& p) : player(p) {
    velocity = { 0.f, 0.f };
    walk_speed = 150.f;
    player_scale = 2.0;
    reachedNode = 1;
    initial_position = { 880 + 20,789 + 20 };
    curr_state = idle;
    tmp_state = idle;
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
        if (Keyboard::isKeyPressed(moveLeftKey)) {
            cur_dir = left;
            tmp_state = amove;
        }
        else if (Keyboard::isKeyPressed(moveRightKey)) {
            cur_dir = right;
            tmp_state = dmove;
        }
        else if (Keyboard::isKeyPressed(moveDownkey)) {
            cur_dir = down;
            tmp_state = smove;
        }
        else if (Keyboard::isKeyPressed(moveUpkey)) {
            cur_dir = up;
            tmp_state = wmove;
        }

        // Update the current node based on the player's position
        cur_node = getCurrentNode(pos, player.getPosition());
    }
}

void Player::updateMovement(unordered_map<int, vector<int>>& adj, vector<pair<int, int>>& pos) {

    Vector2f currentPos = player.getPosition();
    float invert = 1;
    for (int i = 1; i <= 64; i++) {
        if (abs(pos[i].first - currentPos.x) < 3 && abs(pos[i].second - currentPos.y) < 3) {
            int next_node = -1;

            if (tmp_state == wmove) {
                for (int neighbor : adj[i]) {
                    if (pos[neighbor].second < pos[i].second) {
                        next_node = neighbor;
                        invert = 1;
                        break;
                    }
                }
            }
            else if (tmp_state == smove) {
                for (int neighbor : adj[i]) {
                    if (pos[neighbor].second > pos[i].second) {
                        next_node = neighbor;
                        invert = 1;
                        break;
                    }
                }
            }
            else if (tmp_state == amove) {
                if (i == 23) {
                    next_node = 30;
                    invert = -1;
                    break;
                }
                for (int neighbor : adj[i]) {
                    if (pos[neighbor].first < pos[i].first) {
                        next_node = neighbor;
                        invert = 1;
                        break;
                    }
                }
            }
            else if (tmp_state == dmove) {
                if (i == 30) {
                    next_node = 23;
                    invert = -1;
                    break;
                }
                for (int neighbor : adj[i]) {
                    if (pos[neighbor].first > pos[i].first) {
                        next_node = neighbor;
                        invert = 1;
                        break;
                    }
                }
            }

            if (next_node != -1) {
                cur_node = next_node;
                switch (tmp_state) {
                case wmove: velocity.y = -walk_speed * playerdeltatime, velocity.x = 0; curr_state = tmp_state; break;
                case smove: velocity.y = walk_speed * playerdeltatime, velocity.x = 0; curr_state = tmp_state; break;
                case amove: velocity.x = -invert * walk_speed * playerdeltatime, velocity.y = 0; curr_state = tmp_state; break;
                case dmove: velocity.x = invert * walk_speed * playerdeltatime, velocity.y = 0; curr_state = tmp_state; break;
                default: break;
                }
                break;
            }
            else {
                bool keepMoving = false;
                for (int neighbor : adj[cur_node]) {
                    if ((curr_state == wmove && pos[neighbor].second < pos[cur_node].second) ||
                        (curr_state == smove && pos[neighbor].second > pos[cur_node].second) ||
                        (curr_state == amove && pos[neighbor].first < pos[cur_node].first) ||
                        (curr_state == dmove && pos[neighbor].first > pos[cur_node].first)) {
                        keepMoving = true;
                        break;
                    }
                }

                if (!keepMoving) {
                    velocity = { 0,0 };
                    break;
                }
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
            if (curr_state == dead) {
                if (live > 0) {
                    cout << live << endl;
                    live--;
                    resetAfterDeath();
                }
                else {
                    cout << "Game Over" << endl;
                    gameOver = true;
                    return;
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