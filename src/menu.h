#pragma once
#include <SFML/Graphics.hpp>

#define MAX_ITEM_NO 4

using namespace sf;

class menu
{
public:
	menu(float width, float height);
	~menu();

	void draw(sf::RenderWindow& window);
	void up();
	void down();
	int ReturnButtonIndex() {
		return curButtonIndex;
	}
protected:
	int curButtonIndex;
	int curMaxButtons;
	Font font;
	Text menuSelection[MAX_ITEM_NO];
};
class options : public menu
{
public:
	options(float width, float height);
	~options() {};

};