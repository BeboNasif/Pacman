#include "menu.h"
#include "KeyboardKEYS.h"
#include "menu_Bg.h"
#include "Sounds.h"
#include "Player.h"
#include "FilesController.h"
using namespace sf;

menu_Bg menu_UI;
bool pressed = false;
bool esc_button = false;
extern Sounds sound;
int character = 0;
extern bool END;
bool waitingForKey = false;
extern bool win;
int actionToChange;
void Gameplay(int map_num);
Menu::Menu()
    : choises(0)
    , selected(0)
    , downFaceSpeed(100.f)
    , currentFrame(0)
    , eatenPills(0)
    , delayStarted(false)
    , positionOfFace(60.f)
    , frameDuration(seconds(0.1f))
{
    mainmenu.reserve(10);
    faceFrames.reserve(3);
    pills.reserve(3);

    pillConsumed.assign(3, false);
    pillDelayStarted.assign(3, false);
    pillTimers.assign(3, Clock{});

}
void Menu::reset() {
    choises = 0;
    selected = 0;
    downFaceSpeed = 100.f;
    currentFrame = 0;
    eatenPills = 0;
    delayStarted = false;
    positionOfFace = 60.f;
    frameDuration = sf::seconds(0.1f);

    mainmenu.reserve(10);
    faceFrames.reserve(3);
    pills.reserve(3);

    pillConsumed.assign(3, false);
    pillDelayStarted.assign(3, false);
    pillTimers.assign(3, sf::Clock{});
}

void Menu::set_sounds() {
    sound.so.setVolume(stoi(Player::settings["sound"]));
    sound.so2.setVolume(stoi(Player::settings["sound"]));
    sound.so4.setVolume(stoi(Player::settings["sound"]));
    sound.so5.setVolume(stoi(Player::settings["sound"]));
    sound.so6.setVolume(stoi(Player::settings["sound"]));
    sound.bgmusic.setVolume(stoi(Player::settings["music"]));
}

void Menu::player_name_input(RenderWindow& window)
{
    Menu nameMenu;
    nameMenu.Face_intilization();
    nameMenu.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");

    Text headerText;
    headerText.setFont(nameMenu.font);
    headerText.setCharacterSize(60);
    headerText.setString("ENTER YOUR NAME");
    headerText.setPosition(680, 300);
    headerText.setFillColor(Color{ 255, 204, 0 });

    Text inputText;
    inputText.setFont(nameMenu.font);
    inputText.setCharacterSize(50);
    inputText.setPosition(820, 400);
    inputText.setFillColor(Color::White);

    Text instructionText;
    instructionText.setFont(nameMenu.font);
    instructionText.setCharacterSize(30);
    instructionText.setString("Press Enter to confirm, Escape to cancel");
    instructionText.setPosition(700, 500);
    instructionText.setFillColor(Color{ 180, 180, 180 });

    std::string playerName = "";
    bool blinkCursor = true;
    Clock blinkClock;

    nameMenu.deltaClock.restart();
 
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    // Cancel and return to previous menu
                    return;
                }
                else if (event.key.code == Keyboard::Enter && !playerName.empty())
                {
                    // Set the player name in the Player class
                    Player::playerName = playerName;
                    // sound.playMenuSelect();
                    Player::initSettings(1);
                    return;
                }
                else if (event.key.code == Keyboard::BackSpace && !playerName.empty())
                {
                    playerName.pop_back();
     
                    // sound.playMenuMove();
                }
            }
            else if (event.type == Event::TextEntered)
            {
                // Accept only letters, numbers and underscore, limit to 15 characters
                if (event.text.unicode >= 32 && event.text.unicode < 127 && playerName.length() < 15)
                {
                    char character = static_cast<char>(event.text.unicode);
                    if (isalnum(character) || character == '_')
                    {
                        playerName += character;
                        // sound.playMenuMove();
                    }
                }
            }
        }

        // Handle cursor blinking
        if (blinkClock.getElapsedTime().asSeconds() > 0.5f)
        {
            blinkCursor = !blinkCursor;
            blinkClock.restart();
        }

        inputText.setString(playerName + (blinkCursor ? "_" : ""));

        // Center the input text
        float textWidth = inputText.getLocalBounds().width;
        inputText.setPosition((window.getSize().x - textWidth) / 2.0f, 400);

        float dt = nameMenu.deltaClock.restart().asSeconds();
        nameMenu.updateFaces(dt);

        window.clear();
        menu_UI.back_ground(window);
        window.draw(menu_UI.bg);
        window.draw(headerText);
        window.draw(inputText);
        window.draw(instructionText);

        for (auto& p : nameMenu.pills)
            window.draw(p);
        window.draw(nameMenu.DownFace);

        window.display();
    }

}

