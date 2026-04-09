#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>

#define MAX_ITEM_NO 10

using namespace std;
using namespace sf;

enum GameState { MAIN, OPTIONS, GAME };

//STRUCTS

struct MenuData {
    Font font;
    Text menuSelection[MAX_ITEM_NO];
    int curMaxButtons;
    int curButtonIndex;
    Color unselectedColor = sf::Color(83, 227, 255);
    Color selectedColor = sf::Color(234, 138, 13);
    Texture Logo, XLogo;
    Sprite logoSprite, XLogoSprite;
    GameState curState = MAIN; // State logic is now tied to the menu data
};

// Function declarations (m is a menu struct variable, its passed by reference to avoid copying the struct and to allow us to mod the struct's data)

//1. Menu declaration functions (for main menu and options menu)
void initMenu(MenuData &m, float width, float height);
void initOptions(MenuData &m, float width, float height);
void drawMenuSelection(MenuData &m, RenderWindow &window);
void up(MenuData &m);
void down(MenuData &m);
void menuSwitchHandler(RenderWindow &window, Event &event, MenuData &main, MenuData &options, Keyboard::Key interractionButton);
bool resourcesCheck(MenuData &m, RenderWindow &window);

//2
int main()
{
    //Main Window Resolution
    const int windowWidth = 640;
    const int windowHeight = 480;

    RenderWindow window(VideoMode(windowWidth, windowHeight), "MMX prototype");

    MenuData mainMenu;
    MenuData optionsMenu;

    // Load resources for both menus
    if (!resourcesCheck(mainMenu, window) || !resourcesCheck(optionsMenu, window)) {
        return 0;
    }; //crashes program to stop message from looping if logo is not found

    //Menu stuff (will add sub-menus like options and PASS WORRD stuff later)
    initMenu(mainMenu, (float)window.getSize().x, (float)window.getSize().y); //initializes menu

    //Options
    initOptions(optionsMenu, (float)window.getSize().x, (float)window.getSize().y); //initializes options menu

    //Game

    //Password
    // (TBD)

    //Controls
    Keyboard::Key interractionButton = Keyboard::Z;

    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                window.close();
            }
            menuSwitchHandler(window, event, mainMenu, optionsMenu, interractionButton);
        }

        window.clear();

        // Use mainMenu.curState to determine which menu to draw
        switch (mainMenu.curState)
        {
        case MAIN:
            window.draw(mainMenu.XLogoSprite);
            window.draw(mainMenu.logoSprite); //draws logo
            drawMenuSelection(mainMenu, window);
            break;

        case OPTIONS:
            drawMenuSelection(optionsMenu, window);
            break;

        case GAME:
            if (event.key.code == Keyboard::X) {
                mainMenu.curState = MAIN;
            }
            window.clear();
            break;
            //events of game go here

        default:
            mainMenu.curState = MAIN;
            break;
        }

        window.display();
    }
    return 0;
}

//FUNCTION DEFINITIONS  

int charSize = 20, xOffset = -100, yOffset = 270;

//Initializes main menu resources
bool resourcesCheck(MenuData &m, RenderWindow &window) {
    if (!m.Logo.loadFromFile("textures/logo.png") || !m.XLogo.loadFromFile("textures/XLogo.png")) {
        cout << "ERR : Logo not found";
        return false;
    }
    if (!m.font.loadFromFile("fonts/mega-man-x.ttf")) {
        cout << "ERR : Font not found";
        return false;
    }
    m.XLogoSprite.setTexture(m.XLogo);
    m.XLogoSprite.setPosition(320, 130);
    m.XLogoSprite.setScale(1.5f, 1.5f);
    m.logoSprite.setTexture(m.Logo);
    m.logoSprite.setPosition(150, 130);
    m.logoSprite.setScale(1.5f, 1.5f);
    return true;
}

