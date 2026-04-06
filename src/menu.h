#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

#define MAX_ITEM_NO 10

struct MenuData {
    Font font;
    Text menuSelection[MAX_ITEM_NO];
    int curMaxButtons;
    int curButtonIndex;
};

// Function declarations (m is a menu struct variable, its passed by reference to avoid copying the struct and to allow us to mod the struct's data)
void initMenu(MenuData &m, float width, float height);
void initOptions(MenuData &m, float width, float height);
void drawMenu(MenuData &m, RenderWindow &window);
void up(MenuData &m);
void down(MenuData &m);