void Menu::changeKeyMapping(int& action, Keyboard::Key newKey, Menu& menu9) {
    if (KeyboardKEYS::keyboardKeyToString(newKey) == "Escape")
        return;
    if (action == 0) {
        Player::settings["moveLeftKey"] = KeyboardKEYS::keyboardKeyToString(newKey);
        FilesController::updatePlayerSettings(Player::playerName, Player::settings);
    }
    else if (action == 1) {
        Player::settings["moveRightKey"] = KeyboardKEYS::keyboardKeyToString(newKey);
        FilesController::updatePlayerSettings(Player::playerName, Player::settings);
    }
    else if (action == 2) {
        Player::settings["moveUpkey"] = KeyboardKEYS::keyboardKeyToString(newKey);
        FilesController::updatePlayerSettings(Player::playerName, Player::settings);
    }
    else if (action == 3) {
        Player::settings["moveDownkey"] = KeyboardKEYS::keyboardKeyToString(newKey);
        FilesController::updatePlayerSettings(Player::playerName, Player::settings);
    }
}

void Menu::updateFaces(float dt)
{

    DownFace.move(downFaceSpeed * dt, 0);

    for (int i = 0; i < 3; ++i) {
        if (!pillConsumed[i] && !pillDelayStarted[i]
            && DownFace.getGlobalBounds().intersects(pills[i].getGlobalBounds()))
        {
            pillDelayStarted[i] = true;
            pillTimers[i].restart();
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (pillDelayStarted[i]
            && pillTimers[i].getElapsedTime().asSeconds() >= 0.5f)
        {
            pillConsumed[i] = true;
            eatenPills++;
            pills[i].setPosition(-100.f, -100.f);
            pillDelayStarted[i] = false;
        }
    }

    if (eatenPills >= 3 && !delayStarted) {
        delayStarted = true;
        resetDelayClock.restart();
    }
    if (delayStarted
        && resetDelayClock.getElapsedTime().asSeconds() >= 0.4f)
    {
        eatenPills = 0;
        static const Vector2f pos[3] = {
            {400.f,1000.f}, {450.f,1000.f}, {500.f,1000.f}
        };
        for (int i = 0; i < 3; ++i) {
            pillConsumed[i] = false;
            pillDelayStarted[i] = false;
            pills[i].setPosition(pos[i]);
        }
        DownFace.setPosition(400.f, 1000.f);
        delayStarted = false;
    }

    if (animationClock.getElapsedTime() > frameDuration) {
        currentFrame = (currentFrame + 1) % faceFrames.size();
        Face.setTexture(&faceFrames[currentFrame]);
        DownFace.setTexture(&faceFrames[currentFrame]);
        animationClock.restart();
    }
    if (DownFace.getPosition().x > 600)
        DownFace.setPosition(400.f, 1000.f);
}

void Menu::Face_intilization()
{
    faceFrames.resize(3);
    pills.resize(3);
    pillConsumed.assign(3, false);
    pillDelayStarted.assign(3, false);
    pillTimers.assign(3, Clock{});

    faceFrames[0].loadFromFile("Assets/Textures/pacman/0.png");
    faceFrames[1].loadFromFile("Assets/Textures/pacman/1.png");
    faceFrames[2].loadFromFile("Assets/Textures/pacman/2.png");

    Face.setTexture(&faceFrames[0]);
    Face.setSize({ 60,60 });
    Face.setPosition(800, 582);
    Face.setScale(-1, 1);

    DownFace.setTexture(&faceFrames[0]);
    DownFace.setSize({ 50,50 });
    DownFace.setPosition(400, 1000);
    DownFace.setScale(-1, 1);

    currentFrame = 0;
    animationClock.restart();
    frameDuration = milliseconds(300);

    pill.loadFromFile("Assets/Textures/pacman/pill.png");
    static const Vector2f pos[3] = {
        {400.f,1000.f}, {450.f,1000.f}, {500.f,1000.f}
    };
    for (int i = 0; i < 3; ++i) {
        pills[i].setTexture(&pill);
        pills[i].setSize({ 50,50 });
        pills[i].setPosition(pos[i]);
    }
}

void Menu::MoveDown(int& selected, int choises)
{
    if (selected < choises)
    {
        mainmenu[selected].setFillColor(Color::White);
        Face.setPosition(Face.getPosition().x, Face.getPosition().y + positionOfFace);
        selected++;
        if (selected == choises)
        {
            selected = 0;
            Face.setPosition(Face.getPosition().x, Face.getPosition().y - (choises * positionOfFace));
        }
        mainmenu[selected].setFillColor(Color{ 255,204,0 });
    }
}

void Menu::MoveUp(int& selected, int choises)
{
    if (selected > -1)
    {
        mainmenu[selected].setFillColor(Color::White);
        Face.setPosition(Face.getPosition().x, Face.getPosition().y - positionOfFace);
        selected--;
        if (selected == -1)
        {
            selected = choises - 1;
            Face.setPosition(Face.getPosition().x, Face.getPosition().y + (positionOfFace * choises));
        }
        mainmenu[selected].setFillColor(Color{ 255,204,0 });
    }
}

void Menu::menu1(RenderWindow& window)
{
	reset();
    set_sounds();
    font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    sound.music(0);
    choises = 7;
    selected = 0;
    mainmenu.resize(choises);

    static const string labels[7] = {
        "Play Game","Instructions","Change Profile",
        "High Score","Options","Credits","Exit"
    };

    float yOff = 40.f;
    float midY = window.getSize().y * 0.5f;
    for (int i = 0; i < choises; ++i) {
        mainmenu[i].setFont(font);
        mainmenu[i].setCharacterSize(50);
        mainmenu[i].setString(labels[i]);
        mainmenu[i].setPosition(820, midY + yOff);
        mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOff += positionOfFace;
    }

    Face_intilization();
    deltaClock.restart();

    while (window.isOpen()) {
        Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == Event::Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }
            if (evt.type == Event::KeyReleased)
                pressed = false;
            if (evt.type == Event::KeyPressed && !pressed) {
                pressed = true;
                if (evt.key.code == Keyboard::Down)
                    MoveDown(selected, choises);
                if (evt.key.code == Keyboard::Up)
                    MoveUp(selected, choises);

                if (evt.key.code == Keyboard::Enter
                    || (evt.key.code == Keyboard::Escape && selected == 6))
                {

                    if (selected == 0)
                        Play_menu(window);
                    if (selected == 2)
                    {
                        if (show_name_input) {
                            player_name_input(window);
                        }
                        // Ensure settings file exists

                        try {
                            ifstream settingsFile("Assets/Settings.txt");
                            if (!settingsFile) {
                                ofstream createSettings("Assets/Settings.txt");
                                createSettings.close();
                            }
                        }
                        catch (...) {
                            ofstream createSettings("Assets/Settings.txt");
                            createSettings.close();
                        }
						set_sounds();
                    }
                    if (selected == 1)
                        instructions(window);
                    if (selected == 3)
                        high_scores(window);
                    if (selected == 4)
                        options_menu(window);
                    if (selected == 5)
                        credits(window);
                    if (selected == 6) {
                        window.close();
                        break;
                    }
                }
                if (evt.key.code == Keyboard::Escape) {
                    mainmenu[selected].setFillColor(Color::White);
                    selected = 6;
                    mainmenu[6].setFillColor(Color{ 255,204,0 });
                    Face.setPosition(Face.getPosition().x, midY + 7 * positionOfFace - 15);
                }
            }
        }

        float dt = deltaClock.restart().asSeconds();
        updateFaces(dt);

        window.clear();
        menu_UI.back_ground(window);
        window.draw(menu_UI.bg);
        for (auto& t : mainmenu)  window.draw(t);
        window.draw(Face);
        for (auto& p : pills)     window.draw(p);
        window.draw(DownFace);
        window.display();
    }
}

