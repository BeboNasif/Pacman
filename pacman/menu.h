#pragma once
#include "include.h"

class Menu
{
public:
	int choises;
	int pageNumber = 1000;
	int shift = 60;
	vector<Text> mainmenu;
	Texture pill;
	vector<RectangleShape> pills;
	int selected = 0;
	Font font;
	int height = 1080;
	int positionOfFace = 60;
	int x = 40;
	RectangleShape DownFace;
	vector<bool> pillConsumed;
	int eatenPills = 0;
	Clock deltaClock;
	float downFaceSpeed = 80.f;
	Clock resetDelayClock;
	bool delayStarted = false;
	vector<Clock> pillTimers;
	vector<bool> pillDelayStarted;

	RectangleShape Face;
	vector<Texture> faceFrames;
	int currentFrame = 0;
	Clock animationClock;
	Time frameDuration;


	Menu();
	void Face_intilization();
	void MoveDown(int& selected, int choises);
	void MoveUp(int& selected, int choises);
	void menu1(RenderWindow& window);
};

	/*void Play_menu(RenderWindow& window, int& GameMode);
	void options_menu(RenderWindow& window);
	void credits(RenderWindow& window);
	void instructions(RenderWindow& window);*/