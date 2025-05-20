// Menu.h
#pragma once

#include "include.h"

class Menu {
public:
    Menu();
    void menu1(RenderWindow& window);
    void Play_menu(RenderWindow& window);
    void GFX(RenderWindow& window);
	void sound_options(RenderWindow& window);
    void player_controls(RenderWindow& window, Keyboard::Key& moveLeftKey, Keyboard::Key& moveRightKey, Keyboard::Key& moveUpkey, Keyboard::Key& moveDownkey);
    void options_menu(RenderWindow& window);
	void credits(RenderWindow& window);
	void instructions(RenderWindow& window);

private:
    void updateFaces(float dt);
    void Face_intilization();
    void MoveDown(int& sel, int choices);
    void MoveUp(int& sel, int choices);
    void changeKeyMapping(int& action, Keyboard::Key newKey, Keyboard::Key& moveLeftKey, Keyboard::Key& moveRightKey, Keyboard::Key& moveUpkey, Keyboard::Key& moveDownkey, Menu& menu9);
    string keyboardKeyToString(sf::Keyboard::Key key);

    // ——— Menu text items ———
    vector<Text> mainmenu;
    int choises;
    int selected;
    Font font;

    // ——— Face & animation ———
    vector<Texture> faceFrames;
    RectangleShape Face, DownFace;
    float downFaceSpeed;
    int currentFrame;
    Clock animationClock;
    Time frameDuration;

    // ——— Pill logic ———
    vector<RectangleShape> pills;
    Texture pill;
    vector<bool> pillConsumed;
    vector<bool> pillDelayStarted;
    vector<Clock> pillTimers;
    Clock resetDelayClock;
    int eatenPills;
    bool delayStarted;

    // ——— Timing & layout ———
    Clock deltaClock;
    float positionOfFace;
};