void Menu::high_scores(RenderWindow& window) {
    vector<string> data = FilesController::readFile("Assets/Settings.txt");

    // Parse into vector of pairs: (name, score as int)
    vector<pair<string, int>> players;

    for (const auto& line : data) {
        if (line.empty()) continue;

        size_t colon = line.find(':');
        if (colon == string::npos) continue;

        string name = line.substr(0, colon);
        size_t scorePos = line.find("maxScore=", colon);
        if (scorePos == string::npos) continue;

        scorePos += 9;
        size_t comma = line.find(',', scorePos);
        string scoreStr = line.substr(scorePos, comma - scorePos);

        int score = 0;
        try {
            score = stoi(scoreStr);
        }
        catch (...) {
            score = 0; // If conversion fails, default to 0
        }

        players.emplace_back(name, score);
    }

    // Sort descending by score
    sort(players.begin(), players.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
        });

    // Prepare SFML texts for top 5 only
    vector<pair<sf::Text, sf::Text>> scores;

    float yOffset = 600.0f; // Starting Y position
    int limit = min(5, (int)players.size());

    for (int i = 0; i < limit; ++i) {
        sf::Text nameText, scoreText;
        nameText.setFont(font);
        scoreText.setFont(font);

        nameText.setString(players[i].first);
        scoreText.setString(to_string(players[i].second));

        nameText.setCharacterSize(60);
        scoreText.setCharacterSize(60);

        nameText.setFillColor(sf::Color::White);
        scoreText.setFillColor(sf::Color::Yellow);

        nameText.setPosition(700.0f, yOffset);
        scoreText.setPosition(1100.0f, yOffset);

        yOffset += 80.0f;

        scores.emplace_back(nameText, scoreText);
    }

    // Main render loop (same as before)
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            pressed = true;
            return;
        }

        float dt = deltaClock.restart().asSeconds();
        updateFaces(dt);

        window.clear();
        menu_UI.back_ground(window);
        window.draw(menu_UI.bg);
        for (auto& p : pills) window.draw(p);
        window.draw(DownFace);

        for (const auto& [nameText, scoreText] : scores) {
            window.draw(nameText);
            window.draw(scoreText);
        }

        window.display();
    }
}


