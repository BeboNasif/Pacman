// Menu.h
#pragma once

#include "include.h"

class Menu {
public:
    Menu();
    void menu1(RenderWindow& window);
    void Play_menu(RenderWindow& window);
	void sound_options(RenderWindow& window);
	void options_menu(RenderWindow& window);
	void credits(RenderWindow& window);
	void instructions(RenderWindow& window);

private:
    void updateFaces(float dt);
    void Face_intilization();
    void MoveDown(int& sel, int choices);
    void MoveUp(int& sel, int choices);

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
    int pageNumber;
};
