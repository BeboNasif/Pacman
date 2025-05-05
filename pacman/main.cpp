#include"include.h"
#include"Menu.h"
#include "menu_Bg.h"
#include "Player.h"
#include "Sounds.h"


Menu menu;
Sounds sound;
RenderWindow window(VideoMode(1920, 1080), "Pacman", Style::Close | Style::Fullscreen);
void Gameplay() {
    Sprite pacmanSprite;

    Player pacman(pacmanSprite);

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
            pacman.updateMovement();
            pacman.updateAnimation();
            pacman.updatePlace(Vector2f(window.getSize().x, window.getSize().y));
            if (ghostShape.getGlobalBounds().intersects(pacmanSprite.getGlobalBounds())) {

                pacman.die();

            }
        }

        window.clear();
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