void Menu::Play_menu(RenderWindow& window)
{
    Menu menu2;
    menu2.choises = 3;
    menu2.selected = 0;
    menu2.mainmenu.resize(3);

    menu2.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    menu2.Face_intilization();

    static const std::string labels[3] = { "Normal","Hard","Back" };
    float yOff = 40.f;
    float midY = window.getSize().y * 0.5f;
    for (int i = 0; i < menu2.choises; ++i) {
        menu2.mainmenu[i].setFont(menu2.font);
        menu2.mainmenu[i].setCharacterSize(50);
        menu2.mainmenu[i].setString(labels[i]);
        menu2.mainmenu[i].setPosition(820, midY + yOff);
        menu2.mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOff += menu2.positionOfFace;
    }

    menu2.deltaClock.restart();

    while (window.isOpen()) {
        Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == Event::Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }
            if (evt.type == Event::KeyReleased)
                pressed = false;
            if (evt.type == Event::KeyPressed && !pressed) {
                pressed = true;

                if ((evt.key.code == Keyboard::Enter && menu2.selected == 2) || (evt.key.code == Keyboard::Escape && menu2.selected == 2))
                {
                    return;
                }
                if (evt.key.code == Keyboard::Escape) {
                    menu2.mainmenu[menu2.selected].setFillColor(Color::White);
                    menu2.selected = 2;
                    menu2.mainmenu[2].setFillColor(Color{ 255,204,0 });
                    menu2.Face.setPosition(menu2.Face.getPosition().x, midY + 3 * positionOfFace - 15);
                }
                if (evt.key.code == Keyboard::Down)
                    menu2.MoveDown(menu2.selected, 3);
                if (evt.key.code == Keyboard::Up)
                    menu2.MoveUp(menu2.selected, 3);

                if (evt.key.code == Keyboard::Enter) {
                    if (menu2.selected == 0) Gameplay(0);
                    else if (menu2.selected == 1) Gameplay(1);
                    if (!play_again)
                        sound.bgmusic.play();
                }
            }
        }

        float dt = menu2.deltaClock.restart().asSeconds();
        menu2.updateFaces(dt);

        window.clear();
        menu_UI.back_ground(window);
        window.draw(menu_UI.bg);
        for (auto& t : menu2.mainmenu)  window.draw(t);
        window.draw(menu2.Face);
        for (auto& p : menu2.pills)     window.draw(p);
        window.draw(menu2.DownFace);
        window.display();
    }
}

void  Menu::GFX(RenderWindow& window)
{
    Menu menu5;
    menu5.choises = 2;
    menu5.selected = 0;
    menu5.mainmenu.resize(2);
    menu5.Face_intilization();
    menu5.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    static const std::string labels[2] = { "Charchters <>","Back" };
    float yOff = 40.f;
    float midY = window.getSize().y * 0.5f;
    for (int i = 0; i < menu5.choises; ++i) {
        menu5.mainmenu[i].setFont(menu5.font);
        menu5.mainmenu[i].setCharacterSize(50);
        menu5.mainmenu[i].setString(labels[i]);
        menu5.mainmenu[i].setPosition(820, midY + yOff);
        menu5.mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOff += menu5.positionOfFace;
    }
    const int players = 2;
    Texture pl[players];
    pl[0].loadFromFile("Assets/Textures/pacman/1.png");
    pl[1].loadFromFile("Assets/Textures/pacman/left_1m.png");

    Sprite chara;
    menu5.deltaClock.restart();

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }
            if (event.type == Event::KeyReleased)
                pressed = false;
            if (event.type == Event::KeyPressed && !pressed) {
                pressed = true;

                if ((event.key.code == Keyboard::Enter && menu5.selected == 1) || (event.key.code == Keyboard::Escape && menu5.selected == 1))
                {
                    return;
                }
                if (event.key.code == Keyboard::Escape) {
                    menu5.mainmenu[menu5.selected].setFillColor(Color::White);
                    menu5.selected = 1;
                    menu5.mainmenu[1].setFillColor(Color{ 255,204,0 });
                    menu5.Face.setPosition(menu5.Face.getPosition().x, midY + 2 * positionOfFace - 15);
                }
                if (event.key.code == Keyboard::Down)
                    menu5.MoveDown(menu5.selected, 2);
                if (event.key.code == Keyboard::Up)
                    menu5.MoveUp(menu5.selected, 2);

            }
            if (menu5.selected == 0)
            {
                if (Keyboard::isKeyPressed(Keyboard::Left))
                {
                    if (character > 0)
                        character--;
                    else
                        character = players - 1;
                }
                if (Keyboard::isKeyPressed(Keyboard::Right))
                {
                    if (character < players - 1)
                        character++;
                    else
                        character = 0;
                }
            }
        }
        float dt = menu5.deltaClock.restart().asSeconds();
        menu5.updateFaces(dt);

        if (character == 0)
        {
            chara.setTexture(pl[0]);
        }
        else if (character == 1)
        {
            chara.setTexture(pl[1]);
        }

        chara.setScale(2.4, 2.4);
        chara.setPosition(1200, midY + 50);

        window.clear();
        menu_UI.back_ground(window);
        window.draw(menu_UI.bg);
        for (auto& t : menu5.mainmenu)  window.draw(t);
        window.draw(menu5.Face);
        for (auto& p : menu5.pills)     window.draw(p);
        window.draw(menu5.DownFace);
        window.draw(chara);
        window.display();
    }
}

