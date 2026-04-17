#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>

#define MAX_ITEM_NO 10
#define nbullets 10// number of bullets that the window can show , not the magazine
const float gravity = 0.5f;

using namespace std;
using namespace sf;

enum GameState { MAIN, OPTIONS, GAME };

//STRUCTS

//Main Window Resolution
const int windowWidth = 640;
const int windowHeight = 480;
SoundBuffer buffer; // declarations for sfx
Sound sound;


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
View camera(FloatRect(0, 0, windowWidth, windowHeight));
struct player
{
	Texture megamanTexture;
	Sprite megamanSpr;

	float Vx = 100.0f;
    float Vy = 0.0f;
	float frameduration = 0.05f;
	float timer = 0.0f;
	int sheet_width = 216; // sprite sheet height and width 
	int sheet_height = 35;
	int frame = 6;
	int framewidth = sheet_width / frame; // each frame height and width don't ask how i calculated it 
	int frameheight = sheet_height;
	int i = 0; // our frame counter
	bool moving;
    bool isground = false;
    float jumpstrength = -200.f;
		
} playerst;
struct enemy
{
	Texture megamanTexture;
	Sprite megamanSpr;

	float Vx = 100.0f;
    float Vy = 0.0f;
	float frameduration = 0.05f;
	float timer = 0.0f;
	int sheet_width = 216; // sprite sheet height and width 
	int sheet_height = 35;
	int frame = 6;
	int framewidth = sheet_width / frame; // each frame height and width don't ask how i calculated it 
	int frameheight = sheet_height;
	int i = 0; // our frame counter
	bool moving;
    bool isground = false;
		
} dEnemy;
struct bullet
{
    RectangleShape shape;
    float speed =1.f ;

    Vector2f bullet2D;
    
    int direction = 1;  
    bool isthere = false;//this condition  helps us when we are using the struct array to know if the slot has a bullet in it or an empty bullet 
    //if there is a bullet in the slot the loop will skip it , if it found an empty slot and the player clicked on the fire button it will
    // make the slot has a bullet , to sum it up it create the bullet
};

struct groundobj
{
    RectangleShape gnd;
    int blockwidth = 200.f;
    int blockheight = 100.f;

}ground;

// Function declarations (m is a menu struct variable, its passed by reference to avoid copying the struct and to allow us to mod the struct's data)

//1. Menu declaration functions (for main menu and options menu)
void initMenu(MenuData &m, float width, float height);
void initOptions(MenuData &m, float width, float height);
void drawMenuSelection(MenuData &m, RenderWindow &window);
void up(MenuData &m);
void down(MenuData &m);
void menuSwitchHandler(RenderWindow &window, Event &event, MenuData &m, MenuData &options, Keyboard::Key interractionButton);
bool resourcesCheck(MenuData &m);
void playerstats(player& playerst);
void inputhandler(player& playerst, float dt , bullet windowmag[]);
void animationhandler(player& playerst, float dt);
void bulletstates(bullet& prj);
void groundInit(groundobj& grcollision  );
void handleIntersection(bool& isground);
void groundinit(groundobj& grcollision, RenderWindow& window);
void Gravity(player& playerst, float &dt);

/*NOTE : m IS A FORMAL PARAMETER, IT CAN BE CALLED ANYTHING, I JUST CHOSE M FOR MENU.
THE NAMES OF THE PARAMETERS DO NOT AFFECT THE FUNCTIONALITY OF THE CODE, THEY ARE JUST PLACEHOLDERS TO MAKE THE CODE MORE READABLE.
NOTICE THAT THE INT MAIN FUNCTION CALLS ACTUALLY USE THE NAMES (ARGUMENTS) mainMenu AND optionsMenu.
*/

//2

