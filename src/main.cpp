#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <sstream>
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
    // (TBA)
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
                if (curState == MAIN) {
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
                }
                else if (curState == OPTIONS) {
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
                }
                // temp solution for entering the game (press X to return to menu)
                // make a function to handle inputs for easier management
                // also gotta make an ingame menu somehow for ingame stuff
                else if (curState == GAME) {
                    if (event.key.code == Keyboard::X) {
                        curState = MAIN;
                    }
                }

            }
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear();
        if (curState == MAIN) {
            menu.draw(window);
        }
        if (curState == OPTIONS) {
            options.draw(window);
        }
        if (curState == GAME) {
            window.clear();
        }
        window.display();
    }
    return 0;
}