void Menu::sound_options(RenderWindow& window)
{
    Menu menu6;

    menu6.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    menu6.Face_intilization();
    menu6.choises = 3;
    menu6.selected = 0;
    menu6.mainmenu.resize(menu6.choises);

    static const std::string labels[3] = {
        "Sound - \t\t\t\t\t +",
        "Music  - \t\t\t\t\t +",
        "Back"
    };
    float yOffset = 40.f;
    float midY = window.getSize().y * 0.5f;
    for (int i = 0; i < menu6.choises; ++i)
    {
        menu6.mainmenu[i].setFont(menu6.font);
        menu6.mainmenu[i].setCharacterSize(50);
        menu6.mainmenu[i].setString(labels[i]);
        menu6.mainmenu[i].setPosition(820, midY + yOffset);
        menu6.mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOffset += menu6.positionOfFace;
    }

    RectangleShape barSound({ stoi(Player::settings["sound"]) * 3.f, 40.f });
    barSound.setFillColor({ 255, 255, 0 });
    barSound.setPosition(970, midY + 1 * menu6.positionOfFace);

    RectangleShape barMusic({ stoi(Player::settings["music"]) * 3.f, 40.f });
    barMusic.setFillColor({ 255, 255, 0 });
    barMusic.setPosition(970, midY + 2 * menu6.positionOfFace);

    menu6.deltaClock.restart();

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }

            if (event.type == Event::KeyReleased)
                pressed = false;

            if (event.type == Event::KeyPressed && !pressed)
            {
                pressed = true;

                if (event.key.code == Keyboard::Down)
                    menu6.MoveDown(menu6.selected, menu6.choises);
                if (event.key.code == Keyboard::Up)
                    menu6.MoveUp(menu6.selected, menu6.choises);


                if (event.key.code == Keyboard::Enter
                    || (event.key.code == Keyboard::Escape && menu6.selected == 2))
                {
                    if (menu6.selected == 2)
                    {
                        FilesController::updatePlayerSettings(Player::playerName, Player::settings);
                        return;
                    }
                }
                if (event.key.code == Keyboard::Escape && menu6.selected != 2)
                {
                    menu6.mainmenu[menu6.selected].setFillColor(Color::White);
                    menu6.selected = 2;
                    menu6.mainmenu[2].setFillColor(Color{ 255,204,0 });
                    menu6.Face.setPosition(
                        menu6.Face.getPosition().x,
                        midY + 3 * menu6.positionOfFace - 15
                    );
                }

                if (menu6.selected == 0)
                {
                    if (Keyboard::isKeyPressed(Keyboard::Left))
                        Player::settings["sound"] = to_string(std::max(0, stoi(Player::settings["sound"]) - 10));
                    if (Keyboard::isKeyPressed(Keyboard::Right))
                        Player::settings["sound"] = to_string(std::min(100, stoi(Player::settings["sound"]) + 10));

                    barSound.setSize({ stoi(Player::settings["sound"]) * 3.f, 40.f });
                }
                else if (menu6.selected == 1)
                {
                    if (Keyboard::isKeyPressed(Keyboard::Left))
                        Player::settings["music"] = to_string(std::max(0, stoi(Player::settings["music"]) - 10));
                    //P_M_Music = max(0, P_M_Music - 10);
                    if (Keyboard::isKeyPressed(Keyboard::Right))
                        Player::settings["music"] = to_string(std::min(100, stoi(Player::settings["music"]) + 10));
                    //P_M_Music = min(100, P_M_Music + 10);

                    barMusic.setSize({ stoi(Player::settings["music"]) * 3.f, 40.f });
                }
                menu6.set_sounds();
            }
        }

        if (!Keyboard::isKeyPressed(Keyboard::Escape))
            pressed = false;

        float dt = menu6.deltaClock.restart().asSeconds();
        menu6.updateFaces(dt);

        window.clear();
        menu_UI.back_ground(window);
        window.draw(menu_UI.bg);

        window.draw(barSound);
        window.draw(barMusic);

        for (auto& txt : menu6.mainmenu)
            window.draw(txt);

        window.draw(menu6.Face);
        for (auto& p : menu6.pills)
            window.draw(p);
        window.draw(menu6.DownFace);

        window.display();
    }
}

