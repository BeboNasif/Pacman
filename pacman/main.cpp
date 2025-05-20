#include <random>
#include "Menu.h"
#include "menu_Bg.h"
#include "Player.h"
#include "Sounds.h"
#include "Map.h"
#include "Ghost.h"
#include "include.h"
Map mp;
Menu menu;
Sounds sound;
RenderWindow window(VideoMode(1920, 1080), "Pacman", Style::Close);

Text nodeNums[95];
float timer = 0;

void Gameplay() {
    int score = 0;
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
    Text Score;
    Text ScoreVal;
    mp.init();
    //mp.printAdjList();

    Ghost redGhost(34, "Assets/red.png", mp.adjList, mp.pos);
    Ghost pinkGhost(40, "Assets/pink.png", mp.adjList, mp.pos);
    Ghost blueGhost(41, "Assets/cyan.png", mp.adjList, mp.pos);
    Ghost yellowGhost(42, "Assets/yellow.png", mp.adjList, mp.pos);
    sf::Texture poisonedTexture;
    poisonedTexture.loadFromFile("Assets/poisoned.png");


    vector<Ghost*> ghosts = { &redGhost, &pinkGhost, &blueGhost, &yellowGhost };

    Player pacman(pacmanSprite);

    Font fnt, fnt2;
    fnt.loadFromFile("Assets/Fonts/Freedom-10eM.ttf");
    fnt2.loadFromFile("Assets/Fonts/Carre-JWja.ttf");
    Score.setFont(fnt);
    ScoreVal.setFont(fnt2);
    Score.setScale(2, 2);
    ScoreVal.setScale(2, 2);
    Score.setPosition(50, 0);
    ScoreVal.setPosition(300, 0);
    Score.setString("Score ");
    ScoreVal.setString(": " + to_string(score));

    for (int i = 1; i <= 92; i++) {
        CircleShape c(3);
        c.setFillColor(Color::Yellow);
        auto cur = mp.pos[i];
        Vector2f v(cur.first, cur.second);
        /*Text node;
        node.setFont(fnt2);
        node.setPosition(v);
        node.setString(to_string(i));
        node.setCharacterSize(30);
        node.setFillColor(Color::Blue);
        nodeNums[i] = node;*/
        c.setPosition(v);
        pacman.nodes[i] = c;
    }

    vector<int> PowerUpNodes(4);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis1(0, 14), dis2(0, 19);

    vector<int> upperleft, upperright, lowerleft, lowerright;
    for (int i = 0; i <= 2; i++)
        for (int j = 1; j <= 5; j++)
            upperleft.push_back(10 * i + j), upperright.push_back(10 * i + j + 5);

    for (int i = 5; i <= 8; i++)
        for (int j = 1; j <= 5; j++)
            lowerleft.push_back(10 * i + j), lowerright.push_back(10 * i + j + 5);

    PowerUpNodes[0] = upperleft[dis1(gen)];
    PowerUpNodes[1] = upperright[dis1(gen)];
    PowerUpNodes[2] = lowerright[dis2(gen)];
    PowerUpNodes[3] = lowerleft[dis2(gen)];

    for (auto it : PowerUpNodes) {
        pacman.nodes[it].setRadius(15);
        pacman.nodes[it].setFillColor({ 255,165,0 });
        pacman.nodes[it].setOrigin(15, 15);
    }

    Ghost::allPaths = Ghost::precomputeAllPaths(mp.adjList);
    Clock clock, clock2;
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

        if (clock2.getElapsedTime().asSeconds() >= 1 && pacman.curr_state != 0) {
            timer++;
            clock2.restart();
        }


        if (!pacman.gameOver) {
            pacman.setDeltaTime(deltaTime);
            pacman.handleInput(mp.adjList, mp.pos);
            pacman.updateMovement(mp.adjList, mp.pos);
            pacman.updateAnimation(ghosts, mp);
            pacman.updatePlace(Vector2f(window.getSize().x, window.getSize().y));
        }

        Vector2f pacmanPosition = pacmanSprite.getPosition();
        bool allAreNotPoisened = true;
        int pacmanNode = pacman.getCurrentNode(mp.pos, pacmanPosition);
        for (int i = 0; i < 4;i++) {

            if ((!pacman.gameOver && !pacman.isDead && pacman.curr_state) && ghosts[i]->shouldUpdate(i, timer)) {
                int target = 0;
                if (i == 0) target = ghosts[i]->Ad3k(pacmanNode);
                if (i == 1) target = ghosts[i]->EL7okooma(pacmanNode, pacman.curr_state, mp.adjList);
                if (i == 2) target = ghosts[i]->Amoor(pacmanNode);
                if (i == 3) target = ghosts[i]->ELSaad(pacmanNode, pacman.curr_state, ghosts[0]->getCurrentNode(), mp.adjList);

                ghosts[i]->ghostOut = 1;

                if (ghosts[i]->isDead)
                {
                    target = 40 + (rand() % 3);
					
                }
                else if (ghosts[i]->isPoisoned) {
                    target = 1 + rand() % 90;
					//cout << "target is: " << target << endl;
                }
                ghosts[i]->update(deltaTime, mp.pos, target);
                if (ghosts[i]->isPoisoned)
                    allAreNotPoisened = false;

            }
        }
        if (allAreNotPoisened)
            pacman.walk_speed = 150;
        else 
            pacman.walk_speed = 250;

        for (auto& ghost : ghosts) {
            if (!pacman.isDead && pacmanSprite.getGlobalBounds().intersects(ghost->getSprite().getGlobalBounds())) {
                if (ghost->isPoisoned) {
                    if(!ghost->isDead) ghost->die("Assets/DEAD2.png",score);
                }
                else {
                    pacman.die();
                    timer = 0;
                }
            }
        }

        for (int i = 1; i <= 90; i++) {
            if (i == 34 or i == 40 or i == 41 or i == 42) continue;

            if (!pacman.isDead && pacmanSprite.getGlobalBounds().intersects(pacman.nodes[i].getGlobalBounds())) {
                if (i == PowerUpNodes[0] or i == PowerUpNodes[1] or i == PowerUpNodes[2] or i == PowerUpNodes[3]) {
                    for (auto& ghost : ghosts) {
                        if (ghost->ghostOut == 1)
                            ghost->poisoned("Assets/poisoned.png");
                    }

                    pacman.walk_speed = 250;
                    cout << "speed : " << pacman.walk_speed;
                }
                pacman.nodes[i].setScale(0, 0);
                score += 20;
            }
        }
        Score.setString("Score");
        ScoreVal.setString(to_string(score));
        window.clear();
        window.draw(MapSprite);

        for (int i = 1; i <= 90; i++) {
            if (i == 34 or i == 40 or i == 41 or i == 42) continue;
            window.draw(pacman.nodes[i]);
            window.draw(nodeNums[i]);
        }


        for (auto& ghost : ghosts) {
            ghost->draw(window);
        }
        window.draw(pacmanSprite);
        window.draw(Score);
        window.draw(ScoreVal);

        window.display();
    }
}




int main() {
    window.setFramerateLimit(120);
    menu.menu1(window);
    Gameplay();
}