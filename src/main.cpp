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

//Main Window Resolution
const int windowWidth = 640;
const int windowHeight = 480;
struct MenuData {
    Font font;
    Text menuSelection[MAX_ITEM_NO];
    int curMaxButtons;
    int curButtonIndex;
    Color unselectedColor = Color(83, 227, 255);
    Color selectedColor = Color(234, 138, 13);
    Texture Logo, XLogo;
    Sprite logoSprite, XLogoSprite;
    GameState curState = MAIN;
};
//megaman struct
struct player
{
	Texture megamanTexture;
	Sprite megamanSpr;

	float speed = 100.0f;
	float frameduration = 0.09f;
	float timer = 0.0f;
	int sheet_width = 216; // sprite sheet height and width 
	int sheet_height = 35;
	int frame = 6;
	int framewidth = sheet_width / frame; // each frame height and width don't ask how i calculated it 
	int frameheight = sheet_height;
	int i = 0; // our frame counter
	bool moving;
		
} playerst;
struct bullet
{
    CircleShape shape;
    float speed =5.f ;
    int direction = 1;
    bool isthere = false;

};



//bullet practice struct

// Function declarations (m is a menu struct variable, its passed by reference to avoid copying the struct and to allow us to mod the struct's data)

//1. Menu declaration functions (for main menu and options menu)
void initMenu(MenuData &m, float width, float height);
void initOptions(MenuData &m, float width, float height);
void drawMenuSelection(MenuData &m, RenderWindow &window);
void up(MenuData &m);
void down(MenuData &m);
void menuSwitchHandler(RenderWindow &window, Event &event, MenuData &m, MenuData &options, Keyboard::Key interractionButton);
bool resourcesCheck(MenuData &m);
void playerstats(player& p);
void inputhandler(player& p, float dt);
void animationhandler(player& p, float dt);

/*NOTE : m IS A FORMAL PARAMETER, IT CAN BE CALLED ANYTHING, I JUST CHOSE M FOR MENU.
THE NAMES OF THE PARAMETERS DO NOT AFFECT THE FUNCTIONALITY OF THE CODE, THEY ARE JUST PLACEHOLDERS TO MAKE THE CODE MORE READABLE.
NOTICE THAT THE INT MAIN FUNCTION CALLS ACTUALLY USE THE NAMES (ARGUMENTS) mainMenu AND optionsMenu.
*/

//2
int main()
{
    int i = 0;

    RenderWindow window(VideoMode(windowWidth, windowHeight), "MMX prototype");

    MenuData mainMenu;
    MenuData optionsMenu;

    // Load resources for both menus
    if (!resourcesCheck(mainMenu) || !resourcesCheck(optionsMenu)) {
        return 0;
        //crashes program to stop messages (in resourcesCheck) from looping if logo is not found
    };

    //Menu stuff (will add sub-menus like options and PASS WORRD stuff later)
    initMenu(mainMenu, (float)window.getSize().x, (float)window.getSize().y); //initializes menu in 1st argument, 2nd and 3rd arguments are used for calculations regarding positions of menu items.

    //Options
    initOptions(optionsMenu, (float)window.getSize().x, (float)window.getSize().y); //same as initMenu but for options menu.

    //Game
	float dt; // delta time and clock for the whole game loop
	Clock clock;
	// creating our megaman with the struct stats
	playerstats(playerst); //assigning the texture and sprites to megaman

    //Password
    // (TBD)

    //Controls
    Keyboard::Key interractionButton = Keyboard::Z;

    Event event;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();// this calculate the deltatime don't ask how:D
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                window.close();
            }
            if(event.key.code == Keyboard::Space) {
                playerst.megamanSpr.move(0, -10);
                playerst.moving = true;
            }
            menuSwitchHandler(window, event, mainMenu, optionsMenu, interractionButton);
        }

        window.clear();

        // Use mainMenu.curState to determine which menu to draw
        switch (mainMenu.curState)
        {
        case MAIN:
            window.draw(mainMenu.XLogoSprite);
            window.draw(mainMenu.logoSprite); //draws logo
            drawMenuSelection(mainMenu, window);
            break;

        case OPTIONS:
            drawMenuSelection(optionsMenu, window);
            break;

        case GAME:
            if (event.key.code == Keyboard::X) {
                mainMenu.curState = MAIN;
            }
            window.clear();
            window.draw(playerst.megamanSpr);
            animationhandler(playerst, dt);
            inputhandler(playerst, dt);

            //events of game go here
            break;
            

        default:
            mainMenu.curState = MAIN;
            break;
        }

        window.display();
    }
    return 0;
}

//FUNCTION DEFINITIONS  

int charSize = 20, xOffset = -100, yOffset = 270;