void Menu::player_controls(RenderWindow& window)
{
    Menu menu9;
    menu9.Face_intilization();
    menu9.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    menu9.choises = 5;
    menu9.mainmenu.resize(menu9.choises);
    float yOffset = 40.f;
    float midY = window.getSize().y * 0.5f;

    for (int i = 0; i < menu9.choises; ++i)
    {
        menu9.mainmenu[i].setFont(menu9.font);
        menu9.mainmenu[i].setCharacterSize(50);
        menu9.mainmenu[i].setPosition(820, midY + yOffset);
        menu9.mainmenu[i].setFillColor(i == 0 ? Color{ 255,204,0 } : Color::White);
        yOffset += menu9.positionOfFace;
    }

    menu9.mainmenu[4].setString("Back");

    bool draw = false;
    RectangleShape overlay;
    overlay.setSize(Vector2f(1920, 1080));
    overlay.setFillColor(Color(0, 0, 0, 100));

    Texture direction[4];
    direction[0].loadFromFile("Assets/Textures/left.png");
    direction[1].loadFromFile("Assets/Textures/right.png");
    direction[2].loadFromFile("Assets/Textures/up.png");
    direction[3].loadFromFile("Assets/Textures/down.png");

    Sprite directionSprite;
    directionSprite.setScale(1.5, 1.5);
    directionSprite.setPosition(650, 400);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }

            if (event.type == Event::KeyReleased)
                pressed = false;

            if (event.type == Event::KeyPressed && !pressed)
            {
                if (!waitingForKey)
                {
                    if (event.key.code == Keyboard::Down)
                        menu9.MoveDown(menu9.selected, menu9.choises);
                    else if (event.key.code == Keyboard::Up)
                        menu9.MoveUp(menu9.selected, menu9.choises);
                    else if (event.key.code == Keyboard::Escape && menu9.selected != 4)
                    {
                        menu9.mainmenu[menu9.selected].setFillColor(Color::White);
                        menu9.selected = 4;
                        menu9.mainmenu[4].setFillColor(Color{ 255, 204, 0 });
                        menu9.Face.setPosition(menu9.Face.getPosition().x, midY + 5 * menu9.positionOfFace - 15);
                        pressed = true;
                    }
                    if (((event.key.code == Keyboard::Enter && menu9.selected == 4) || (event.key.code == Keyboard::Escape && menu9.selected == 4)) && !pressed)
                    {
                        FilesController::updatePlayerSettings(Player::playerName, Player::settings);
                        return;
                    }
                    else if (event.key.code == Keyboard::Enter)
                    {
                        pressed = true;
                        directionSprite.setTexture(direction[menu9.selected]);
                        waitingForKey = true;
                        draw = true;
                    }
                }
                else // Waiting for a new key
                {
                    if (event.key.code != Keyboard::Enter)
                    {
                        int action = menu9.selected;
                        menu9.changeKeyMapping(action, event.key.code, menu9);
                        draw = false;
                        waitingForKey = false;
                        pressed = true;
                    }
                }
            }
        }

        // Update labels with current key bindings
        menu9.mainmenu[0].setString("Move Left : " + Player::settings["moveLeftKey"]);
        menu9.mainmenu[1].setString("Move Right : " + Player::settings["moveRightKey"]);
        menu9.mainmenu[2].setString("Move Up : " + Player::settings["moveUpkey"]);
        menu9.mainmenu[3].setString("Move Down : " + Player::settings["moveDownkey"]);

        float dt = menu9.deltaClock.restart().asSeconds();
        menu9.updateFaces(dt);

        window.clear();
        menu_UI.back_ground(window);
        window.draw(menu_UI.bg);
        for (auto& t : menu9.mainmenu) window.draw(t);
        window.draw(menu9.Face);
        for (auto& p : menu9.pills)     window.draw(p);
        window.draw(menu9.DownFace);

        if (draw)
        {
            window.draw(overlay);
            window.draw(directionSprite);
        }

        window.display();
    }
}

