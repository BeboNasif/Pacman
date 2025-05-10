#include"include.h"
#include"Menu.h"
#include "menu_Bg.h"
#include "Player.h"
#include "Sounds.h"
#include "Map.h"

Map mp;
Menu menu;
Sounds sound;
RenderWindow window(VideoMode(1920, 1080), "Pacman");
void Gameplay() {
    Sprite pacmanSprite;
    Sprite MapSprite;
    Texture mapText;
    mapText.loadFromFile("Assets/FullMap.png");
    MapSprite.setTexture(mapText);
    MapSprite.setScale(1.65, 1.65);
    MapSprite.setOrigin(mapText.getSize().x / 2, mapText.getSize().y / 2);
    MapSprite.setPosition(1920 / 2, 1080 / 2);
    Player pacman(pacmanSprite);
    mp.init();
    mp.printAdjList();
    RectangleShape ghostShape(Vector2f(40.f, 40.f));
    ghostShape.setFillColor(Color::Red);
    ghostShape.setPosition(200.f, 200.f);

    Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.key.code == Keyboard::Escape)
                return;
        }

        if (!pacman.gameOver) {
            pacman.setDeltaTime(deltaTime);
            pacman.handleInput();
            //cout << pacmanSprite.getPosition().x << " " << pacmanSprite.getPosition().y << endl;
            pacman.updateMovement();
            pacman.updateAnimation();
            pacman.updatePlace(Vector2f(window.getSize().x, window.getSize().y));
            if (ghostShape.getGlobalBounds().intersects(pacmanSprite.getGlobalBounds())) {

                pacman.die();

            }
        }

        window.clear();
        window.draw(MapSprite);
        window.draw(pacmanSprite);
        window.draw(ghostShape);
        window.display();
    }
}

int main()
{
    window.setFramerateLimit(120);
    menu.menu1(window);
}