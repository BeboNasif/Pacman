#include"include.h"
#include"Menu.h"
#include "menu_Bg.h"

Menu menu;
RenderWindow window(VideoMode(1920, 1080), "Pacman", Style::Close | Style::Fullscreen);


int main()
{
	window.setFramerateLimit(120);
	//MainMenu
	menu.menu1(window);
}