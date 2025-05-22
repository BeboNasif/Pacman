#include "Player.h"
#include "include.h"
#include "FilesController.h"
#include "KeyboardKEYS.h"
using namespace sf;

extern int character;
string Player::playerName = "Player1";
map<string, string> Player::settings;

Player::Player(Sprite& p, const Vector2f& startPos) : player(p), initial_position(startPos) {
    velocity = { 0.f, 0.f };
    walk_speed = 150.f;
    player_scale = (character == 1 ? 2.3f : 2.0f);
    reachedNode = 1;

    curr_state = idle;
    tmp_state = idle;
    player.setScale(player_scale, player_scale);
    player.setPosition(initial_position);

    Idle.loadFromFile("Assets/Textures/pacman/neutral" + string(character == 1 ? "_m" : "") + ".png");
    live = 2;
    ImageCounter = 0;
    maximagecounter = 0;
    AnimationCounter = 0;
    cur_dir = left;
    cur_node = 65;

    animationTextures[wmove] = vector<Texture>(2);
    animationTextures[smove] = vector<Texture>(2);
    animationTextures[amove] = vector<Texture>(2);
    animationTextures[dmove] = vector<Texture>(2);
    animationTextures[dead] = vector<Texture>(11);

    for (int i = 0; i < 2; i++) {
        animationTextures[wmove][i].loadFromFile("Assets/Textures/pacman/up_" + to_string(i) + (character == 1 ? "m" : "") + ".png");
        animationTextures[smove][i].loadFromFile("Assets/Textures/pacman/down_" + to_string(i) + (character == 1 ? "m" : "") + ".png");
        animationTextures[amove][i].loadFromFile("Assets/Textures/pacman/left_" + to_string(i) + (character == 1 ? "m" : "") + ".png");
        animationTextures[dmove][i].loadFromFile("Assets/Textures/pacman/right_" + to_string(i) + (character == 1 ? "m" : "") + ".png");
    }

    for (int i = 0; i < 11; i++) {
        animationTextures[dead][i].loadFromFile("Assets/Textures/pacman/d-" + to_string(i) + ".png");
    }
}


void Player::setInitPos(Vector2f pos)
{
    initial_position = pos;
}

void Player::initSettings(int map_num) {
  
    settings = FilesController::getPlayerSettings(playerName);
    if (settings.empty()) {
        settings["moveLeftKey"] = "Left";
        settings["moveRightKey"] = "Right";
        settings["moveUpkey"] = "Up";
        settings["moveDownkey"] = "Down";
        settings["maxScore"] = "0";
        settings["sound"] = "50";
        settings["music"] = "50";
    }
}

void Player::setDeltaTime(float dt) {
    playerdeltatime = dt;
}


void Player::handleInput(unordered_map<int, vector<int>>& adj, vector<pair<int, int>>& pos, int map_num) {
    
        // get the key pressed name 
        sf::Keyboard::Key moveLeftKey = KeyboardKEYS::KeyNameToKey(settings["moveLeftKey"]);
        sf::Keyboard::Key moveRightKey = KeyboardKEYS::KeyNameToKey(settings["moveRightKey"]);
        sf::Keyboard::Key moveUpkey = KeyboardKEYS::KeyNameToKey(settings["moveUpkey"]);
        sf::Keyboard::Key moveDownkey = KeyboardKEYS::KeyNameToKey(settings["moveDownkey"]);
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
        
        cur_node = getCurrentNode(pos, player.getPosition(),map_num);
}

void Player::updateMovement(unordered_map<int, vector<int>> adj, vector<pair<int, int>>& pos, int map_num) {
    int cleared = 17 * (map_num ? 2 : 1);
    adj[cleared].clear();
    adj[cleared].push_back(cleared-1), adj[cleared].push_back(cleared + 1);

    Vector2f currentPos = player.getPosition();
    float invert = 1;
    if (curr_state != dead) {
        for (int i = 1; i <= (map_num ? 90 : 55); i++) {
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
                    if (i == 91) {
                        next_node = 92;
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
                    if (i == 92) {
                        next_node = 91;
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
}

void Player::updatePlace(Vector2f window) {

    if (player.getPosition().x > 1475) {
        player.setPosition(400, player.getPosition().y);
    }
    else if (player.getPosition().x < 400) {
        player.setPosition(1475, player.getPosition().y);
    }


}
void Player::updateAnimation(vector<Ghost*>& ghosts,Map &mp,int map_num ) {
    if (curr_state == idle) {
        player.setTexture(Idle);
        ImageCounter = 0;
    }
    else {
        maximagecounter = animationTextures[curr_state].size();

        player.setTexture(animationTextures[curr_state][ImageCounter]);

        if (curr_state == dead)
            updateAnimationCounter(0.07f, ghosts, mp, map_num);
        else
            updateAnimationCounter(0.08f,ghosts,mp, map_num);
    }


}


void Player::updateAnimationCounter(float speedThreshold,vector<Ghost*> &ghosts, Map &mp, int map_num) {
    AnimationCounter += playerdeltatime;
    if (AnimationCounter >= speedThreshold) {
        AnimationCounter = 0;
        ImageCounter++;
        if (ImageCounter >= maximagecounter) {
            ImageCounter = 0;
            if (curr_state == dead) {
                if (live > 0) {
                    live--;
                    resetAfterDeath();
                    int i = 0;
                    vector<int> ghostNodeBegins[2] = { {17,27,28,29},{34,40,41,42} };
                    vector<int>  starts = ghostNodeBegins[map_num];
                    for (auto& ghost : ghosts) {
                        ghost->reset(starts[i], mp.pos[map_num]);
                        i++;
                    }
                }
                else {
                    gameOver = true;
                    return;
                }
            }

        }
    }
}
void Player::die() {
        curr_state = dead;
}

void Player::resetAfterDeath() {
    curr_state = idle; 
    tmp_state = idle;
    player.setPosition(initial_position);
}

int Player::getCurrentNode(std::vector<std::pair<int, int>>& pos, sf::Vector2f playerPosition,  int map_num) {
    int closestNode = -1;
    float minDistance = FLT_MAX;

    for (int i = 1; i <= (map_num ? 90 : 55); i++) {
        float distance = sqrt(pow(pos[i].first - playerPosition.x, 2) + pow(pos[i].second - playerPosition.y, 2));
        if (distance < minDistance) {
            minDistance = distance;
            closestNode = i;
        }
    }
    return closestNode;
}