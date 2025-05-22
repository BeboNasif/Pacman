#include <random>
#include "menu.h"
#include "menu_Bg.h"
#include "Player.h"
#include "Sounds.h"
#include "Map.h"
#include "Ghost.h"
#include "include.h"
#include "FilesController.h"
#include <chrono>
#include <thread>

Map mp;
Menu menu;
Sounds sound;
RenderWindow window(VideoMode(1920, 1080), "Pacman" , Style::Close);

Text nodeNums[95];
extern int character;
bool END = 1;
extern bool pressed;
FilesController filesController;
int winScore;
bool win = false;
float delay = 0;
int tempscore = 0;
void Gameplay(int map_num) {
    END = 1;
	win = false;
	sound.startup();
    sound.bgmusic.pause();
    Texture lives;
    lives.loadFromFile("Assets/Textures/pacman/right_" + to_string(1) + (character == 1 ? "m" : "") + ".png");
    Sprite livesSprite[2];
    for (int i = 0; i < 2; i++)
    {
        livesSprite[i].setTexture(lives);
        livesSprite[i].setPosition(50 * (i + 1) + i * 50, 900);
        livesSprite[i].setScale(3, 3);
    }
    string maxscore = Player::settings["maxScore"];
    int score = 0;
    Sprite pacmanSprite;
    pacmanSprite.setOrigin(11, 11);
    pacmanSprite.setScale(0.6, 0.6);
    Sprite MapSprite;
    Texture mapText;
    Texture GameTexture;

    if (map_num == 1) {
        mapText.loadFromFile("Assets/FullMap.png");
        winScore = 86;
    }
    else {
        mapText.loadFromFile("Assets/MapNormal.png");
        winScore = 51;
    }
    MapSprite.setTexture(mapText);
    MapSprite.setScale(1.65f * (map_num ? 1 : 2), 1.65f * (map_num ? 1: 2 ));
    MapSprite.setOrigin(mapText.getSize().x / 2.f, mapText.getSize().y / 2.f);
    MapSprite.setPosition(1920 / 2.f, 1080 / 2.f);
    Text Score;
    Text MaxScore;
    Text ScoreVal;
    Text MaxScoreVal;
    mp.init();

    vector<int> ghostNodeBegins[2] = { {17,27,28,29},{34,40,41,42} };
    int idx = 0;
    Ghost redGhost(ghostNodeBegins[map_num][idx++], "Assets/red.png", mp.adjList[map_num], mp.pos[map_num]);
    Ghost pinkGhost(ghostNodeBegins[map_num][idx++], "Assets/pink.png", mp.adjList[map_num], mp.pos[map_num]);
    Ghost yellowGhost(ghostNodeBegins[map_num][idx++], "Assets/yellow.png", mp.adjList[map_num], mp.pos[map_num]);
    Ghost blueGhost(ghostNodeBegins[map_num][idx++], "Assets/cyan.png", mp.adjList[map_num], mp.pos[map_num]);

    Texture poisonedTexture;
    poisonedTexture.loadFromFile("Assets/poisoned.png");


    vector<Ghost*> ghosts = { &redGhost, &pinkGhost, &yellowGhost ,&blueGhost};

    Vector2f pos = {0,0};
    pos = {(float) mp.pos[map_num][map_num?65 : 50].first ,(float)mp.pos[map_num][map_num ? 65 : 50].second };
    Player pacman(pacmanSprite,pos);
   
    Player::initSettings(map_num);
  
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

    MaxScore.setFont(fnt);
    MaxScoreVal.setFont(fnt2);
    MaxScore.setScale(2, 2);
    MaxScoreVal.setScale(2, 2);
    MaxScore.setPosition(50, 100);
    MaxScoreVal.setPosition(200, 150);
    MaxScore.setString("Max Score ");
    MaxScoreVal.setString(" " + maxscore);
    for (int i = 1; i <= (map_num?92:55); i++) {
        
        CircleShape c(3);
        c.setFillColor(Color::Yellow);
        auto cur = mp.pos[map_num][i];
        Vector2f v(cur.first, cur.second);
        c.setPosition(v);
        pacman.nodes[i] = c;
    }

    vector<int> PowerUpNodes(4);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis1(0, 14), dis2(0, 19), dis3(0,5);

    vector<int> upperleft, upperright, lowerleft, lowerright;
    if (map_num) {
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
    }
    else {
        upperleft = { 1,2,3,12,13,14 };
        upperright = { 9,10,11,20,21,22 };

        lowerleft = { 34,35,36,45,46,47 };
        lowerright = { 41,42,43,55,53,54 };
        PowerUpNodes[0] = upperleft[dis3(gen)];
        PowerUpNodes[1] = upperright[dis3(gen)];
        PowerUpNodes[2] = lowerright[dis3(gen)];
        PowerUpNodes[3] = lowerleft[dis3(gen)];

    }

   
    for (auto it : PowerUpNodes) {
        pacman.nodes[it].setRadius(15);
        pacman.nodes[it].setFillColor({ 255,165,0 });
        pacman.nodes[it].setOrigin(15, 15);
    }
  
    Ghost::allPaths = Ghost::precomputeAllPaths(mp.adjList[map_num],map_num);

    Clock clock, clock2;
    srand(time(NULL));

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        if (sound.so.getStatus() != Sound::Playing)
        {
            if(sound.so2.getStatus() != Sound::Playing && sound.so6.getStatus() != Sound::Playing)
				sound.chasing(0);
        }
        Event event;
        int maxScore = stoi(Player::settings["maxScore"]);
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyReleased)
                pressed = false;
        }
        if(!map_num)
			tempscore = score;
        if (map_num && (score == 0 || score == 20)) {
			score = tempscore;
        }

        if (clock2.getElapsedTime().asSeconds() >= 1 && pacman.curr_state != 0) {
            Ghost::timer++;
            clock2.restart();
        }
        if (!winScore && !map_num && delay>=0.09) {
            win = true;
            delay = 0;
        }

        if (!pacman.gameOver) {
            pacman.setDeltaTime(deltaTime);
            pacman.handleInput(mp.adjList[map_num], mp.pos[map_num],map_num);
            pacman.updateMovement(mp.adjList[map_num], mp.pos[map_num],map_num);
            pacman.updateAnimation(ghosts, mp, map_num);
            if(map_num)
                pacman.updatePlace(Vector2f(window.getSize().x, window.getSize().y));


        }
        Vector2f pacmanPosition = pacmanSprite.getPosition();
        bool allAreNotPoisened = true;
        int pacmanNode = pacman.getCurrentNode(mp.pos[map_num], pacmanPosition, map_num);
        for (int i = 0; i < 4;i++) {

            if ((!pacman.gameOver  && pacman.curr_state) && ghosts[i]->shouldUpdate(i)) {
                int target = 0;
                if (i == 0 && map_num == 0) target = ghosts[i]->Blinky(pacmanNode);
                if (i == 0 && map_num == 1) {target = ghosts[i]->HardMove(pacmanNode, pacman.nodes, mp.adjList[map_num], 0);}
                if (i == 1 && map_num == 0) target = ghosts[i]->Pinky(pacmanNode, pacman.curr_state, mp.adjList[map_num],ghosts[0]->currentNode);
                if (i == 1 && map_num == 1) {target = ghosts[i]->HardMove(pacmanNode, pacman.nodes, mp.adjList[map_num], 10);}
                if (i == 2 && map_num == 0) target = ghosts[i]->Clyde(pacmanNode,map_num);
                if (i == 2 && map_num == 1) {target = ghosts[i]->HardMove(pacmanNode, pacman.nodes, mp.adjList[map_num], -10);}
                if (i == 3 && map_num == 0) target = ghosts[i]->Inky(pacmanNode, pacman.curr_state, ghosts[0]->getCurrentNode(), mp.adjList[map_num]);
                if (i == 3 && map_num == 1) {target = ghosts[i]->HardMove(pacmanNode, pacman.nodes, mp.adjList[map_num], 2);}

                ghosts[i]->ghostOut = 1;

                if (ghosts[i]->isDead)
                {
                    target = (map_num?40:27) + (rand() % 3);
					
                }
                else if (ghosts[i]->isPoisoned) {
                    target = 1 + rand() % (map_num ? 90 : 55);
                }
                ghosts[i]->update(deltaTime, mp.pos[map_num], target);
                if (ghosts[i]->isPoisoned)
                    allAreNotPoisened = false;
            }
        }

        if (allAreNotPoisened)
            pacman.walk_speed = 150;
        else 
            pacman.walk_speed = 250;

        for (auto& ghost : ghosts) {
            if ( pacmanSprite.getGlobalBounds().intersects(ghost->getSprite().getGlobalBounds())) {
                if (ghost->isPoisoned) {
					sound.eat_ghost();
                    if(!ghost->isDead) ghost->die("Assets/DEAD2.png",score);
                }
                else {
					sound.so2.stop();
				    sound.dying();
                    pacman.die();
                    int prevMaxScore = stoi(pacman.settings["maxScore"]);
                    if (score > prevMaxScore) {
                        pacman.settings["maxScore"] = to_string(score);
                    }
                    FilesController::updatePlayerSettings(Player::playerName, Player::settings);
                    Ghost::timer = 0;
                }
            }
        }

        vector<int> skips = ghostNodeBegins[map_num];
        for (int i = 1; i <= (map_num ? 90 : 55); i++) {
            for (auto x : skips)
                if (i == x) i++;


            if ( pacmanSprite.getGlobalBounds().intersects(pacman.nodes[i].getGlobalBounds())) {
                if (i == PowerUpNodes[0] or i == PowerUpNodes[1] or i == PowerUpNodes[2] or i == PowerUpNodes[3]) {
                    for (auto& ghost : ghosts) {
                        if (ghost->ghostOut == 1)
                            ghost->poisoned("Assets/poisoned.png");
                    }
                    sound.chasing(1);
                    pacman.walk_speed = 250;
                }

                sound.eating();
                pacman.nodes[i].setScale(0, 0);
                score += 20;
                winScore--;
            }
        }
        if(!winScore)
			delay += deltaTime;
        Score.setString("Score");
        ScoreVal.setString(to_string(score));
        window.clear();
        window.draw(MapSprite);
        if (!win)
        {
            for (int i = 1; i <= (map_num ? 90 : 55); i++) {
                for (auto x : skips) {

                    if (i == x) i++;
                }
                    window.draw(pacman.nodes[i]);
            }
        }
        
        for (auto& ghost : ghosts) {
            ghost->draw(window);
        }
        window.draw(pacmanSprite);
        window.draw(Score);
        window.draw(ScoreVal);

        window.draw(MaxScore);
        window.draw(MaxScoreVal);

        for (int i = 0; i < pacman.live; i++)
        {
			window.draw(livesSprite[i]);
        }
        if (!winScore && map_num && delay>=0.16) {
			pacman.gameOver = true;
            delay = 0;
        }
        if (pacman.gameOver)
        {
            filesController.Gameover(score, maxScore);
            window.draw(filesController.overlay);
            if (!winScore)
                window.draw(filesController.winnersp);
            else
                if (score >= maxScore)
                    window.draw(filesController.highscoreENDsp);
                else
                    window.draw(filesController.gameoversp);
            window.draw(filesController.scoreText1);
            window.draw(filesController.scoreText2);
        }
        window.display();
        if ((event.key.code == Keyboard::Escape && !pressed) || win)
        {
            if (score > maxScore) {
                Player::settings["maxScore"] = to_string(score);
            }
            sound.so2.stop();
            pressed = true;
            if (pacman.gameOver)
                END = 0;
            GameTexture.create(1920, 1080);
            GameTexture.update(window);
            menu.Pause(window, GameTexture);
            if (menu.exit)
            {
                menu.exit = 0;
                sound.music(0);
                int i = 0;
                vector<int> starts = ghostNodeBegins[map_num];
                for (auto& ghost : ghosts) {

                    ghost->reset(starts[i], mp.pos[map_num]);
                    i++;
                }
                if (menu.play_again)
                    Gameplay(map_num);
                return;
            }


        }
    }

    // Update the score in player's settings if it's higher than previous max
    if (score > stoi(pacman.settings["maxScore"])) {
        pacman.settings["maxScore"] = to_string(score);
    }
    // Save settings when gameplay ends
    FilesController::updatePlayerSettings(Player::playerName, Player::settings);
}

int main() {
   
    window.setFramerateLimit(120);
    // Show player name input menu first if it's the first time
    if (menu.show_name_input) {
        menu.player_name_input(window);
    }
    // Ensure settings file exists
   
    try {
        ifstream settingsFile("Assets/Settings.txt");
        if (!settingsFile) {
            ofstream createSettings("Assets/Settings.txt");
            createSettings.close();
        }
    } catch (...) {
        ofstream createSettings("Assets/Settings.txt");
        createSettings.close();
    }
   
    menu.menu1(window);
    
    
    return 0;
}