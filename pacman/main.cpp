#include "include.h"
#include "Menu.h"
#include "menu_Bg.h"
#include "Player.h"
#include "Sounds.h"
#include "Map.h"
#include "Ghost.h"

Map mp;
Menu menu;
Sounds sound;
RenderWindow window(VideoMode(1920, 1080), "Pacman", Style::Close);

Text nodeNums[95];

void Gameplay() {
    Sprite pacmanSprite;
    pacmanSprite.setOrigin(11, 11);
    pacmanSprite.setScale(0.6, 0.6);
    Sprite MapSprite;
    Texture mapText;
    mapText.loadFromFile("Assets/FullMap.png");
    MapSprite.setTexture(mapText);
    MapSprite.setScale(1.65f, 1.65f);
    MapSprite.setOrigin(mapText.getSize().x / 2.f, mapText.getSize().y / 2.f);
    MapSprite.setPosition(1920 / 2.f, 1080 / 2.f);

    mp.init();
    mp.printAdjList();

    Ghost redGhost(34, "Assets/red_ghost.png", mp.adjList, mp.pos);
    Ghost pinkGhost(40, "Assets/pink_ghost.png", mp.adjList, mp.pos);
    Ghost blueGhost(41, "Assets/blue_ghost.png", mp.adjList, mp.pos);
    Ghost yellowGhost(42, "Assets/yellow_ghost.png", mp.adjList, mp.pos);

    std::vector<Ghost*> ghosts = { &redGhost, &pinkGhost, &blueGhost, &yellowGhost }; 

    Player pacman(pacmanSprite);

    Font fnt;
    fnt.loadFromFile("Assets/Fonts/Komigo3D-Regular.ttf");
    
    for (int i = 1; i <= 92; i++) {
        CircleShape c(1);
        auto cur = mp.pos[i];
        Vector2f v(cur.first, cur.second);
    
        Text node;
        node.setFont(fnt);
        node.setPosition(v);
        node.setString(to_string(i));
        node.setCharacterSize(30);
        node.setFillColor(Color::Blue);
        nodeNums[i] = node;
        c.setPosition(v);
        pacman.nodes[i] = c;
    }

    Clock clock;
    srand(time(NULL));

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
            pacman.handleInput(mp.adjList, mp.pos);
            pacman.updateMovement(mp.adjList,mp.pos);
            pacman.updateAnimation();
            pacman.updatePlace(Vector2f(window.getSize().x, window.getSize().y));
        }

        Vector2f pacmanPosition = pacmanSprite.getPosition();
        int pacmanNode = pacman.getCurrentNode(mp.pos, pacmanPosition);

        for (auto& ghost : ghosts) {
            if (!pacman.gameOver or !pacman.isDead) {
                ghost->update(deltaTime, mp.adjList, mp.pos, pacmanNode);
            }
        }

        for (auto& ghost : ghosts) {
            if (!pacman.isDead && pacmanSprite.getGlobalBounds().intersects(ghost->getSprite().getGlobalBounds())) {
                pacman.die();
                int i = 0;
                vector<int> starts = { 34,40,41,42 };
                for (auto& ghost : ghosts) {
                    ghost->reset(starts[i], mp.pos);
                    i++;
                }
            }
        }


        window.clear();
        window.draw(MapSprite);

        //for (int i = 1; i <= 92; i++) {
          /*  window.draw(pacman.nodes[i]);
            window.draw(nodeNums[i]);*/
        //}

        window.draw(pacmanSprite);

        for (auto& ghost : ghosts) {
            ghost->draw(window);
        }
        window.display();
    }
}




int main() {
    window.setFramerateLimit(120);
    menu.menu1(window); 
    Gameplay();
}


