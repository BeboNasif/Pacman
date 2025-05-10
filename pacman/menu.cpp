#include "Menu.h"
#include "menu_Bg.h"
#include "Sounds.h"
using namespace sf;

menu_Bg menu_UI;
bool pressed = false;
bool esc_button = false;
int P_M_Sound = 100;
int P_M_Music = 100;
extern Sounds sound;
void Gameplay();

Menu::Menu()
    : choises(0)
    , selected(0)
    , downFaceSpeed(100.f)
    , currentFrame(0)
    , eatenPills(0)
    , delayStarted(false)
    , positionOfFace(60.f)
    , frameDuration(seconds(0.1f))
    , pageNumber(0)
{
    mainmenu.reserve(10);
    faceFrames.reserve(3);
    pills.reserve(3);

    pillConsumed.assign(3, false);
    pillDelayStarted.assign(3, false);
    pillTimers.assign(3, Clock{});
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
    if(DownFace.getPosition().x>600)
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
    Face.setPosition(830, 582);
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
    font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    sound.music(0);
    choises = 7;
    selected = 0;
    mainmenu.resize(choises);

    static const std::string labels[7] = {
        "Play Game","Instructions","Change Profile",
        "High Score","Options","Credits","Exit"
    };

    float yOff = 40.f;
    float midY = window.getSize().y * 0.5f;
    for (int i = 0; i < choises; ++i) {
        mainmenu[i].setFont(font);
        mainmenu[i].setCharacterSize(50);
        mainmenu[i].setString(labels[i]);
        mainmenu[i].setPosition(850, midY + yOff);
        mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOff += positionOfFace;
    }

    Face_intilization();
    deltaClock.restart();
    pageNumber = 1000;

    while (window.isOpen()) {
        Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == Event::Closed)
                window.close();
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
                    if (selected == 1)
                        instructions(window);
                    if (selected == 4)
                        options_menu(window);
                    if (selected == 5)
                        credits(window);
                    if (selected == 6)
                        pageNumber = -1;
                }
                if (evt.key.code == Keyboard::Escape) {
                    mainmenu[selected].setFillColor(Color::White);
                    selected = 6;
                    mainmenu[6].setFillColor(Color{ 255,204,0 });
                    Face.setPosition(830, midY + 7 * positionOfFace-15);
                }
            }
        }

        if (pageNumber == -1) {
            window.close();
            break;
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

void Menu::Play_menu(RenderWindow& window)
{
    Menu menu2;
    menu2.choises = 3;
    menu2.selected = 0;
    menu2.mainmenu.resize(3);

    menu2.font.loadFromFile("Assets/Fonts/HalloweenSlimePersonalUse-4B80D.otf");
    menu2.Face_intilization();

    static const std::string labels[3] = { "Single","Multi","Back" };
    float yOff = 40.f;
    float midY = window.getSize().y * 0.5f;
    for (int i = 0; i < 3; ++i) {
        menu2.mainmenu[i].setFont(menu2.font);
        menu2.mainmenu[i].setCharacterSize(50);
        menu2.mainmenu[i].setString(labels[i]);
        menu2.mainmenu[i].setPosition(850, midY + yOff);
        menu2.mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOff += menu2.positionOfFace;
    }

    menu2.deltaClock.restart();
    menu2.pageNumber = 500;

    while (window.isOpen()) {
        Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == Event::Closed)
                window.close();
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
                    menu2.Face.setPosition(830, midY + 3 * positionOfFace -15);
                }
                if (evt.key.code == Keyboard::Down)
                    menu2.MoveDown(menu2.selected, 3);
                if (evt.key.code == Keyboard::Up)
                    menu2.MoveUp(menu2.selected, 3);

                if (evt.key.code == Keyboard::Enter) {
                    if (menu2.selected == 0) Gameplay();
                    else if (menu2.selected == 1) Gameplay();
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
        menu6.mainmenu[i].setPosition(850, midY + yOffset);
        menu6.mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOffset += menu6.positionOfFace;
    }

    RectangleShape barSound({ P_M_Sound * 3.f, 40.f });
    barSound.setFillColor({ 255, 255, 0 });
    barSound.setPosition(1005, midY + 1 * menu6.positionOfFace);

    RectangleShape barMusic({ P_M_Music * 3.f, 40.f });
    barMusic.setFillColor({ 255, 255, 0 });
    barMusic.setPosition(1005, midY + 2 * menu6.positionOfFace);

    menu6.deltaClock.restart();
    menu6.pageNumber = 1000;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

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
                        return;
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
                        P_M_Sound = std::max(0, P_M_Sound - 10);
                    if (Keyboard::isKeyPressed(Keyboard::Right))
                        P_M_Sound = std::min(100, P_M_Sound + 10);

                    barSound.setSize({ P_M_Sound * 3.f, 40.f });
                    sound.so.setVolume(P_M_Sound);
                    sound.so2.setVolume(P_M_Sound);
                }
                else if (menu6.selected == 1)
                {
                    if (Keyboard::isKeyPressed(Keyboard::Left))
                        P_M_Music = std::max(0, P_M_Music - 10);
                    if (Keyboard::isKeyPressed(Keyboard::Right))
                        P_M_Music = std::min(100, P_M_Music + 10);

                    barMusic.setSize({ P_M_Music * 3.f, 40.f });
                    sound.bgmusic.setVolume(P_M_Music);
                }
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
        menu4.mainmenu[i].setPosition(850, midY + yOffset);
        menu4.mainmenu[i].setFillColor(i == 0
            ? Color{ 255,204,0 }
        : Color::White);
        yOffset += menu4.positionOfFace;
    }

    menu4.deltaClock.restart();
    menu4.pageNumber = 1000;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

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
                        midY + 4 * menu4.positionOfFace-15);
                }
                if (event.key.code == Keyboard::Enter
                    || (event.key.code == Keyboard::Escape && menu4.selected == 3))
                {
                    //if (menu4.selected == 0)  /*options_menu1(window);*/
                    if (menu4.selected == 1)  sound_options(window);
                    //if (menu4.selected == 2)   /*control_menu(window);*/
                }
            }
        }

        if (!Keyboard::isKeyPressed(Keyboard::Escape))
            pressed = false;
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
    Texture cre;
    cre.loadFromFile("Assets/Textures/d7k.jpg");
    Sprite credits;
    credits.setTexture(cre);
	credits.scale(5, 5);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            pageNumber = 1000;
            pressed = true;
            return;
        }
        window.clear();
        window.draw(credits);
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
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            pageNumber = 1000;
            pressed = true;
            return;
        }
        window.clear();
        window.draw(instructions);
        window.display();
    }
}