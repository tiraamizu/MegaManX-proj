#include "Menu.h"
#include <iostream>
using namespace std;
int charSize = 20, xOffset = 1, yOffset = 220;
menu::menu(float width, float height) {
	if (!font.loadFromFile("fonts/mega-man-x.ttf")) {
		cout << "Font not found";
	}
	curMaxButtons = 4;
	//GAME START
	menuSelection[0].setFont(font);
	menuSelection[0].setFillColor(Color::Yellow);
	menuSelection[0].setString("GAME START");
	menuSelection[0].setCharacterSize(charSize);
	menuSelection[0].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 1) * 1) + yOffset);
	//PASS WORD
	menuSelection[1].setFont(font);
	menuSelection[1].setFillColor(Color::Cyan);
	menuSelection[1].setString("PASS WORD");
	menuSelection[1].setCharacterSize(charSize);
	menuSelection[1].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 1) * 1.25) + yOffset);
	//OPTION MODE
	menuSelection[2].setFont(font);
	menuSelection[2].setFillColor(Color::Cyan);
	menuSelection[2].setString("OPTION MODE");
	menuSelection[2].setCharacterSize(charSize);
	menuSelection[2].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 1) * 1.5) + yOffset);
	//TERMINATE
	menuSelection[3].setFont(font);
	menuSelection[3].setFillColor(Color::Cyan);
	menuSelection[3].setString("TERMINATE");
	menuSelection[3].setCharacterSize(charSize);
	menuSelection[3].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 1) * 1.75) + yOffset);

	//Variable for checking selected button
	curButtonIndex = 0;
}
menu::~menu() {

}
	//Draw menu
	void menu::draw(RenderWindow & window) {
		for (int i = 0; i < curMaxButtons; ++i) {
			window.draw(menuSelection[i]);
		}
	}
	void menu::up() {
		menuSelection[curButtonIndex].setFillColor(Color::Cyan);
		curButtonIndex--;
		if (curButtonIndex < 0) {
			curButtonIndex = curMaxButtons - 1;
		}
		menuSelection[curButtonIndex].setFillColor(Color::Yellow);
	}
	void menu::down() {
		menuSelection[curButtonIndex].setFillColor(Color::Cyan);
		curButtonIndex++;
		if (curButtonIndex >= curMaxButtons) {
			curButtonIndex = 0;
		}
		menuSelection[curButtonIndex].setFillColor(Color::Yellow);
		
	}


	options::options(float width, float height) : menu(width, height) {
		curMaxButtons = 3;
		//CONTROLS
		menuSelection[0].setString("CONTROLS");
		//AUDIO
		menuSelection[1].setString("AUDIO");
		//BACK
		menuSelection[2].setString("BACK");

		//Variable for checking selected button
		curButtonIndex = 0;
	}