void handleIntersection(player& playerst , float &dt) {
  // Platfrom-Player
  if (playerst.megamanSpr.getGlobalBounds().intersects(ground.gnd.getGlobalBounds())) {
    // Set player vy = 0;
    playerst.Vy = 0;
    playerst.isground = true;
  }
  else 
  {

    playerst.Vy += gravity * dt;
    playerst.isground = false;
  }

  // Wall-Player : Set player vx = 0;

  // Enemy-player : Make the player get hit

  // Enemy-Platform

  // Enemy-Wall
}

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

    //~~~~~~~~~~~~Game~~~~~~~~~~~~~~~
    //assigning the texture and sprites to megaman
	float dt; // delta time and clock for the whole game loop
	Clock clock;
	// creating our megaman with the struct stats
	playerstats(playerst);
    bullet windowmag[nbullets];//creating the array of struct 
    //this struct helps us in alot of functions , first it helps us in creating 10 bullets without writing 10 line of codes for each one
    // also this helps us to nulify the bullets that hit the boarder wihout needing to do this 10 times
    //just using this global array and editting it in any loop we want

    
    for(int i =0 ; i<nbullets ; i++)
    {
        bulletstates(windowmag[i]);//this set up all the empty bullets with all of the bulletstates intializations
    }

    //Password
    // (TBD)

    //Controls
    Keyboard::Key interractionButton = Keyboard::Z;

    Event event;
    while (window.isOpen())
    {
        camera.setCenter(playerst.megamanSpr.getPosition()); //sets camera to follow the player
        dt = clock.restart().asSeconds();// this calculate the deltatime don't ask how:D
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) 
            {
                window.close();
            }
            //jump  
            if(event.key.code == Keyboard::Space && playerst.isground) 
            {
                playerst.Vy = playerst.jumpstrength;
                playerst.isground = false;
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bullet firing code
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::A)
            {
                for(int i = 0 ; i < nbullets ; i++)
                {
                    if(!windowmag[i].isthere)//here we are scaning if the slot of the struct array already has a bullet and fired ot empty?
                    {
                        // of the slot is empty and the player pressed 'z' then these next line of codes relode this bullet and fire it 
                        //from the postion of the character and in horizontal and vertical position , since our character can jump while jumping:D
                        windowmag[i].isthere = true;    // x positon                                    y position
                        windowmag[i].shape.setPosition(playerst.megamanSpr.getPosition().x,playerst.megamanSpr.getPosition().y);//changed
                        //this because the y = 0 will always spawn the ballet in a different location if megaman y is different
                        if(playerst.megamanSpr.getScale().x > 0)//this check the character direction by the x scale we wrote above
                        {
                            windowmag[i].direction = 1;
                        }
                        else
                        {
                            windowmag[i].direction = -1;
                        }
                        if(!buffer.loadFromFile("sounds/shoot.wav"))
                        { 
                            cout<<"Error! Couldn't load sound files"<<endl;
                        }
                        
                        sound.setBuffer(buffer);
                        sound.play();
                        break;
                        // this is the most IMPORTANT line here i hope you know why:D
                        //look if this break wasn't here because this loop must on;y trigger once it found the 
                        //the first empty slot and then it will be fired in the switch case above ,
                        //the main problem here if the loop continues it will fill all of the empty slots with bullets 
                        // then we the the character fire it will fire >> 10 bullets but in the same pixel moving with same direction
                        // we won't see it but it will make the collusion with the enemy a hell:DDDD

                    }   
                    
                }
            }
            menuSwitchHandler(window, event, mainMenu, optionsMenu, interractionButton);
        }

        window.clear();

        // Use mainMenu.curState to determine which menu to draw
        switch (mainMenu.curState)
        {
        case MAIN:
        window.setView(window.getDefaultView()); //sets camera to default view rather than where the player WAS.
            window.draw(mainMenu.XLogoSprite);
            window.draw(mainMenu.logoSprite); //draws logo
            drawMenuSelection(mainMenu, window);
            break;

        case OPTIONS:
            drawMenuSelection(optionsMenu, window);
            break;

        case GAME:
            window.setView(camera);
            if (event.key.code == Keyboard::X) 
            {
                mainMenu.curState = MAIN;
            }
            inputhandler(playerst, dt,windowmag); 
            animationhandler(playerst, dt);
            groundInit(ground);
            window.draw(ground.gnd);
            Gravity(playerst, dt);
            handleIntersection(playerst, dt);
          
            //window.clear(); is this redundent?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bullet movement update
            for(int i = 0 ; i < nbullets ; i++)// this loop after the game loop above so that after we determined the bullet and said to shoot
            //this loop reads the array and locate the slot we want to fire , then it fire it and reset this bullet condition or bool
            {
                if(windowmag[i].isthere)
                {
                    windowmag[i].shape.move(windowmag[i].speed* windowmag[i].direction, 0);// here is the FIRING 
                    //this function does fire the bullet in the direction that we condifiermed in the directionfunction inside the inputhandler  
                    
                    //so that the bullet fire in the direction the character is  facing 

                    //the code below make each bullet when it hit the boundary it will reset so that it won't continue to move in the background //~~~~~~~~~~~~~~~~~
                    //and consume alot more resources each bullet
                    if (windowmag[i].shape.getPosition().x > playerst.megamanSpr.getPosition().x + windowWidth  || windowmag[i].shape.getPosition().x < playerst.megamanSpr.getPosition().x - windowWidth) {
                
                        windowmag[i].isthere = false;
                    }
                    
                    window.draw(windowmag[i].shape);
                    window.setKeyRepeatEnabled(false);// this built-in event works when the player hold the button 
                    //and don't release it but we want the action to happen only once in this for loop
                    
                }
            }   
            window.draw(playerst.megamanSpr);


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
void inputhandler(player& playerst, float dt ,bullet windowmag[])
{
	playerst.moving = false;

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		playerst.megamanSpr.move(playerst.Vx * dt, 0);// to calculate distance moved for each frame
		playerst.megamanSpr.setScale(2.0f, 2.0f); // the scale to make the character face which direction we want
		// note the ngeative direction changes based on the TEXTURE direction which we implemented
		playerst.moving = true;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		//distance covered
		playerst.megamanSpr.move(-playerst.Vx * dt, 0);
		playerst.megamanSpr.setScale(-2.0f, 2.0f); // negative to make the sprite face the other direction
		playerst.moving = true;
	}
    
    else//idle (state movement)
    {

		playerst.i = 0;
		playerst.timer = 0.0f;
        playerst.megamanSpr.setTextureRect(IntRect(0, 0, playerst.framewidth, playerst.frameheight));
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
void bulletstates(bullet& prj)
{
    prj.bullet2D.x = 20;
    prj.bullet2D.y = 20;
    prj.shape.setSize(prj.bullet2D);
    prj.shape.setFillColor(Color::Red);
    //prj.isthere =false; // leave if for future bec. this will help us if we add a button to rest the level
    // due to this button resets the condition after every loob not the struct

}
void groundInit(groundobj& grcollision  )
{
    grcollision.gnd.setSize(Vector2f(grcollision.blockwidth, grcollision.blockheight));
    grcollision.gnd.setFillColor(Color::Green);
    grcollision.gnd.setPosition(200, 400);
    grcollision.gnd.setOrigin(grcollision.blockwidth/2, grcollision.blockheight/2);
}
void Gravity(player& playerst, float &dt)
{            
            playerst.megamanSpr.move(0, playerst.Vy * dt);
              if(!playerst.isground){
                    playerst.Vy += gravity;
                }
                else{
                    playerst.Vy = 0;
                }
};