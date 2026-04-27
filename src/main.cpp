#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;
using namespace sf;
#define MAX_ITEM_NO 10
#define nbullets 10// number of bullets that the window can show , not the magazine
const float gravity = 1000.f;
const int blocks = 100;
const float ratio_health=3.84;

enum GameState { MAIN, OPTIONS, GAME };

//STRUCTS

//Main Window Resolution
const float windowWidth = 640;
const float windowHeight = 480;
//player hitbox
const Vector2f mega_hitbox_size(55.f,60.f); //reminder to change this later depending on megaman's sprite,

struct map {
Sprite mapSpr;
Texture mapTexture;

Sprite backgroundSpr[100];
Texture backgroundTexture;

Sprite lampSpr;
Texture lampTexture;

Sprite backgroundSpr2;
Texture backgroundTexture2;

Sprite car1Spr;
Texture car1;

Sprite car2Spr;
Texture car2;
} map1;
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
View menuCamera(FloatRect(0, 0, windowWidth, windowHeight));
View healthUI(FloatRect(0, 0, windowWidth, windowHeight));
struct player
{
	Texture megamanTexture;
	Sprite megamanSpr;
    RectangleShape hitbox; //for every interaction EXCEPT ground and wall jump
    Texture healthbar_text;
    RectangleShape healthbar;
    float Vx = 500.f;
    float Vy = 0.0f;
    float inv_timer=3.0;
    Vector2f Pos_Tracker; 
	float frameduration = 0.05f;
	float timer = 0.0f;
	int sheet_width = 216; // sprite sheet height and width 
	int sheet_height = 35;
	int frame = 6;
	int framewidth = sheet_width / frame; // each frame height and width don't ask how i calculated it 
	int frameheight = sheet_height;
	int i = 0; // our frame counter
  bool invincible=true;
  enum dir {NONE,LEFT,RIGHT};
  dir toucheswall = NONE; 
  bool moving = false;
  bool touchesground = false;
  bool issliding = false;
  float jumpstrength = -400.f;
  int health =19; //max hp is 19
		
} playerst;
struct enemy {
    Texture enemyTexture;
    Sprite enemySpr;

    bool touchesground = true;
    float detectionRange = 350.f;
    bool isActive = false; // the range at which the enemy will detect the player and start moving towards him
    bool alive = true;
    float timer= 0.0f;
    float frameduration = 0.15f;
    int framewidth = 50; // each frame height and width don't ask how i calculated it 
	int frameheight = 65;
    int eIndex = 0;
    bool goingForward = true; // ping-pong direction flag
    bool hasFired    = false;
} dEnemy; 
struct bullet
{
    RectangleShape shape;
    float speed =500.f ;
    Vector2f bullet2D;
    int direction = 1;  
    bool isthere = false;//this condition  helps us when we are using the struct array to know if the slot has a bullet in it or an empty bullet 
    //if there is a bullet in the slot the loop will skip it , if it found an empty slot and the player clicked on the fire button it will
    // make the slot has a bullet , to sum it up it create the bullet
}prj;
struct enemybullet
{

    RectangleShape shape;
    float speed =800.f ;
    Vector2f bullet2D;
    int direction = 1;  
    bool isthere = false;
    


}dEnemyBullet;
struct groundobj
{
    RectangleShape gnd;
    int blockwidth = 200.f;
    int blockheight = 100.f;

}ground[blocks];

// Function declarations (m is a menu struct variable, its passed by reference to avoid copying the struct and to allow us to mod the struct's data)

//1. Menu declaration functions (for main menu and options menu)
void initMenu(MenuData &m, float width, float height);
void initOptions(MenuData &m, float width, float height);
void drawMenuSelection(MenuData &m, RenderWindow &window);
void up(MenuData &m);
void down(MenuData &m);
void menuSwitchHandler(RenderWindow &window, Event &event, MenuData &m, MenuData &options, Keyboard::Key interractionButton);
bool resourcesCheck(MenuData &m);
void camBounds(float LeftOffset, float RightOffset, float UpOffset, float DownOffset);