//Initializes main menu resources
bool resourcesCheck(MenuData &m) {
    if (!m.Logo.loadFromFile("textures/logo.png") || !m.XLogo.loadFromFile("textures/XLogo.png")) {
        cout << "ERR : Logo not found";
        return false;
    }
    if (!m.font.loadFromFile("fonts/mega-man-x.ttf")) {
        cout << "ERR : Font not found";
        return false;
    }
    m.XLogoSprite.setTexture(m.XLogo);
    m.XLogoSprite.setPosition(320, 130);
    m.XLogoSprite.setScale(1.5f, 1.5f);
    m.logoSprite.setTexture(m.Logo);
    m.logoSprite.setPosition(150, 130);
    m.logoSprite.setScale(1.5f, 1.5f);
    return true;
}
//Initializes main menu (note: we pass width and height by value bc we are using them for a calculation, no need to mod them.)
void initMenu(MenuData &m, float width, float height) {
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

//main is a placeholder for mainmenu, options is a placeholder for options menu.
//this function handles switching between menus and the game, it also handles the menu interraction button (Z in this case) for both menus and the game.
void menuSwitchHandler(RenderWindow &window, Event &event, MenuData &main, MenuData &options, Keyboard::Key interractionButton) {
    if (event.type == Event::KeyPressed) {
        switch (main.curState)
        {
        case MAIN:
            if (event.key.code == Keyboard::Up) {
                up(main);
            }
            if (event.key.code == Keyboard::Down) {
                down(main);
            }
            if (event.key.code == interractionButton) {
                if (main.curButtonIndex == 0) {
                    main.curState = GAME;
                }
                if (main.curButtonIndex == 2) {
                    main.curState = OPTIONS;
                }
                if (main.curButtonIndex == 3) {
                    window.close();
                }
            }
            break;

        case OPTIONS:
            if (event.key.code == Keyboard::Up) {
                up(options);
            }
            if (event.key.code == Keyboard::Down) {
                down(options);
            }
            if (event.key.code == interractionButton) {
                if (options.curButtonIndex == 2) {
                    main.curState = MAIN;
                }
            }
            break;

        case GAME:
            if (event.key.code == Keyboard::X) {
                main.curState = MAIN;
            }
            break;

        default:
            break;
        }
    }
}

//~~~~~~~~~~~~~~intialize !!megaman texture and sprite function~~~~~~~~~~~~~~~~~~~~~~
void playerstats(player& playerst) // p for better writing :D
{
	playerst.megamanTexture.loadFromFile("textures/shit.jpeg");
	playerst.megamanSpr.setTexture(playerst.megamanTexture); //assigning the texture to the sprite so that we can use it in the game loop
	playerst.megamanSpr.setPosition(windowWidth/2, windowHeight/2);
	playerst.megamanSpr.setScale(2.0f, 2.0f);  
	playerst.megamanSpr.setTextureRect(IntRect(0, 0, playerst.framewidth, playerst.frameheight));//start with the first frame of the sprite sheet
	//note we will change this if we want to make a standing animation
	playerst.megamanSpr.setOrigin(playerst.framewidth	 / 2.0f, playerst.frameheight / 2.0f);	
}
//~~~~~~~~~~~~~~~megaman buttons and input handler~~~~~~~~~~~~~~~~~~~~~~
void inputhandler(player& playerst, float dt)
{
	playerst.moving = false;

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		playerst.megamanSpr.move(playerst.speed * dt, 0);// to calculate distance moved for each frame
		playerst.megamanSpr.setScale(2.0f, 2.0f); // the scale to make the character face which direction we want
		// note the ngeative direction changes based on the TEXTURE direction which we implemented
		playerst.moving = true;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		//distance covered
		playerst.megamanSpr.move(-playerst.speed * dt, 0);
		playerst.megamanSpr.setScale(-2.0f, 2.0f); // negative to make the sprite face the other direction
		playerst.moving = true;
	}
	else
		// this condition is made only when the button isn't clicked we conclude that the character is idle and not moving
		// so we resets the frames and the timer to start again whenever the player click the bottun:D
	{

		playerst.i = 0;
		playerst.timer = 0.0f;
	}

}
//~~~~~~~~~~~~~~~~megaman frames and deltatime handler~~~~~~~~~~~~~~~~~~~~~~
void animationhandler(player& playerst, float dt)
{
	if (playerst.moving)//this if condition is wrote so that when the player click any movement botton the code reads it so that the 
		// frames update
	{
	    playerst.timer += dt; // this line add the  fraction of time we pressed on the button to our timer so
		//that each frame update is presicely know or determined according to our frame duration constant
		// btw we assinged the timer to be each 0.0f this means it will act as a real timer and reads every 0.1 s the player
		// click on the button


		if (playerst.timer >= playerst.frameduration)// this condition is only made for the time being because since we only now have 6 frmaes 
			// and each frame frameduartion of 0.1 , this means that each time the timer goes up by 0.1 the the frame is updated 
			// and the game loop after 0.1 s will display the new frame and in out case since we only have 6 frames then after
			// each 0.6 s the frame loop resets  and that's what this condition is saying :DD
		{
			playerst.timer = 0;
			playerst.i++;
			if (playerst.i >= 6)
				playerst.i = 0;
			playerst.megamanSpr.setTextureRect(IntRect(playerst.i * playerst.framewidth, 0, playerst.framewidth, playerst.frameheight));
		}
	}
	
}