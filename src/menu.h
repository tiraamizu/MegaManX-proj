#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

#define MAX_ITEM_NO 10

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