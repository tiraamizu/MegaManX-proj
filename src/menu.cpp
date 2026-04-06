#include "Menu.h"
#include <iostream>

using namespace std;

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