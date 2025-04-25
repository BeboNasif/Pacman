#include "Menu.h"
#include "menu_Bg.h"

Event event;
menu_Bg menu_UI;

bool pressed = true;
bool esc_button = true;
void Gameplay();

Menu::Menu()
{
    mainmenu.reserve(10);
    faceFrames.reserve(3);
    pills.reserve(3);
    pillConsumed.reserve(3);
}

void Menu::Face_intilization()
{
    pillTimers.resize(3);
    pillDelayStarted.assign(3, false);
    faceFrames.resize(3);
    pills.resize(3);
    pillConsumed.assign(3, false);

    faceFrames[0].loadFromFile("Assets/Textures/pacman/0.png");
    faceFrames[1].loadFromFile("Assets/Textures/pacman/1.png");
    faceFrames[2].loadFromFile("Assets/Textures/pacman/2.png");

    Face.setTexture(&faceFrames[0]);
    Face.setSize(Vector2f(60, 60));
    Face.setPosition(830, 582);
    Face.setScale(-1, 1);

    DownFace.setTexture(&faceFrames[0]);
    DownFace.setSize(Vector2f(50, 50));
    DownFace.setPosition(400, 1000);
    DownFace.setScale(-1, 1);

    currentFrame = 0;
    animationClock.restart();
    frameDuration = milliseconds(300);

    pill.loadFromFile("Assets/Textures/pacman/pill.png");
    Vector2f pillPositions[3] = { {400.f,1000.f}, {450.f,1000.f}, {500.f,1000.f} };
    for (int i = 0; i < 3; ++i)
    {
        pills[i].setTexture(&pill);
        pills[i].setSize({ 50, 50 });
        pills[i].setPosition(pillPositions[i]);
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

    choises = 7;
    mainmenu.resize(choises);
    selected = 0;

    string labels[] = {
        "Play Game", "Instructions", "Change Profile",
        "High Score", "Options", "Credits", "Exit"
    };

    x = 40;
    for (int i = 0; i < choises; i++)
    {
        mainmenu[i].setString(labels[i]);
        mainmenu[i].setFillColor(i == 0 ? Color{ 255, 204, 0 } : Color::White);
        mainmenu[i].setFont(font);
        mainmenu[i].setCharacterSize(50);
        mainmenu[i].setPosition(Vector2f(850, height / 2 + x));
        x += 60;
    }

    menu_UI.back_ground(window);
    Face_intilization();
    deltaClock.restart();

    while (window.isOpen())
    {
        if (pageNumber == 1000)
        {
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
                        MoveDown(selected, choises);

                    if (event.key.code == Keyboard::Up)
                        MoveUp(selected, choises);

                    if (event.key.code == Keyboard::Enter || (event.key.code == Keyboard::Escape && selected == 6))
                    {
                        esc_button = 1;
                        if (selected == 0)
                            Gameplay();
                       	if (selected == 6)
								pageNumber = -1;

                        
                    }

                    if (event.key.code == Keyboard::Escape)
                    {
                        mainmenu[selected].setFillColor(Color::White);
                        selected = 6;
                        mainmenu[6].setFillColor(Color{ 255, 204, 0 });
                        Face.setPosition(830, 582 + 6 * shift);
                    }
                }
            }
        }

        if (pageNumber == -1)
        {
            window.close();
            break;
        }

        float dt = deltaClock.restart().asSeconds();
        DownFace.move(downFaceSpeed * dt, 0);
        

        for (int i = 0; i < 3; ++i)
        {
            if (!pillConsumed[i] && !pillDelayStarted[i] && DownFace.getGlobalBounds().intersects(pills[i].getGlobalBounds()))
            {
                pillDelayStarted[i] = true;
                pillTimers[i].restart();
            }
        }
        for (int i = 0; i < 3; ++i)
        {
            if (pillDelayStarted[i] && pillTimers[i].getElapsedTime().asSeconds() >= 0.5f)
            {
                pillConsumed[i] = true;
                eatenPills++;
                pills[i].setPosition(-100.f, -100.f);
                pillDelayStarted[i] = false;
            }
        }

        if (eatenPills >= 3 && !delayStarted)
        {
            delayStarted = true;
            resetDelayClock.restart(); 
            pillDelayStarted.assign(3, false);
            pillTimers.clear();
            pillTimers.resize(3);
        }

        if (delayStarted && resetDelayClock.getElapsedTime().asSeconds() >= .4f) 
        {
            eatenPills = 0;
            Vector2f pillPositions[3] = { {400.f,1000.f},{450.f,1000.f},{500.f,1000.f} };
            for (int i = 0; i < 3; ++i)
            {
                pillConsumed[i] = false;
                pills[i].setPosition(pillPositions[i]);
            }
            DownFace.setPosition(400.f, 1000.f);
            delayStarted = false;  
        }


        if (animationClock.getElapsedTime() > frameDuration)
        {
            currentFrame = (currentFrame + 1) % faceFrames.size();
            Face.setTexture(&faceFrames[currentFrame]);
            DownFace.setTexture(&faceFrames[currentFrame]);
            animationClock.restart();
        }

        window.clear();
        window.draw(menu_UI.bg);
        for (auto& item : mainmenu)
            window.draw(item);
        window.draw(Face);
        for (auto& p : pills)
            window.draw(p);
        window.draw(DownFace);
        window.display();
    }
}
