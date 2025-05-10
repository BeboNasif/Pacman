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
RenderWindow window(VideoMode(1920, 1080), "Pacman");
CircleShape a7a;

void Gameplay() {
    Sprite pacmanSprite;
    pacmanSprite.setOrigin(11, 11);  

    Sprite MapSprite;
    Texture mapText;
    mapText.loadFromFile("Assets/FullMap.png");
    MapSprite.setTexture(mapText);
    MapSprite.setScale(1.65f, 1.65f);
    MapSprite.setOrigin(mapText.getSize().x / 2.f, mapText.getSize().y / 2.f);
    MapSprite.setPosition(1920 / 2.f, 1080 / 2.f);

    Player pacman(pacmanSprite);

    mp.init();
    auto allPaths = Ghost::precomputeAllPaths(mp.adjList);

    for (int i = 1; i <= 64; i++) {
        CircleShape c(1);
        auto cur = mp.pos[i];
        Vector2f v(cur.first, cur.second);
        c.setPosition(v);
        pacman.nodes[i] = c;
    }

    Texture ghostTexture;
    ghostTexture.loadFromFile("Assets/ghost.png");
    Sprite ghostSprite;
    ghostSprite.setTexture(ghostTexture);
    ghostSprite.setPosition(500.f, 500.f);  
    ghostSprite.setOrigin(ghostTexture.getSize().x / 2.f, ghostTexture.getSize().y / 2.f);
    ghostSprite.setScale(2.8f, 2.8f);

    Clock clock;

    srand(time(NULL));

    float ghostSpeed = 100.f;

    int currentNode = 1;
    int pathIndex = 0;
    std::vector<int> path;
    Vector2f ghostPosition = pacman.nodes[currentNode].getPosition();
    ghostSprite.setPosition(ghostPosition);

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.key.code == Keyboard::Escape)
                return;
        }

        // If the game is not over, update Pacman
        if (!pacman.gameOver) {
            pacman.setDeltaTime(deltaTime);
            pacman.handleInput(mp.adjList, mp.pos);
            pacman.updateMovement(mp.pos);
            pacman.updateAnimation();
            pacman.updatePlace(Vector2f(window.getSize().x, window.getSize().y));

            if (ghostSprite.getGlobalBounds().intersects(pacmanSprite.getGlobalBounds())) {
                pacman.die();  
            }
        }

        Vector2f pacmanPosition = pacmanSprite.getPosition();
        int pacmanNode = pacman.getCurrentNode(mp.pos, pacmanPosition);

        int ghostNode = pacman.getCurrentNode(mp.pos, ghostSprite.getPosition());

        // If target changed, update path
        if (ghostNode != pacmanNode) {
            path = allPaths[ghostNode][pacmanNode];
            pathIndex = 0;
            currentNode = ghostNode;

            std::cout << "New path to Pacman: ";
            for (int node : path) {
                std::cout << node << " ";
            }
            std::cout << std::endl;
        }

        if (!path.empty() && pathIndex < path.size() - 1) {
            int currentPathNode = path[pathIndex];
            int nextNode = path[pathIndex + 1];

            Vector2f currentPos = pacman.nodes[currentPathNode].getPosition();
            Vector2f nextPos = pacman.nodes[nextNode].getPosition();

            Vector2f direction = nextPos - currentPos;
            float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= distance;

            ghostSprite.move(direction * ghostSpeed * deltaTime);

            if (abs(ghostSprite.getPosition().x - nextPos.x) < 5.f &&
                abs(ghostSprite.getPosition().y - nextPos.y) < 5.f) {
                pathIndex++;
                std::cout << "Ghost moved to node: " << nextNode << std::endl;
            }
        }


        window.clear();
        window.draw(MapSprite);
        for (int i = 1; i <= 64; i++) {
            window.draw(pacman.nodes[i]);
        }
        window.draw(pacmanSprite);
        window.draw(ghostSprite);
        window.display();
    }
}


int main() {
    window.setFramerateLimit(120);
    menu.menu1(window); 
    Gameplay();
}


