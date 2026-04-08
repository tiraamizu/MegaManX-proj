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
    GameState curState = MAIN;
};

struct StateSwitchData {
    GameState curState;
    MenuData mainMenu;
    MenuData optionsMenu;
};

StateSwitchData ssv; //state switch variable

// Function declarations (m is a menu struct variable, its passed by reference to avoid copying the struct and to allow us to mod the struct's data)

//1. Menu declaration functions (for main menu and options menu)
void initMenu(MenuData &m, float width, float height);
void initOptions(MenuData &m, float width, float height);
void drawMenuSelection(MenuData &m, RenderWindow &window);
void up(MenuData &m);
void down(MenuData &m);
void menuSwitchHandler(RenderWindow &window, Event &event, StateSwitchData &ssv, Keyboard::Key interractionButton);
bool logoCheck(MenuData &m, RenderWindow &window);

//2
int main()
{
    //Main Window Resolution
    const int windowWidth = 640;
    const int windowHeight = 480;

    RenderWindow window(VideoMode(windowWidth, windowHeight), "MMX prototype");

    //Menu stuff (will add sub-menus like options and PASS WORRD stuff later)
    initMenu(ssv.mainMenu, (float)window.getSize().x, (float)window.getSize().y); //initializes menu

    //Options
    initOptions(ssv.optionsMenu, (float)window.getSize().x, (float)window.getSize().y); //initializes options menu

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
            menuSwitchHandler(window, event, ssv, interractionButton);
        }

        window.clear();

        switch (ssv.mainMenu.curState)
        {
        case MAIN:
            drawMenuSelection(ssv.mainMenu, window);
            if (!logoCheck(ssv.mainMenu, window)) {
                return 0;
            }; //crashes program to stop message from looping if logo is not found
            break;

        case OPTIONS:
            drawMenuSelection(ssv.optionsMenu, window);
            break;

        case GAME:
            window.clear();
            break;

        default:
            ssv.mainMenu.curState = MAIN;
            break;
        }

        window.display();
    }
    return 0;
}

//FUNCTION DEFINITIONS  

//Functions for menu navigation and drawing. These functions are used to initialize the menu, draw the menu to the window, and handle up/down navigation through the menu options. The logoCheck function is used to load and display the logos on the main menu, and it returns false if the logos cannot be found, which is used to prevent an infinite loop of error messages in the main function.

int charSize = 20, xOffset = -100, yOffset = 270;

//Initializes main menu
bool logoCheck(MenuData &m, RenderWindow &window) {
    if (!m.Logo.loadFromFile("textures/logo.png") || !m.XLogo.loadFromFile("textures/XLogo.png")) {
        cout << "ERR : Logo not found";
        return false;
    }
        m.XLogoSprite.setTexture(m.XLogo);
        m.XLogoSprite.setPosition(320, 130);
        m.XLogoSprite.setScale(1.5f, 1.5f);
        m.logoSprite.setTexture(m.Logo);
        m.logoSprite.setPosition(150, 130);
        m.logoSprite.setScale(1.5f, 1.5f);
    window.draw(m.XLogoSprite);
    window.draw(m.logoSprite);
    return true;
}

void initMenu(MenuData &m, float width, float height) {
    if (!m.font.loadFromFile("fonts/mega-man-x.ttf")) {
        cout << "ERR : Font not found";
    }
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

void menuSwitchHandler(RenderWindow &window, Event &event, StateSwitchData &ssv, Keyboard::Key interractionButton) {
    if (event.type == Event::KeyPressed) {
        switch (ssv.mainMenu.curState)
        {
        case MAIN:
            if (event.key.code == Keyboard::Up) {
                up(ssv.mainMenu);
            }
            if (event.key.code == Keyboard::Down) {
                down(ssv.mainMenu);
            }
            if (event.key.code == interractionButton) {
                if (ssv.mainMenu.curButtonIndex == 0) {
                    ssv.mainMenu.curState = GAME;
                }
                if (ssv.mainMenu.curButtonIndex == 2) {
                    ssv.mainMenu.curState = OPTIONS;
                }
                if (ssv.mainMenu.curButtonIndex == 3) {
                    window.close();
                }
            }
            break;

        case OPTIONS:
            if (event.key.code == Keyboard::Up) {
                up(ssv.optionsMenu);
            }
            if (event.key.code == Keyboard::Down) {
                down(ssv.optionsMenu);
            }
            if (event.key.code == interractionButton) {
                if (ssv.optionsMenu.curButtonIndex == 2) {
                    ssv.mainMenu.curState = MAIN;
                }
            }
            break;

        case GAME:
            // put game events in here (like player movement and stuff)
            // temp solution for entering the game (press X to return to menu)
            // make a function to handle inputs for easier management
            // also gotta make an ingame menu somehow for ingame stuff
            if (event.key.code == Keyboard::X) {
                ssv.mainMenu.curState = MAIN;
            }
            break;

        default:
            break;
        }
    }
}