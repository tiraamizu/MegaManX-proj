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

struct MenuData {
    Font font;
    Text menuSelection[MAX_ITEM_NO];
    int curMaxButtons;
    int curButtonIndex;
	Color unselectedColor = sf::Color(83, 227, 255);
	Color selectedColor = sf::Color(234, 138, 13);
	Texture Logo, XLogo;
	Sprite logoSprite, XLogoSprite;
};

// Function declarations (m is a menu struct variable, its passed by reference to avoid copying the struct and to allow us to mod the struct's data)

//1. Menu declaration functions (for main menu and options menu)
void initMenu(MenuData &m, float width, float height);
void initOptions(MenuData &m, float width, float height);
void drawMenu(MenuData &m, RenderWindow &window);
void up(MenuData &m);
void down(MenuData &m);
bool logoCheck(MenuData &m, RenderWindow &window);



enum GameState { MAIN, OPTIONS, GAME};

int main()
{
    //Main Window Resolution
    const int windowWidth = 640;
    const int windowHeight = 480;

    RenderWindow window(VideoMode(windowWidth, windowHeight), "MMX prototype");

    //Menu stuff (will add sub-menus like options and PASS WORRD stuff later)
    MenuData mainMenu;
    initMenu(mainMenu, (float)window.getSize().x, (float)window.getSize().y); //initializes menu

    //Options
    MenuData optionsMenu;
    initOptions(optionsMenu, (float)window.getSize().x, (float)window.getSize().y); //initializes options menu

    //Game

    //Password
    // (TBD)
    //Controls
    Keyboard::Key interractionButton = Keyboard::Z;

    //GameState
    GameState curState = MAIN;

    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                switch (curState)
                {
                case MAIN:
                    if (event.key.code == Keyboard::Up) {
                        up(mainMenu);
                    }
                    if (event.key.code == Keyboard::Down) {
                        down(mainMenu);
                    }
                    if (event.key.code == interractionButton) {
                        if (mainMenu.curButtonIndex == 0) {
                            curState = GAME;
                        }
                        if (mainMenu.curButtonIndex == 2) {
                            curState = OPTIONS;
                        }
                        if (mainMenu.curButtonIndex == 3) {
                            window.close();
                        }
                    }
                    break;

                case OPTIONS:
                    if (event.key.code == Keyboard::Up) {
                        up(optionsMenu);
                    }
                    if (event.key.code == Keyboard::Down) {
                        down(optionsMenu);
                    }
                    if (event.key.code == interractionButton) {
                        if (optionsMenu.curButtonIndex == 2) {
                            curState = MAIN;
                        }
                    }
                    break;

                case GAME:
                    // temp solution for entering the game (press X to return to menu)
                    // make a function to handle inputs for easier management
                    // also gotta make an ingame menu somehow for ingame stuff
                    if (event.key.code == Keyboard::X) {
                        curState = MAIN;
                    }
                    break;

                default:
                    break;
                }
            }
        }

        window.clear();

        switch (curState)
        {
        case MAIN:
            drawMenu(mainMenu, window);
            if(!logoCheck(mainMenu,window)){
                return 0;
            }; //crashes program to stop message from looping if logo is not found
            break;

        case OPTIONS:
            drawMenu(optionsMenu, window);
            break;

        case GAME:
            window.clear();
            break;

        default:
            curState = MAIN;
            break;
        }

        window.display();
    }
    return 0;
}

int charSize = 20, xOffset = -100, yOffset = 270;

//Initializes main menu
bool logoCheck(MenuData &m, RenderWindow &window) {
	if (!m.Logo.loadFromFile("C:/project/MegaManX-proj/textures/logo.png") || !m.XLogo.loadFromFile("C:/project/MegaManX-proj/textures/XLogo.png")) {
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
void drawMenu(MenuData &m, RenderWindow &window) {
    for (int i = 0; i < m.curMaxButtons; ++i) {
        window.draw(m.menuSelection[i]);
    }
}

//Menu up button function
void up(MenuData &m) {
    m.menuSelection[m.curButtonIndex--].setFillColor(m.unselectedColor);
    if (m.curButtonIndex < 0) {
        m.curButtonIndex = m.curMaxButtons - 1;
    }
    m.menuSelection[m.curButtonIndex].setFillColor(m.selectedColor);
}

//Menu down button function
void down(MenuData &m) {
    m.menuSelection[m.curButtonIndex++].setFillColor(m.unselectedColor);
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