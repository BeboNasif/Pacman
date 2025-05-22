#pragma once

#include "include.h"

class Menu {
public:
    Menu();
    void menu1(RenderWindow& window);
    void high_scores(RenderWindow& window);
    void player_name_input(RenderWindow& window);
    void Play_menu(RenderWindow& window);
    void GFX(RenderWindow& window);
    void sound_options(RenderWindow& window);
    void player_controls(RenderWindow& window);
    void options_menu(RenderWindow& window);
    void credits(RenderWindow& window);
    void instructions(RenderWindow& window);
    void Pause(RenderWindow& window, Texture gametexture);
    bool exit = 0;
    bool play_again = 0;
    bool show_name_input = true;


private:
    void reset();
    static void set_sounds();
    void updateFaces(float dt);
    void Face_intilization();
    void MoveDown(int& sel, int choices);
    void MoveUp(int& sel, int choices);
    void changeKeyMapping(int& action, Keyboard::Key newKey, Menu& menu9);

    // ��� Menu text items ���
    vector<Text> mainmenu;
    int choises;
    int selected;
    Font font;

    // ��� Face & animation ���
    vector<Texture> faceFrames;
    RectangleShape Face, DownFace;
    float downFaceSpeed;
    int currentFrame;
    Clock animationClock;
    Time frameDuration;

    // ��� Pill logic ���
    vector<RectangleShape> pills;
    Texture pill;
    vector<bool> pillConsumed;
    vector<bool> pillDelayStarted;
    vector<Clock> pillTimers;
    Clock resetDelayClock;
    int eatenPills;
    bool delayStarted;

    // ��� Timing & layout ���
    Clock deltaClock;
    float positionOfFace;

};