/*NOTE : m IS A FORMAL PARAMETER, IT CAN BE CALLED ANYTHING, I JUST CHOSE M FOR MENU.
THE NAMES OF THE PARAMETERS DO NOT AFFECT THE FUNCTIONALITY OF THE CODE, THEY ARE JUST PLACEHOLDERS TO MAKE THE CODE MORE READABLE.
NOTICE THAT THE INT MAIN FUNCTION CALLS ACTUALLY USE THE NAMES (ARGUMENTS) mainMenu AND optionsMenu.
*/

//2. Game function declarations
void playerstats(player& playerst);
void bulletstates(bullet& prj);
void enemystatus(enemy& dEnemy);
void enemybulletstatus(enemybullet& dEnemyBullet);
void shooting(enemybullet& dEnemyBullet,player& playerst, float dt , enemy& dEenmy);
void update(player& playerst ,enemybullet& dEnemyBullet , float dt );
void enemydetection(enemy& dEnemy , player& playerst);
void health_blockout(player& playerst,RectangleShape& blackout);
void inputhandler(player& playerst, float dt );
void animationhandler(player& playerst, float dt);
void enemyAnimation(enemy& dEnemy, float dt);

void Gravity(player& playerst, float &dt);
void createBlock(int index, float x, float y, float width, float height);
void playerhitbox_pos(player& playerst);
void check_invincibility(player& playerst,float dt);
void handleIntersection(player& playerst , float &dt);


View aspectRatio(View view, float windowWidth, float windowHeight);
void carMovement(Sprite& car, float carSpeed, float dt);
//temp functions
void damage (player& playerst);
void heal (player& playerst);