void Menu::options_menu(RenderWindow& window)
{
    Menu menu4;

    menu4.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    menu4.Face_intilization();

    menu4.choises = 4;
    menu4.selected = 0;
    menu4.mainmenu.resize(menu4.choises);

    static const std::string labels[4] = {
        "GFX Options",
        "Sound Options",
        "Controls",
        "Back"
    };
    float yOffset = 40.f;
    float midY = window.getSize().y * 0.5f;
    for (int i = 0; i < menu4.choises; ++i)
    {
        menu4.mainmenu[i].setFont(menu4.font);
        menu4.mainmenu[i].setCharacterSize(50);
        menu4.mainmenu[i].setString(labels[i]);
        menu4.mainmenu[i].setPosition(820, midY + yOffset);
        menu4.mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOffset += menu4.positionOfFace;
    }

    menu4.deltaClock.restart();

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }

            if (event.type == Event::KeyReleased)
                pressed = false;

            if (event.type == Event::KeyPressed && !pressed)
            {
                pressed = true;

                if (event.key.code == Keyboard::Down)
                {
                    menu4.MoveDown(menu4.selected, menu4.choises);
                }
                if (event.key.code == Keyboard::Up)
                {
                    menu4.MoveUp(menu4.selected, menu4.choises);
                }
                if ((event.key.code == Keyboard::Enter && menu4.selected == 3)
                    || (event.key.code == Keyboard::Escape && menu4.selected == 3))
                {
                    return;
                }
                if (event.key.code == Keyboard::Escape && menu4.selected != 3)
                {
                    menu4.mainmenu[menu4.selected].setFillColor(Color::White);
                    menu4.selected = 3;
                    menu4.mainmenu[3].setFillColor(Color{ 255,204,0 });
                    menu4.Face.setPosition(menu4.Face.getPosition().x,
                        midY + 4 * menu4.positionOfFace - 15);
                }
                if (event.key.code == Keyboard::Enter
                    || (event.key.code == Keyboard::Escape && menu4.selected == 3))
                {
                    if (menu4.selected == 0)  GFX(window);
                    if (menu4.selected == 1)  sound_options(window);
                    if (menu4.selected == 2)   player_controls(window);
                }
            }
        }
        float dt = menu4.deltaClock.restart().asSeconds();
        menu4.updateFaces(dt);

        window.clear();
        menu_UI.back_ground(window);
        window.draw(menu_UI.bg);
        for (auto& txt : menu4.mainmenu)
            window.draw(txt);
        window.draw(menu4.Face);
        for (auto& pill : menu4.pills)
            window.draw(pill);
        window.draw(menu4.DownFace);
        window.display();
    }
}

void  Menu::credits(RenderWindow& window)
{
  
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            pressed = true;
            return;
        }
        window.clear();
        window.display();
    }
}

void  Menu::instructions(RenderWindow& window)
{
    Texture instr;
    instr.loadFromFile("Assets/Textures/instructions.jpg");
    Sprite instructions;
    instructions.setTexture(instr);
    instructions.setScale(3.2, 3.66);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            pressed = true;
            return;
        }
        window.clear();
        window.draw(instructions);
        window.display();
    }
}