void initMenu(MenuData &m, float width, float height) {
    m.curMaxButtons = 4;

    //GAME START
    m.menuSelection[0].setFont(m.font);
    m.menuSelection[0].setFillColor(m.selectedColor);
    m.menuSelection[0].setString("GAME START");
    m.menuSelection[0].setCharacterSize(charSize);
    m.menuSelection[0].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 1) * 1) + yOffset);

    //PASS WORD
    m.menuSelection[1].setFont(m.font);
    m.menuSelection[1].setFillColor(m.unselectedColor);
    m.menuSelection[1].setString("PASS WORD");
    m.menuSelection[1].setCharacterSize(charSize);
    m.menuSelection[1].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 1) * 1.5) + yOffset);

    //OPTION MODE
    m.menuSelection[2].setFont(m.font);
    m.menuSelection[2].setFillColor(m.unselectedColor);
    m.menuSelection[2].setString("OPTION MODE");
    m.menuSelection[2].setCharacterSize(charSize);
    m.menuSelection[2].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 1) * 2) + yOffset);

    //TERMINATE
    m.menuSelection[3].setFont(m.font);
    m.menuSelection[3].setFillColor(m.unselectedColor);
    m.menuSelection[3].setString("TERMINATE");
    m.menuSelection[3].setCharacterSize(charSize);
    m.menuSelection[3].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 1) * 2.5) + yOffset);

    m.curButtonIndex = 0;
}

//Draws menu components to the window
void drawMenuSelection(MenuData &m, RenderWindow &window) {
    for (int i = 0; i < m.curMaxButtons; ++i) {
        window.draw(m.menuSelection[i]);
    }
}

//Menu up button function
void up(MenuData &m) {
    m.menuSelection[m.curButtonIndex].setFillColor(m.unselectedColor);
    m.curButtonIndex--;
    if (m.curButtonIndex < 0) {
        m.curButtonIndex = m.curMaxButtons - 1;
    }
    m.menuSelection[m.curButtonIndex].setFillColor(m.selectedColor);
}

//Menu down button function
void down(MenuData &m) {
    m.menuSelection[m.curButtonIndex].setFillColor(m.unselectedColor);
    m.curButtonIndex++;
    if (m.curButtonIndex >= m.curMaxButtons) {
        m.curButtonIndex = 0;
    }
    m.menuSelection[m.curButtonIndex].setFillColor(m.selectedColor);
}

//Initializes options menu
void initOptions(MenuData &m, float width, float height) {
    // we use initMenu to copy data from the menu struct and reuse it in options.
    initMenu(m, width, height);

    m.curMaxButtons = 3;
    //CONTROLS
    m.menuSelection[0].setString("CONTROLS");
    //AUDIO
    m.menuSelection[1].setString("AUDIO");
    //BACK
    m.menuSelection[2].setString("BACK");

    m.curButtonIndex = 0;
}

void menuSwitchHandler(RenderWindow &window, Event &event, MenuData &main, MenuData &options, Keyboard::Key interractionButton) {
    if (event.type == Event::KeyPressed) {
        switch (main.curState)
        {
        case MAIN:
            if (event.key.code == Keyboard::Up) {
                up(main);
            }
            if (event.key.code == Keyboard::Down) {
                down(main);
            }
            if (event.key.code == interractionButton) {
                if (main.curButtonIndex == 0) {
                    main.curState = GAME;
                }
                if (main.curButtonIndex == 2) {
                    main.curState = OPTIONS;
                }
                if (main.curButtonIndex == 3) {
                    window.close();
                }
            }
            break;

        case OPTIONS:
            if (event.key.code == Keyboard::Up) {
                up(options);
            }
            if (event.key.code == Keyboard::Down) {
                down(options);
            }
            if (event.key.code == interractionButton) {
                if (options.curButtonIndex == 2) {
                    main.curState = MAIN;
                }
            }
            break;

        case GAME:
            if (event.key.code == Keyboard::X) {
                main.curState = MAIN;
            }
            break;

        default:
            break;
        }
    }
}