int main()
{   //blackout that blocks healthbar to make it seem gone
    RectangleShape blackout;
    blackout.setFillColor(Color::Black);
    blackout.setSize(Vector2f(15,1));
    blackout.setPosition(31.f,141.f);
    
    int i = 0;
    int eindex = 0;
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
    enemystatus(dEnemy);
    enemybulletstatus(dEnemyBullet);
    bullet windowmag[nbullets];
    for(int i =0 ; i<nbullets ; i++)
    {
        bulletstates(windowmag[i]);//this set up all the empty bullets with all of the bulletstates intializations
    }//creating the array of struct 
    //this struct helps us in alot of functions , first it helps us in creating 10 bullets without writing 10 line of codes for each one
    // also this helps us to nulify the bullets that hit the boarder wihout needing to do this 10 times
    //just using this global array and editting it in any loop we want

    //LOADING THE MAP

    createBlock(0, 310, 380, 1490, 100);
    createBlock(1, 1890, 320, 930, 100);
    createBlock(2, 2900, 360, 1985, 100);
    createBlock(3, 4965, 360, 1050, 100);
    createBlock(4, 6100, 410, 1540, 100);
    createBlock(5, 7765, 410, 125, 100);
    createBlock(6, 7955, 360, 260, 100);
    createBlock(7, 8335, 360, 390, 100);
    createBlock(8, 8850, 290, 125, 100);
    createBlock(9, 9045, 290, 125, 100);
    createBlock(10, 9245, 290, 430, 100);
    createBlock(11, 9815, 345, 125, 100);
    createBlock(12, 10008, 420, 792, 100);
    createBlock(13, 10900, 355, 575, 100);
    createBlock(14, 11640, 420, 887, 100);
    createBlock(15, 12687, 520, 2883, 100);
    


    //Password
    // (TBD)

    //Controls
    Keyboard::Key interractionButton = Keyboard::Z;
    Event event;
    while (window.isOpen())
    {

        dt = clock.restart().asSeconds();// this calculate the deltatime don't ask how:D
        if (dt > 0.05f) {
        dt = 0.05f; 
        
        } // dt limiter to prevent lagspikes from breaking some game stuff
        while (window.pollEvent(event))
        //aspect ratio logic
        {
            if (event.type == Event::Resized) 
            {
                camera = aspectRatio(camera, event.size.width, event.size.height);
                menuCamera = aspectRatio(menuCamera, event.size.width, event.size.height);
                healthUI = aspectRatio(healthUI, event.size.width, event.size.height); 
            }
            if (event.type == Event::Closed) 
            {
                window.close();
            }
            //jump  
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space && (playerst.touchesground || playerst.issliding)) 
            {
                playerst.Vy = playerst.jumpstrength;
                playerst.touchesground = false;
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
        aspectRatio(menuCamera, window.getSize().x, window.getSize().y);
        window.setView(menuCamera);
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
            if (event.key.code == Keyboard::C) 
            {
                playerst.megamanSpr.setPosition(windowWidth/2, windowHeight/2);
                playerst.hitbox.setPosition(playerst.megamanSpr.getPosition());
            } //debugging
            inputhandler(playerst, dt); 
            handleIntersection(playerst, dt);
            animationhandler(playerst, dt);
            Gravity(playerst, dt);
            enemyAnimation(dEnemy , dt);
            shooting(dEnemyBullet, playerst, dt , dEnemy);
            camBounds(70, 40, 40, 60);
            carMovement(map1.car1Spr, -200.f, dt);
            carMovement(map1.car2Spr, -200.f, dt);

          
            //window.clear(); is this redundent?
            window.draw(map1.backgroundSpr[0]);
            window.draw(map1.mapSpr);
            window.draw(dEnemy.enemySpr);
            window.draw(dEnemyBullet.shape);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bullet movement update
            for(int i = 0 ; i < nbullets ; i++)// this loop after the game loop above so that after we determined the bullet and said to shoot
            //this loop reads the array and locate the slot we want to fire , then it fire it and reset this bullet condition or bool
            {
                if(windowmag[i].isthere)
                {
                    windowmag[i].shape.move(windowmag[i].speed* windowmag[i].direction * dt, 0);// here is the FIRING 
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
            playerst.Pos_Tracker=playerst.megamanSpr.getPosition();//tracks position of megaman
            playerhitbox_pos(playerst); //constnatly updates hitbox to be on megaman
            window.draw(playerst.megamanSpr);
            window.draw(playerst.hitbox);
            window.draw(map1.car1Spr);
            window.draw(map1.car2Spr);
            //constants on the screen (aka just health), dont do .draw under it or else it wont function the same as you want
            // if you wanna do .draw   make sure to reuse window.setView(camera); and then use window.draw under it
            window.setView(healthUI);
            health_blockout(playerst,blackout);
            window.draw(playerst.healthbar);
            window.draw(blackout);


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
    if (!map1.mapTexture.loadFromFile("textures/map.png")) {
    cout << "ERROR: Could not find textures/map.png" << endl;
        return false;
    }
    if (!m.Logo.loadFromFile("textures/logo.png") || !m.XLogo.loadFromFile("textures/XLogo.png")) {
        cout << "ERR : Logo not found";
        return false;
    }
    if (!m.font.loadFromFile("fonts/mega-man-x.ttf")) {
        cout << "ERR : Font not found";
        return false;
    }
    if (!map1.backgroundTexture.loadFromFile("textures/background1.png")) {
    cout << "ERR : Background not found";
    return false;
    }
    if (!map1.car1.loadFromFile("textures/car1.png")) {
        cout << "ERR : Car 1 not found";
        return false;
    }
    if (!map1.car2.loadFromFile("textures/car2.png")) {
        cout << "ERR : Car 2 not found";
        return false;
    }
    m.XLogoSprite.setTexture(m.XLogo);
    m.XLogoSprite.setPosition(320, 130);
    m.XLogoSprite.setScale(1.5f, 1.5f);
    m.logoSprite.setTexture(m.Logo);
    m.logoSprite.setPosition(150, 130);
    m.logoSprite.setScale(1.5f, 1.5f);

    map1.mapSpr.setTexture(map1.mapTexture);
    map1.mapSpr.setPosition(200, 0);
    map1.mapSpr.setScale(2.0f, 2.0f);

    map1.backgroundSpr[0].setTexture(map1.backgroundTexture);
    map1.backgroundTexture.setRepeated(true);
    map1.backgroundSpr[0].setPosition(0, 42);
    map1.backgroundSpr[0].setScale(2.0f, 2.0f);
    map1.backgroundSpr[0].setOrigin(0, 0);
    map1.backgroundSpr[0].setTextureRect(IntRect(0, 0, 15400, map1.backgroundTexture.getSize().y));

    map1.car1Spr.setTexture(map1.car1);
    map1.car1Spr.setPosition(500, 230);
    map1.car1Spr.setScale(2.0f, 2.0f);

    map1.car2Spr.setTexture(map1.car2);
    map1.car2Spr.setPosition(1000, 275);
    map1.car2Spr.setScale(2.0f, 2.0f);
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
    playerst.hitbox.setFillColor(Color::Transparent);
    playerst.hitbox.setSize(mega_hitbox_size);
    playerst.hitbox.setOrigin(mega_hitbox_size.x/2,mega_hitbox_size.y/2);
    playerst.healthbar.setSize(Vector2f(30,55));
    playerst.healthbar.setPosition(20,135);
    playerst.healthbar.scale(1.25f,2.f);
    playerst.healthbar_text.loadFromFile("textures/healthbar.png");
    playerst.healthbar.setTexture(&playerst.healthbar_text);
}
//~~~~~~~~~~~~~~~Put the rectangle on megaman~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void playerhitbox_pos(player& playerst){
    playerst.hitbox.setPosition(playerst.Pos_Tracker);
}
//~~~~~~~~~~~~~~~megaman buttons and input handler~~~~~~~~~~~~~~~~~~~~~~
void inputhandler(player& playerst, float dt )
{
  playerst.moving = false;

  if (Keyboard::isKeyPressed(Keyboard::Right) && playerst.toucheswall != playerst.RIGHT) {
    playerst.megamanSpr.move(playerst.Vx * dt, 0);// to calculate distance moved for each frame
    playerst.megamanSpr.setScale(2.0f, 2.0f); // the scale to make the character face which direction we want
                                              // note the ngeative direction changes based on the TEXTURE direction which we implemented
    playerst.moving = true; 
  }
  else if (Keyboard::isKeyPressed(Keyboard::Left) && playerst.toucheswall != playerst.LEFT)	{
    //distance covered
    playerst.megamanSpr.move(-playerst.Vx * dt, 0);
    playerst.megamanSpr.setScale(-2.0f, 2.0f); // negative to make the sprite face the other direction
    playerst.moving = true; 
  }
  else { 
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
    prj.shape.setOrigin(prj.shape.getGlobalBounds().width/2 , prj.shape.getGlobalBounds().height/2);
    prj.shape.setSize(prj.bullet2D);
    prj.shape.setFillColor(Color::Red);
    //prj.isthere =false; // leave if for future bec. this will help us if we add a button to rest the level
    // due to this button resets the condition after every loob not the struct

}
void Gravity(player& playerst, float &dt)
{            
  playerst.megamanSpr.move(0, playerst.Vy *dt);
  if (playerst.issliding) {
    playerst.Vy += gravity*0.1*dt;
  }
  else if(!playerst.touchesground) {
    playerst.Vy += gravity * dt; //vf = vi + at for proper gravity that depends on dt to streamline everything.
  }
  else {
    playerst.Vy = 0;
  }
};
void createBlock(int index, float x, float y, float width, float height) {
    ground[index].blockwidth = width;
    ground[index].blockheight = height;
    x += width / 2.0f;

    ground[index].gnd.setSize(Vector2f(width, height));
        
    ground[index].gnd.setOrigin(width / 2.0f, height / 2.0f);

    ground[index].gnd.setPosition(x, y);
}

// Platform Function syntax: createBlock(block you want to start from (Starting block), number of blocks you want to add starting from aforementioned Starting Block, startingXPosition, yPos, Spacing Between Blocks);

void check_invincibility(player& playerst,float dt){
    if (playerst.invincible==true &&playerst.inv_timer>=0)
    {
        playerst.inv_timer=playerst.inv_timer-dt;
    }
    else{
        playerst.invincible=false;
        playerst.inv_timer=3.0f;
    }
    
}

int checkWallIntersection(int ind) {
  // Checks if the player are on the same x-axis of the ground, then they intersect vertically. Otherwise, they intersect horizontally
  // 1 = Player's left touches the wall, 2 = Player's right touches the wall
  auto start = ground[ind].gnd.getPosition().x-0.5*ground[ind].blockwidth;
  auto end = ground[ind].gnd.getPosition().x+0.5*ground[ind].blockwidth;
  if (playerst.Pos_Tracker.x-end > 0.5*playerst.framewidth) return 1;
  if (start-playerst.Pos_Tracker.x > 0.5*playerst.framewidth) return 2;
  return 0;
}

void handleIntersection(player& playerst , float &dt) {
  playerst.touchesground = false;
  playerst.toucheswall = player::NONE;
  playerst.issliding = false;
  // Platfrom-Player
  for(int i = 0 ; i < blocks ; i++)
  {
    if (playerst.hitbox.getGlobalBounds().intersects(ground[i].gnd.getGlobalBounds())) 
    {
      // Wall-Player : Set player vx = 0;
      if (checkWallIntersection(i) == 1) {
        playerst.toucheswall = player::LEFT;
      }
      else if (checkWallIntersection(i) == 2) {
        playerst.toucheswall = player::RIGHT;
      }
      else if (playerst.Vy >= 0)
      {
        playerst.touchesground = true;
        // Set player vy = 0;
      }
    }
  }
  
  // Sliding
  if (playerst.touchesground == false && playerst.toucheswall != player::NONE) playerst.issliding = true;

  // Enemy-player : Make the player get hit

  // Enemy-Platform

  // Enemy-Wall

}
void enemystatus(enemy& denemy)
{
 denemy.enemySpr.setPosition( 1053.89f , 220.f );
 denemy.enemyTexture.loadFromFile("textures/enemies_full2.png");
 denemy.enemySpr.setTexture(denemy.enemyTexture); //assigning the texture to the sprite so that we can use it in the game loop
 denemy.enemySpr.setOrigin(denemy.framewidth/ 2.0f, denemy.frameheight / 2.0f);	
 denemy.enemySpr.setScale(4.0f, 4.0f);  
denemy.enemySpr.setTextureRect(IntRect(0, 0, denemy.framewidth, denemy.frameheight));

}
void enemydetection(enemy& dEnemy , player& playerst)
{

    if(playerst.megamanSpr.getPosition().x <dEnemy.enemySpr.getPosition().x + dEnemy.detectionRange && playerst.megamanSpr.getPosition().x > dEnemy.enemySpr.getPosition().x - dEnemy.detectionRange)
    {
        dEnemy.isActive = true;
        if(playerst.megamanSpr.getPosition().x< dEnemy.enemySpr.getPosition().x)
        {
            dEnemy.enemySpr.setScale(4.f , 4.f);
        }
        else
        {
            dEnemy.enemySpr.setScale(-4.f , 4.f);
        }
    }  
    else
    {
        dEnemy.isActive = false;
    }

}   
void enemybulletstatus(enemybullet& dEnemyBullet)
{
    dEnemyBullet.bullet2D.x = 20;
    dEnemyBullet.bullet2D.y = 20;
    dEnemyBullet.shape.setOrigin(dEnemyBullet.shape.getGlobalBounds().width/2 , dEnemyBullet.shape.getGlobalBounds().height/2);
    dEnemyBullet.shape.setSize(dEnemyBullet.bullet2D);
    dEnemyBullet.shape.setFillColor(Color::Blue);
    
}
void update(player& playerst ,enemybullet& dEnemyBullet , float dt )
{
        if(dEnemyBullet.isthere)
        {
            dEnemyBullet.shape.move(dEnemyBullet.speed * dEnemyBullet.direction* dt , 0);
        }
        if (dEnemyBullet.shape.getPosition().x > playerst.megamanSpr.getPosition().x + windowWidth  || dEnemyBullet.shape.getPosition().x < playerst.megamanSpr.getPosition().x - windowWidth) {
    
            dEnemyBullet.isthere = false;
            dEnemyBullet.shape.setPosition(20000 , 20000);
        }


}
void shooting(enemybullet& dEnemyBullet,player& playerst,float dt , enemy& dEnemy )
{
    enemydetection(dEnemy, playerst );

    if(dEnemy.isActive)
    {

        bool atFireFrame = (dEnemy.eIndex == 11 && dEnemy.goingForward);
        
        if (!dEnemyBullet.isthere && !dEnemy.hasFired && atFireFrame) 
        {
            dEnemyBullet.isthere = true;
            dEnemy.hasFired = true; 
            
            dEnemyBullet.shape.setPosition(dEnemy.enemySpr.getPosition().x,dEnemy.enemySpr.getPosition().y);
                
                
            
    
            if(dEnemy.enemySpr.getPosition().x > playerst.megamanSpr.getPosition().x)
            {
                            
                dEnemyBullet.direction = -1;
            }
            else
            {
                dEnemyBullet.direction = 1;
             
            }
        }

        
    }
    update(playerst,dEnemyBullet , dt);


}
void enemyAnimation(enemy& dEnemy, float dt)
{   
    int raw ;
    int col;

    if (dEnemy.isActive)
    {

        dEnemy.timer+=dt;
        

        if(dEnemy.timer>=dEnemy.frameduration)
        {
            dEnemy.timer = 0;
            if(dEnemy.goingForward)
            {
                dEnemy.eIndex++;
                       
                if (dEnemy.eIndex >= 12)          // hit the last frame → reverse
                {
                    dEnemy.goingForward = false;
                    dEnemy.eIndex = 10;  
                }
            }
            else
            {
                dEnemy.eIndex--;
                if (dEnemy.eIndex < 0)            // back at start → go forward again
                {
                    dEnemy.goingForward = true;
                    dEnemy.eIndex = 1;
                    dEnemy.hasFired = false;  //!!!!    // ← reset so bullet can fire next cycle
                }
            }
        }
    
        if(dEnemy.eIndex<7)
        {
            raw = 0;
            col = dEnemy.eIndex;

        }
        else
        {
            raw =1;
            col =dEnemy.eIndex-7;//!!!!!
        }
        dEnemy.enemySpr.setTextureRect(IntRect(col * dEnemy.framewidth,raw * dEnemy.frameheight,dEnemy.framewidth,dEnemy.frameheight));
        
        
        
        
        
    }
}
//CAMERA BOUNDS FUNCTION
void camBounds(float LeftOffset, float RightOffset, float UpOffset, float DownOffset){
        //cout << Mouse::getPosition(window).x << " " << Mouse::getPosition(window).y << endl; //debugging
        const float MAP_WIDTH = 15400.f; 
        const float MAP_HEIGHT = 600.f;
        const float MAP_START_X = 200.f;
        const float MAP_START_Y = 0.f;
        //updated map logic with limits
        float camX = playerst.megamanSpr.getPosition().x;
        float camY = playerst.megamanSpr.getPosition().y;

        // calculate the boundary limits
        float minX = MAP_START_X + (windowWidth / 2.0f) + LeftOffset;
        float maxX = (MAP_START_X + MAP_WIDTH) - (windowWidth / 2.0f) - RightOffset;

        float minY = MAP_START_Y + (windowHeight / 2.0f) + UpOffset;
        float maxY = (MAP_START_Y + MAP_HEIGHT) - (windowHeight / 2.0f) - DownOffset;
        if (camX < minX) {
            camX = minX; 
        } 
        else if (camX > maxX) {
            camX = maxX; 
        }

        if (camY < minY) {
            camY = minY; 
        } 
        else if (camY > maxY) {
            camY = maxY; 
        }
        camera.setCenter(camX, camY);

}
View aspectRatio(View view, float windowWidth, float windowHeight) {
    float targetAspectRatio = 4.0f / 3.0f;
    float windowAspectRatio = windowWidth / windowHeight;
    float vpWidth = 1.0f, vpHeight = 1.0f, vpLeft = 0.0f, vpTop = 0.0f;

    if (windowAspectRatio > targetAspectRatio) { //If actual ratio is wider than target ratio
        vpWidth = targetAspectRatio / windowAspectRatio; // percentage of the width to use based on the ratio of the two aspect ratios
        vpLeft = (1.0f - vpWidth) / 2.0f; // centering the viewport horizontally so that the ratio percentage shown starts from middle
    }
    else {  // if actual ratio is taller than target ratio
        vpHeight = windowAspectRatio / targetAspectRatio; // percentage of the height to use based on the ratio of the two aspect ratios
        vpTop = (1.0f - vpHeight) / 2.0f; //centering the viewport vertically so that the ratio percentage shown starts from middle
    }

    view.setViewport(FloatRect(vpLeft, vpTop, vpWidth, vpHeight));
    return view;
}

void health_blockout(player& playerst,RectangleShape& blackout){
   float X=(19-playerst.health)*ratio_health;
    blackout.setScale(1,X);

}
void carMovement(Sprite& car, float carSpeed, float dt){
    car.move(carSpeed * dt, 0);
}