void Menu::Pause(RenderWindow& window, Texture gametexture)
{
    END |= win;
    Menu Pause1, option;
    Pause1.choises = END + 3;
    option.choises = 3;
    Pause1.mainmenu.resize(Pause1.choises);
    option.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    Pause1.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    int x = 200;
    for (int i = 0; i < Pause1.choises; i++)
    {
        if (!i) Pause1.mainmenu[i].setFillColor(Color{ 255,204,0 });
        else  Pause1.mainmenu[i].setFillColor(Color::White);
        Pause1.mainmenu[i].setFont(Pause1.font);
        Pause1.mainmenu[i].setCharacterSize(70);
        Pause1.mainmenu[i].setPosition(670, x);
        x += 100;
    }
    option.mainmenu.resize(option.choises);
    x = 200;
    for (int i = 0; i < option.choises; i++)
    {
        if (!i) option.mainmenu[i].setFillColor(Color{ 255,204,0 });
        else  option.mainmenu[i].setFillColor(Color::White);
        option.mainmenu[i].setFont(option.font);
        option.mainmenu[i].setCharacterSize(70);
        option.mainmenu[i].setPosition(670, x);
        x += 100;
    }
    Pause1.mainmenu[0].setString("resume");
    if(win)
        Pause1.mainmenu[0].setString("Next Level");
    Pause1.mainmenu[0 + END].setString("Play Again");
    Pause1.mainmenu[1 + END].setString("Sound Option");
    Pause1.mainmenu[2 + END].setString("Exit");

    option.mainmenu[0].setString("Sound - \t\t\t   +");
    option.mainmenu[1].setString("Music  - \t\t\t   +");
    option.mainmenu[2].setString("Back");


    RectangleShape photo2;
    photo2.setSize(Vector2f(1920, 1080));
    photo2.setFillColor(Color(0, 0, 0, 100));

    Sprite game;
    game.setTexture(gametexture);

    Texture txx;
    txx.loadFromFile("Assets/Textures/Pausemenu.png");

    Sprite pausemenu;
    pausemenu.setTexture(txx);
    pausemenu.setPosition(600, 150);
    pausemenu.setScale(1.7, 2.2);
    Pause1.Face_intilization();
    Pause1.Face.setPosition(630, 220);
    Pause1.positionOfFace = 100;


    option.Face_intilization();
    option.Face.setPosition(630, 220);
    option.positionOfFace = 100;

    RectangleShape Sound(Vector2f(stoi(Player::settings["sound"]) * 3, 40));
    Sound.setFillColor({ 255, 255, 0 });
    Sound.setPosition(885, 220);

    RectangleShape Music(Vector2f(stoi(Player::settings["music"]) * 3, 40));
    Music.setFillColor({ 255, 255, 0 });
    Music.setPosition(885, 320);

    bool op = false;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                FilesController::updatePlayerSettings(Player::playerName, Player::settings);
            }
            if (event.type == sf::Event::KeyReleased) {
                if (!op) {
                    if (event.key.code == sf::Keyboard::Up) {
                        Pause1.MoveUp(Pause1.selected, Pause1.choises);
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        Pause1.MoveDown(Pause1.selected, Pause1.choises);
                    }
                    if (event.key.code == Keyboard::Escape && !pressed && Pause1.selected != 2 + END)
                    {
                        Pause1.mainmenu[Pause1.selected].setFillColor(Color::White);
                        Pause1.selected = 2 + END;
                        Pause1.mainmenu[2 + END].setFillColor(Color{ 255,204,0 });
                        Pause1.Face.setPosition(630, END ? 520 : 420);
                        pressed = true;
                    }
                    if (event.key.code == Keyboard::Enter || (event.key.code == Keyboard::Escape && !pressed)) {
                        if (Pause1.selected == -1 + END) {
                            if (win)
                            {
                                Ghost::timer = 0;
                                Gameplay(1);
                            }
                            else
                                return;
                        }
                        if (Pause1.selected == 0 + END) {
                            play_again = 1;
                            exit = 1;
                            return;
                        }
                        if (Pause1.selected == 1 + END) {
                            op = true;
                        }
                        if (Pause1.selected == 2 + END) {
                            window.setView(window.getDefaultView());
                            play_again = 0;
                            exit = 1;
                            FilesController::updatePlayerSettings(Player::playerName, Player::settings);
                            Pause1.menu1(window);
                        }
                    }


                    if (event.type == Event::KeyReleased)
                        pressed = false;
                }
                else if (op)
                {
                    if (event.key.code == sf::Keyboard::Up) {
                        option.MoveUp(option.selected, option.choises);
                        //break;
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        option.MoveDown(option.selected, option.choises);
                        //break;
                    }
                    if (event.key.code == Keyboard::Escape && !pressed && option.selected != 2)
                    {
                        option.mainmenu[option.selected].setFillColor(Color::White);
                        option.selected = 2;
                        option.mainmenu[2].setFillColor(Color{ 255,204,0 });
                        option.Face.setPosition(630, 420);
                        pressed = true;
                    }
                    if (event.key.code == Keyboard::Enter || (event.key.code == Keyboard::Escape && !pressed))
                    {
                        if (option.selected == 2) {
                            pressed = true;
                            op = false;
                        }
                    }
                    if (option.selected == 0)
                    {
                        if (event.key.code == Keyboard::Left)
                            Player::settings["sound"] = to_string(std::max(0, stoi(Player::settings["sound"]) - 10));
                        if (event.key.code == Keyboard::Right)
                            Player::settings["sound"] = to_string(std::min(100, stoi(Player::settings["sound"]) + 10));
                        Sound.setSize(Vector2f(stoi(Player::settings["sound"]) * 3, 40));
                    }
                    else if (option.selected == 1)
                    {
                        if (event.key.code == Keyboard::Left)
                            Player::settings["music"] = to_string(std::max(0, stoi(Player::settings["music"]) - 10));
                        if (event.key.code == Keyboard::Right)
                            Player::settings["music"] = to_string(std::min(100, stoi(Player::settings["music"]) + 10));
                        Music.setSize(Vector2f(stoi(Player::settings["music"]) * 3, 40));
                    }
                    option.set_sounds();

                    if (!Keyboard::isKeyPressed(Keyboard::Escape))
                    {
                        pressed = false;
                    }
                }

            }
        }

        float dt = Pause1.deltaClock.restart().asSeconds();
        Pause1.updateFaces(dt);

        float dt1 = option.deltaClock.restart().asSeconds();
        option.updateFaces(dt1);

        window.clear();
        window.draw(game);
        window.draw(photo2);
        window.draw(pausemenu);
        if (!op) {
            for (int i = 0; i < Pause1.choises; i++) {
                window.draw(Pause1.mainmenu[i]);
            }
            window.draw(Pause1.Face);
        }
        else
        {
            for (int i = 0; i < option.choises; i++) {
                window.draw(option.mainmenu[i]);
            }
            window.draw(option.Face);
            window.draw(Sound);
            window.draw(Music);
        }
        window.display();
    }
}