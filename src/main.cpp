#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Menu.h"

using namespace std;
using namespace sf;

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