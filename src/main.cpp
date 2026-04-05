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

enum GameState { MAIN, OPTIONS, GAME };

int main()
{
    //Main Window Resolution
    const int windowWidth = 640;
    const int windowHeight = 480;

    RenderWindow window(VideoMode(windowWidth, windowHeight), "MMX prototype");

    //Menu stuff (will add sub-menus like options and PASS WORRD stuff later)
    menu menu(window.getSize().x, window.getSize().y);

    //Options
    options options(window.getSize().x, window.getSize().y);

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
            // should try out switch case for cleaner code
            if (event.type == Event::KeyPressed) {
                switch (curState)
                {
                case MAIN:
                    if (event.key.code == Keyboard::Up) {
                        menu.up();
                    }
                    if (event.key.code == Keyboard::Down) {
                        menu.down();
                    }
                    if (event.key.code == interractionButton) {
                        if (menu.ReturnButtonIndex() == 0) {
                            curState = GAME;
                        }
                        if (menu.ReturnButtonIndex() == 2) {
                            curState = OPTIONS;
                        }
                        if (menu.ReturnButtonIndex() == 3) {
                            window.close();
                        }
                    }
                    break;

                case OPTIONS:
                    if (event.key.code == Keyboard::Up) {
                        options.up();
                    }
                    if (event.key.code == Keyboard::Down) {
                        options.down();
                    }
                    if (event.key.code == interractionButton) {
                        if (options.ReturnButtonIndex() == 2) {
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

            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();

        switch (curState)
        {
        case MAIN:
            menu.draw(window);
            break;

        case OPTIONS:
            options.draw(window);
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