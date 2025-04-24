#include "menu_Bg.h"
extern Vector2f window_size;
extern float dt;

void menu_Bg::back_ground(RenderWindow& window)
{
	texture.loadFromFile("Assets/Textures/menu_wallpaper.jpg");
	bg.setTexture(&texture);
	bg.setSize(Vector2f(window.getSize()));
}