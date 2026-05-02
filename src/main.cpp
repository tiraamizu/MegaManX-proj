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
#define numEnemy2 2 // number of new enemies (the ones that move horizontally) in the game
#define numEnemy1 2
const float gravity = 1000.f;
const int blocks = 100;
const float ratio_health=3.84;
const float MegaSpawnX = 500.f;
const float MegaSpawnY = 100.f;
float musicVolume = 50.f;

enum GameState { MAIN, OPTIONS, GAME, AUDIO};

//SOUND DECLARATIONS
SoundBuffer buffer;
SoundBuffer titlebuffer;
SoundBuffer stagebuffer;
SoundBuffer winbuffer;


Sound shoot;
Sound titlesmusic;
Sound levelmusic;
Sound win;
bool winsound = true;
//~~~~~~~~~~~~~~~~~~~~Main Window Resolution~~~~~~~~~~~~~~~~~~~~~~~~~~
const float windowWidth = 640;
const float windowHeight = 480;
//player hitbox
const Vector2f mega_hitbox_size(55.f,60.f); //reminder to change this later depending on megaman's sprite,

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GAMESTRUCTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

View camera(FloatRect(0, 0, windowWidth, windowHeight));
View menuCamera(FloatRect(0, 0, windowWidth, windowHeight));
View healthUI(FloatRect(0, 0, windowWidth, windowHeight));
// ## player and enemy structs
struct player
{
	Texture megamanTexture , runFireTexture , jumpTexture , jumpFireTexture, wallSlideTexture, idleTexture;
	Sprite megamanSpr;
    RectangleShape hitbox; //for every interaction EXCEPT ground and wall jump
    Texture healthbar_text;
    RectangleShape healthbar;
    Vector2f Pos_Tracker; 
    float Vx = 300.f;
    float Vy = 0.0f;
    float invTimer = 1.0f;
    float death_timer=5.0f;
	float runFrameDuration = 0.1f;
    float jumpFramrDuration = 0.12f;
    float idleAnimDuration = 0.54f;
	float timer = 0.0f;
    float jumpstrength = -400.f;
	int framewidth = 36; // each frame height and width don't ask how i calculated it 
	int frameheight = 35;
	int i = 0; // our frame counter
    int runFrames=11, runFireFrames=10, jumpFrames=7, jumpFireFrames=7, wallSlideFrames = 5, idleAnimFrames = 3;
    int runW=35, runH=34 ;
    int runFireW=41, runFireH=36;
    int jumpW=30, jumpH=46;
    int jumpFireW=36, jumpFireH=52;
    int wallSlideW = 30, wallSlideH = 44;
    int idleAnimW = 30, idleAnimH = 35;
    int health =19; //max hp is 19
    enum dir {NONE,LEFT,RIGHT};
    dir toucheswall = NONE; 
    bool isInv = false;
    bool moving = false;
    bool touchesground = false;
    bool issliding = false;
  
  
		
} playerst;
struct enemy1 {
    RectangleShape hitbox;
    Texture enemyTexture;
    Sprite enemySpr;
    int hp = 10;
    int framewidth = 50; // each frame height and width don't ask how i calculated it 
	int frameheight = 65;
    int eIndex = 0;
    float invTimer = 0.25f;
    float detectionRange = 500.f;
    float timer= 0.0f;
    float frameduration = 0.15f;
    float Vy = 0.0f;
    bool isActive = false; // the range at which the enemy will detect the player and start moving towards him
    bool isInv = false;
    bool alive = true;
    bool atFireFrame = false;
    bool goingForward = true; // ping-pong direction flag
    bool hasFired    = false;
    
    bool touchesground = false;
    
} arrEnemy1[numEnemy1];
struct enemy2
{
    Sprite enemy2Spr;
    Texture arrEnemy2Texture;
    RectangleShape hitbox;
    int framewidth = 36; // each frame height and width don't ask how i calculated it 
    int frameheight = 35;
    int enemy2Index = 0;
    float invTimer = 0.25f;
    float timer= 0.0f;
    float frameduration = 0.1f;
    float speed = -150.f;
    float Vy = 0.0f;
    float detectionRange = 500.f;
    bool isInv = false;
    bool touchesground = false;
    bool alive = true;
    bool isActive = false;
} arrEnemy2[numEnemy2];


// ## enemy and player bullet struct
struct bullet
{
    Texture bulletTexture;
    Sprite bulletSpr;
    RectangleShape shape;
    Vector2f bullet2D;
    int direction = 1;  
    float speed =600.f ;
    bool isthere = false;
   
}prj,windowmag[nbullets];

struct enemy1bullet
{
    Texture enemyBulletTexture;
    Sprite enemyBulletSpr;
    int direction = 1;  
    float speed =400.f ;
    bool isthere = false;
    


}arrEnemy1Bullet;


// ## map struct
struct groundobj
{
    RectangleShape gnd;
    int blockwidth = 200.f;
    int blockheight = 100.f;

}ground[blocks];
struct winobj{
Texture win;
Sprite winRect;
float width = 150.f;
float height = 100.f;
int winX=15000;
int winY=315;
}winobject;

//ّّ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GAME FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




// Function declarations (m is a menu struct variable, its passed by reference to avoid copying the struct and to allow us to mod the struct's data)

//## Menu declaration functions (for main menu and options menu)
void initMenu(MenuData &m, float width, float height);
void initOptions(MenuData &m, float width, float height);
void initAudio(MenuData &m, float width, float height, float &musicVolume);
void drawMenuSelection(MenuData &m, RenderWindow &window);
void up(MenuData &m);
void down(MenuData &m);
void menuSwitchHandler(RenderWindow &window, Event &event, MenuData &m, MenuData &options, MenuData &audioMenu, Keyboard::Key interractionButton);
void volumeAdjustment(MenuData& audioMenu, float &musicVolume, float increaseAmount);
bool resourcesCheck(MenuData &m,winobj &winobject);
void camBounds(float LeftOffset, float RightOffset, float UpOffset, float DownOffset);

/*NOTE : m IS A FORMAL PARAMETER, IT CAN BE CALLED ANYTHING, I JUST CHOSE M FOR MENU.
THE NAMES OF THE PARAMETERS DO NOT AFFECT THE FUNCTIONALITY OF THE CODE, THEY ARE JUST PLACEHOLDERS TO MAKE THE CODE MORE READABLE.
NOTICE THAT THE INT MAIN FUNCTION CALLS ACTUALLY USE THE NAMES (ARGUMENTS) mainMenu AND optionsMenu.
*/

 
// ## States functions
void playerstats(player& playerst);
void playerBulletStates(bullet& prj);
void enemyStates(enemy1& arrEnemy1,float &xpos, float &ypos);
void enemyBulletStates(enemy1bullet& arrEnemy1Bullet);
void enemy2Status(enemy2& arrEnemy2, float xpos);


// ## Animation functions
void animationhandler(player& playerst, float dt);
void runFireAnim(player& playerst , float dt );
void jumpFireAnim(player& playerst, float dt);
void jumpAnim(player& playerst, float dt);
void standFireAnim(player& playerst, float dt);
void enemyAnimation(enemy1& arrEnemy1, float dt);
void wallSlideAnim(player& playerst, float dt);
void idleAnim(player& playerst, float dt);
void enemy2Animation(enemy2& arrEnemy2, float dt);


// ## bullet shooting and update
bool playerBulletUpdate(bullet windowmag[] ,player& playerst ,float dt,RenderWindow& window);
void enemy1Shooting(enemy1bullet& arrEnemy1Bullet,player& playerst, float dt , enemy1& dEenmy);
void enemy1BulletUpdate(player& playerst ,enemy1bullet& arrEnemy1Bullet , float dt );

// ## enemies detection range
void enemydetection(enemy1& arrEnemy1 , player& playerst);
void enemy2Detection(enemy2& arrEnemy2, player& playerst ,float dr);
// ## controls input
void inputhandler(player& playerst, float dt );

// ## detections and intersections
void playerhitbox_pos(player& playerst);
void checkPlayerInvincibility(player& playerst,float &dt);
void handleIntersection(float &dt);
void initwinobject(winobj& winobject,RenderWindow& window);
void winIntresection(winobj& winobject, RenderWindow& window,player &playerst, bool &won, bool &isPaused);
void Gravity(player& playerst, float &dt);

// ## health and damage 
void deathHandler(player& playerst, float &dt);
void death_timer(player& playerst,float &dt);
void damage (player& playerst);
void heal (player& playerst);
bool isPaused = false; 
bool won = false;
void health_blockout(player& playerst,RectangleShape& blackout);
float storedVx = playerst.Vx; // for death functions



// ## camera and map
void createBlock(int index, float x, float y, float width, float height);
View aspectRatio(View view, float windowWidth, float windowHeight);
void carMovement(Sprite& car, float carSpeed, float dt);






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN FUNCTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
    RectangleShape blackout;
    blackout.setFillColor(Color::Black);
    blackout.setSize(Vector2f(15, 1));
    blackout.setPosition(31.f, 141.f);
    
    RenderWindow window(VideoMode(windowWidth, windowHeight), "MMX prototype");

    MenuData mainMenu;
    MenuData optionsMenu;
    MenuData audioMenu;

    if (!resourcesCheck(mainMenu,winobject) || !resourcesCheck(optionsMenu,winobject) || !resourcesCheck(audioMenu,winobject)) {
        cout<< "Error loading resources. Please check the file paths and ensure all resources are available." << endl;
        //crashes program to stop messages (in resourcesCheck) from looping if logo is not found
    };

    initMenu(mainMenu, (float)window.getSize().x, (float)window.getSize().y); //initializes menu in 1st argument, 2nd and 3rd arguments are used for calculations regarding positions of menu items.
    initOptions(optionsMenu, (float)window.getSize().x, (float)window.getSize().y); //same as initMenu but for options menu.
    initAudio(audioMenu, (float)window.getSize().x, (float)window.getSize().y, musicVolume); //same as initMenu but for audio menu.

    float dt; // delta time and clock for the whole game loop
    Clock clock;
    

    playerstats(playerst);
    enemyBulletStates(arrEnemy1Bullet);

    for (int i = 0; i < nbullets; i++) {
        playerBulletStates(windowmag[i]);//this set up all the empty bullets with all of the playerBulletStates intializations
    }

    for (int i = 0; i < numEnemy1; i++) {
        float xpos = 2000.f + (i * 2600.f);
        float ypos = 350;
        enemyStates(arrEnemy1[i], xpos, ypos);
    }

    for (int i = 0; i < numEnemy2; i++) {
        float xpos = 1300.f + (i * 1800.f);
        enemy2Status(arrEnemy2[i], xpos);
    }

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
    createBlock(16, 15555, 520, 100, 700);
    createBlock(17, 210, 370, 100, 700);

    Keyboard::Key interractionButton = Keyboard::Z;
    Event event;

    while (window.isOpen())
    {
        dt = clock.restart().asSeconds();// this calculate the deltatime
        if (dt > 0.05f) {
            dt = 0.05f; 
        } // dt limiter to prevent lagspikes from breaking some game stuff

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) 
            {
                window.close();
            }

            if (event.type == Event::Resized) 
            {
                camera = aspectRatio(camera, event.size.width, event.size.height);
                menuCamera = aspectRatio(menuCamera, event.size.width, event.size.height);
                healthUI = aspectRatio(healthUI, event.size.width, event.size.height); 
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                isPaused = !isPaused;
                if (!isPaused) {
                    clock.restart();
                }
            }

            if (!isPaused && mainMenu.curState == GAME)
            {
                //jump  
                if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space && (playerst.touchesground || playerst.issliding)) 
                {
                    playerst.Vy = playerst.jumpstrength;
                    playerst.touchesground = false;
                }

                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bullet firing code
                if(event.type == Event::KeyPressed && event.key.code == Keyboard::A)
                {
                    shoot.setBuffer(buffer);
                    shoot.play();
                    for(int i = 0 ; i < nbullets ; i++)
                    {
                        if(!windowmag[i].isthere)//here we are scaning if the slot of the struct array already has a bullet and fired ot empty?
                        {
                            // of the slot is empty and the player pressed 'z' then these next line of codes relode this bullet and fire it 
                            //from the postion of the character and in horizontal and vertical position , since our character can jump while jumping:D
                            windowmag[i].isthere = true;    // x positon                                    y position
                            windowmag[i].bulletSpr.setPosition(playerst.megamanSpr.getPosition().x,playerst.megamanSpr.getPosition().y);//changed
                            //this because the y = 0 will always spawn the ballet in a different location if megaman y is different
                            if(playerst.megamanSpr.getScale().x > 0)//this check the character direction by the x scale we wrote above
                            {
                                windowmag[i].direction = 1;
                            }
                            else
                            {
                                windowmag[i].direction = -1;
                            }

                            Transform megaTransform = playerst.megamanSpr.getTransform();
                            Vector2f spawnPos;
                            if(!playerst.touchesground) 
                            {
                                spawnPos = megaTransform.transformPoint(34.f, 22.f);
                            }
                            else
                            {
                                spawnPos = megaTransform.transformPoint(34.f, 15.f);
                            }
                            
                            windowmag[i].bulletSpr.setPosition(spawnPos);
                            windowmag[i].bulletSpr.setScale(2.0f * windowmag[i].direction, 2.0f);
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
            }
            menuSwitchHandler(window, event, mainMenu, optionsMenu, audioMenu, interractionButton);
        }

        window.clear();

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

        case AUDIO:
            drawMenuSelection(audioMenu, window);
            break;

        case GAME:
        {

            //Game logic stuff section
            if (!isPaused)
            {
                inputhandler(playerst, dt); 
                deathHandler(playerst, dt);
                Gravity(playerst, dt);
                camBounds(70, 40, 40, 60);
                carMovement(map1.car1Spr, -200.f, dt);
                carMovement(map1.car2Spr, -200.f, dt);
                playerst.Pos_Tracker=playerst.megamanSpr.getPosition();//tracks position of megaman
                playerhitbox_pos(playerst); //constnatly enemy1BulletUpdates hitbox to be on megaman
                handleIntersection(dt);
                for(int i = 0; i < numEnemy1; i++)//call the enemy1 function in a loop for both enemies
                {
                    arrEnemy1[i].hitbox.setPosition(arrEnemy1[i].enemySpr.getPosition());
                    enemyAnimation(arrEnemy1[i] , dt);
                    enemy1Shooting(arrEnemy1Bullet, playerst, dt , arrEnemy1[i]);
                    /*
                    if(death )
                    {                                                           
                        arrEnemy1[i].alive = false;
                        arrEnemy1[i].enemySpr.setPosition(200000,200000);
                    }*/
                }
                
                for (int i = 0; i < numEnemy2; i++) 
                { 
                    arrEnemy2[i].hitbox.setPosition(arrEnemy2[i].enemy2Spr.getPosition());
                    enemy2Animation(arrEnemy2[i],dt);
                    enemy2Detection(arrEnemy2[i],playerst , dt);

                    
                    /*
                    if(death )
                    {                                                                               
                        arrEnemy2[i].alive = false;
                        arrEnemy2[i].enemy2Spr.setPosition(200000,200000);
                    }*/
                }

                bool IsFiring = playerBulletUpdate(windowmag, playerst, dt, window);
                if(!playerst.moving && !IsFiring && playerst.touchesground){
                    idleAnim(playerst, dt);
                }
                else if(playerst.issliding)
                {
                    wallSlideAnim(playerst, dt);
                }
                else if(!playerst.touchesground && IsFiring )
                {
                    jumpFireAnim(playerst, dt);
                }
                else if(!playerst.touchesground)
                {
                    jumpAnim(playerst, dt);
                }
                else if(playerst.moving && IsFiring)
                {
                    runFireAnim(playerst ,dt);
                }
                else if(!playerst.moving && IsFiring)
                {
                    standFireAnim(playerst, dt);
                }
                else
                {
                    animationhandler(playerst, dt);
                }
            }

            //Rendering section

            window.setView(camera);
            window.draw(map1.backgroundSpr[0]);
            window.draw(map1.mapSpr);
            window.draw(map1.car1Spr);
            window.draw(map1.car2Spr);
            deathHandler(playerst, dt);
            initwinobject(winobject,window);
            winIntresection(winobject,window,playerst,won,isPaused);

            for(int i = 0; i < numEnemy1; i++)
            {
                if(arrEnemy1[i].alive)
                {
                    window.draw(arrEnemy1[i].enemySpr);
                    window.draw(arrEnemy1[i].hitbox); 
                    
                }
            }

            if(arrEnemy1Bullet.isthere)
                window.draw(arrEnemy1Bullet.enemyBulletSpr);
            
            for (int i = 0; i < numEnemy2; i++) 
            { 
                if (arrEnemy2[i].alive) 
                {
                    window.draw(arrEnemy2[i].enemy2Spr);
                    window.draw(arrEnemy2[i].hitbox); 
                    
                }
            }

            window.draw(playerst.megamanSpr);
            window.draw(playerst.hitbox);
            
            for (int i = 0; i < nbullets; i++) 
            {
                if (windowmag[i].isthere) 
                {
                    window.draw(windowmag[i].bulletSpr);
                }
            }

            //constants on the screen (aka just health), dont do .draw under it or else it wont function the same as you want
            // if you wanna do .draw   make sure to reuse window.setView(camera); and then use window.draw under it
            window.setView(healthUI);
            health_blockout(playerst,blackout);
            window.draw(playerst.healthbar);
            window.draw(blackout);

            if (event.key.code == Keyboard::X) 
            {
                mainMenu.curState = MAIN;
            }
            if (event.key.code == Keyboard::C) 
            {
                playerst.megamanSpr.setPosition(MegaSpawnX, MegaSpawnY);
                playerst.hitbox.setPosition(playerst.megamanSpr.getPosition());
                isPaused = false;
                won = false;
                playerst.health = 19;
            } //debugging

            if(isPaused && !won){
                Text pausedText("PAUSED", mainMenu.font, 50);
                pausedText.setFillColor(Color::Red);
                pausedText.setPosition(window.getView().getCenter().x - pausedText.getGlobalBounds().width / 2, window.getView().getCenter().y - pausedText.getGlobalBounds().height / 2);
                window.draw(pausedText);
            }
             if(isPaused && won){
                Text winText("WIN", mainMenu.font, 50);
                winText.setFillColor(Color::Green);
                winText.setPosition(window.getView().getCenter().x - winText.getGlobalBounds().width / 2, window.getView().getCenter().y - winText.getGlobalBounds().height / 2);
                window.draw(winText);
            }
            break;
        }

        default:
            mainMenu.curState = MAIN;
            break;
        }

        window.display();
    }
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FUNCTION DEFINITIONS  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int charSize = 20, xOffset = -100, yOffset = 270;

//##Initializes main menu resources
bool resourcesCheck(MenuData &m,winobj &winobject) {
    if (!map1.mapTexture.loadFromFile("textures/map.png")) {
    cout << "ERROR: Could not find textures/map.png" << endl;
        return false;
    }
    if(!buffer.loadFromFile("sounds/shoot.wav")){
            cout<<"ERR: shoot.wav not found";
            return false;
            }
    if(!titlebuffer.loadFromFile("sounds/titlescreen.wav")){
            cout<<"ERR: titlescreen.wav not found";
            return false;
            }
    if(!stagebuffer.loadFromFile("sounds/stagemusic.wav")){
            cout<<"ERR: stagemusic.wav not found";
            return false;
            }
    if(!winbuffer.loadFromFile("sounds/win.wav")){
            cout<<"ERR: win.wav not found";
            return false;
            }
    if (!winobject.win.loadFromFile("textures/wingem.png")) {
    cout << "ERROR: Could not find textures/wingem.png" << endl;
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
    titlesmusic.setBuffer(titlebuffer);
    titlesmusic.play();
    titlesmusic.setLoop(true);
    m.curMaxButtons = 3;

    //GAME START
    m.menuSelection[0].setFont(m.font);
    m.menuSelection[0].setFillColor(m.selectedColor);
    m.menuSelection[0].setString("GAME START");
    m.menuSelection[0].setCharacterSize(charSize);
    m.menuSelection[0].setPosition(((width / 2)) + xOffset, (height / (MAX_ITEM_NO + 10) * 1) + yOffset);

    //OPTION MODE
    m.menuSelection[1].setFont(m.font);
    m.menuSelection[1].setFillColor(m.unselectedColor);
    m.menuSelection[1].setString("OPTION MODE");
    m.menuSelection[1].setCharacterSize(charSize);
    m.menuSelection[1].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 10) * 2) + yOffset);

    //TERMINATE
    m.menuSelection[2].setFont(m.font);
    m.menuSelection[2].setFillColor(m.unselectedColor);
    m.menuSelection[2].setString("TERMINATE");
    m.menuSelection[2].setCharacterSize(charSize);
    m.menuSelection[2].setPosition((width / 2) + xOffset, (height / (MAX_ITEM_NO + 10) * 3) + yOffset);
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

    m.curMaxButtons = 2;
    //AUDIO
    m.menuSelection[0].setString("AUDIO");
    //BACK
    m.menuSelection[1].setString("BACK");

    m.curButtonIndex = 0;
}

void initAudio(MenuData &m, float width, float height, float &musicVolume) {
    // we use initMenu to copy data from the menu struct and reuse it in options.
    initMenu(m, width, height);

    m.curMaxButtons = 2;
    m.menuSelection[0].setString("MUSIC VOLUME : " + to_string((int)musicVolume));
    //BACK
    m.menuSelection[1].setString("BACK");

    m.curButtonIndex = 0;
}

//main is a placeholder for mainmenu, options is a placeholder for options menu.
//this function handles switching between menus and the game, it also handles the menu interraction button (Z in this case) for both menus and the game.
void menuSwitchHandler(RenderWindow &window, Event &event, MenuData &main, MenuData &options, MenuData &audioMenu, Keyboard::Key interractionButton) {
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
                    titlesmusic.stop();
                    levelmusic.setBuffer(stagebuffer);
                    levelmusic.play();
                    levelmusic.setLoop(true);
                    main.curState = GAME;
                }
                if (main.curButtonIndex == 1) {
                    main.curState = OPTIONS;
                }
                if (main.curButtonIndex == 2) {
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
                if (options.curButtonIndex == 1) {
                    main.curState = MAIN;
                }
            }
            if (event.key.code == interractionButton) {
                if (options.curButtonIndex == 0) {
                    main.curState = AUDIO;
                }
            }
            break;

            case GAME:
            if (event.key.code == Keyboard::X) {
                levelmusic.stop();
                titlesmusic.setBuffer(titlebuffer);
                titlesmusic.play();
                titlesmusic.setLoop(true);
                main.curState = MAIN;
            }
            break;
        
            case AUDIO:
            if (event.key.code == Keyboard::Up) {
                up(audioMenu);
            }
            if (event.key.code == Keyboard::Down) {
                down(audioMenu);
            }
            if (event.key.code == interractionButton) {
                    if (audioMenu.curButtonIndex == 1) {
                        main.curState = OPTIONS;
                    }
            }
            if(audioMenu.curButtonIndex == 0){
                if(event.key.code == Keyboard::Left){
                    volumeAdjustment(audioMenu, musicVolume, -1.f);
                }
                if(event.key.code == Keyboard::Right){
                    volumeAdjustment(audioMenu, musicVolume, 1.f);
                }
            }
            audioMenu.menuSelection[0].setString("MUSIC VOLUME : " + to_string((int)musicVolume));

        default:
            break;
        }
    }
}


//## Menu declaration functions (for main menu and options menu)


// ## States functions
void playerstats(player& playerst) // p for better writing :D
{
	playerst.megamanTexture.loadFromFile("textures/running35-34.png");
	playerst.runFireTexture.loadFromFile("textures/firing-moving(41-36).png");
    playerst.jumpTexture.loadFromFile("textures/jump(30-46).png");
    playerst.jumpFireTexture.loadFromFile("textures/fire-Jumping(36-52).png");
    playerst.wallSlideTexture.loadFromFile("textures/wallSlideAnimation (30x44).png");
    playerst.idleTexture.loadFromFile("textures/Idle_anim.png");

	playerst.megamanSpr.setPosition(MegaSpawnX, MegaSpawnY);
	playerst.megamanSpr.setScale(2.0f, 2.0f);  
	playerst.megamanSpr.setTextureRect(IntRect(0, 0, playerst.framewidth, playerst.frameheight));//start with the first frame of the sprite sheet
	//note we will change this if we want to make a standing animation
	
    playerst.hitbox.setFillColor(Color::Transparent);
    playerst.hitbox.setSize(mega_hitbox_size);
    playerst.hitbox.setOrigin(mega_hitbox_size.x/2,mega_hitbox_size.y/2);
    playerst.healthbar.setSize(Vector2f(30,55));
    playerst.healthbar.setPosition(20,135);
    playerst.healthbar.scale(1.25f,2.f);
    playerst.healthbar_text.loadFromFile("textures/healthbar.png");
    playerst.healthbar.setTexture(&playerst.healthbar_text);
}
void playerBulletStates(bullet& prj)
{
    prj.bulletTexture.loadFromFile("textures/mmx1-buster.png");
    prj.bulletSpr.setTexture(prj.bulletTexture);
    prj.bullet2D.x = 20;
    prj.bullet2D.y = 20;
    prj.bulletSpr.setOrigin(prj.bulletSpr.getGlobalBounds().width/2 , prj.bulletSpr.getGlobalBounds().height/2);
    prj.bulletSpr.setScale(2.0f,2.0f);
    
    //prj.isthere =false; // leave if for future bec. this will help us if we add a button to rest the level
    // due to this button resets the condition after every loob not the struct

}
void enemyStates(enemy1& arrEnemy1 ,float &xpos, float &ypos)
{
    arrEnemy1.enemySpr.setPosition(xpos, 170.f);
    arrEnemy1.enemyTexture.loadFromFile("textures/enemies_full2.png");
    arrEnemy1.enemySpr.setTexture(arrEnemy1.enemyTexture); //assigning the texture to the sprite so that we can use it in the game loop
    arrEnemy1.enemySpr.setOrigin(arrEnemy1.framewidth/ 2.0f, arrEnemy1.frameheight / 2.0f);	
    arrEnemy1.enemySpr.setScale(2.5f, 2.5f);  
    arrEnemy1.enemySpr.setTextureRect(IntRect({0, 0, arrEnemy1.framewidth, arrEnemy1.frameheight}));
    arrEnemy1.hitbox.setOrigin((arrEnemy1.framewidth * 2.2f) / 2.f,(arrEnemy1.frameheight * 2.2f) / 2.f);
    arrEnemy1.hitbox.setSize(Vector2f(arrEnemy1.framewidth * 2.2f, arrEnemy1.frameheight * 2.2f));
    
    arrEnemy1.hitbox.setPosition(xpos, 170.f);
    arrEnemy1.hitbox.setFillColor(Color::Transparent);
    arrEnemy1.hitbox.setOutlineColor(Color::Red);  //testing 
    arrEnemy1.hitbox.setOutlineThickness(1.f);     //testing
}
void enemyBulletStates(enemy1bullet& arrEnemy1Bullet)
{
    arrEnemy1Bullet.enemyBulletTexture.loadFromFile("textures/enemybullet.png");
    arrEnemy1Bullet.enemyBulletSpr.setTexture(arrEnemy1Bullet.enemyBulletTexture);
    arrEnemy1Bullet.enemyBulletSpr.setOrigin(
        arrEnemy1Bullet.enemyBulletTexture.getSize().x / 2.f,
        arrEnemy1Bullet.enemyBulletTexture.getSize().y / 2.f
    );
    arrEnemy1Bullet.enemyBulletSpr.setScale(2.0f, 2.0f); // 
}
void enemy2Status(enemy2& arrEnemy2, float xpos)
{
    arrEnemy2.enemy2Spr.setPosition( xpos , 247.f );
    arrEnemy2.arrEnemy2Texture.loadFromFile("textures\\enemyr2(36x34).png");
    arrEnemy2.enemy2Spr.setTexture(arrEnemy2.arrEnemy2Texture); //assigning the texture to the sprite so that we can use it in the game loop
    arrEnemy2.enemy2Spr.setOrigin(arrEnemy2.framewidth/ 2.0f, arrEnemy2.frameheight / 2.0f);	
    arrEnemy2.enemy2Spr.setScale(4.0f, 4.0f);  
    arrEnemy2.hitbox.setSize(Vector2f(arrEnemy2.framewidth * 4.f, arrEnemy2.frameheight * 4.f));
    arrEnemy2.hitbox.setOrigin(arrEnemy2.framewidth * 2.f, arrEnemy2.frameheight * 2.f);
    arrEnemy2.hitbox.setPosition(xpos, 247.f);
    arrEnemy2.hitbox.setFillColor(Color::Transparent);
    arrEnemy2.hitbox.setOutlineColor(Color::Red);    // optional: see it while debugging
    arrEnemy2.hitbox.setOutlineThickness(1.f);

}

// ## Animation functions
void animationhandler(player& playerst, float dt)
{

	if(playerst.moving)  
    {
        if(playerst.megamanSpr.getTexture() != &playerst.megamanTexture )
        {
            playerst.megamanSpr.setTexture(playerst.megamanTexture);
            playerst.megamanSpr.setOrigin(playerst.runW/2.f,playerst.runH/2);
            playerst.timer = 0.0f;
            playerst.i = 0;
        }
        
        playerst.timer += dt; 
		if (playerst.timer >= playerst.runFrameDuration)// this condition is only made for the time being because since we only now have 6 frmaes 
			// and each frame frameduartion of 0.1 , this means that each time the timer goes up by 0.1 the the frame is enemy1BulletUpdated 
			// and the game loop after 0.1 s will display the new frame and in out case since we only have 6 frames then after
			// each 0.6 s the frame loop resets  and that's what this condition is saying :DD
		{
			playerst.timer = 0;
			playerst.i++;
            if (playerst.i >= playerst.runFrames)
				playerst.i = 0;
			

		}
        playerst.megamanSpr.setTextureRect(IntRect(playerst.i * playerst.runW, 0,playerst.runW,playerst.runH));

    }
    else
    {   
        playerst.megamanSpr.setTexture(playerst.megamanTexture);
        playerst.megamanSpr.setOrigin(playerst.runW / 2.f, playerst.runH / 2.f);
        playerst.i     = 0;
        playerst.timer = 0.f;
        playerst.megamanSpr.setTextureRect(IntRect(0, 0, playerst.runW, playerst.runH));
    
    }
}
void enemyAnimation(enemy1& arrEnemy1, float dt)
{   
    int raw ;
    int col;

    if (arrEnemy1.isActive)
    {

        arrEnemy1.timer+=dt;
        

        if(arrEnemy1.timer>=arrEnemy1.frameduration)
        {
            arrEnemy1.timer = 0;
            if(arrEnemy1.goingForward)
            {
                arrEnemy1.eIndex++;
                       
                if (arrEnemy1.eIndex >= 12)          // hit the last frame → reverse
                {
                    arrEnemy1.goingForward = false;
                    arrEnemy1.eIndex = 11;  
                }
            }
            else
            {
                arrEnemy1.eIndex--;
                if (arrEnemy1.eIndex < 0)            // back at start → go forward again
                {
                    arrEnemy1.goingForward = true;
                    arrEnemy1.eIndex = 1;
                    arrEnemy1.hasFired = false;  //!!!!    // ← reset so bullet can fire next cycle
                }
            }
        }
    
        if(arrEnemy1.eIndex<7)
        {
            raw = 0;
            col = arrEnemy1.eIndex;

        }
        else
        {
            raw =1;
            col =arrEnemy1.eIndex-7;//!!!!!
        }
        arrEnemy1.enemySpr.setTextureRect(IntRect(col * arrEnemy1.framewidth,raw * arrEnemy1.frameheight,arrEnemy1.framewidth,arrEnemy1.frameheight));
        
        
        
        
        
    }
}
void enemy2Animation(enemy2& arrEnemy2, float dt)
{
    if(arrEnemy2.isActive)
    {
        arrEnemy2.timer += dt;
        if (arrEnemy2.timer >= arrEnemy2.frameduration)
        {
            arrEnemy2.timer = 0.0f;
            arrEnemy2.enemy2Index++;
            if(arrEnemy2.enemy2Index>=3)
            {
                arrEnemy2.enemy2Index = 0;
            }
            
        }
        arrEnemy2.enemy2Spr.setTextureRect(IntRect(arrEnemy2.enemy2Index * arrEnemy2.framewidth, 0, arrEnemy2.framewidth, arrEnemy2.frameheight) );


    }
}
void runFireAnim(player& playerst , float dt)  
{
    if(playerst.megamanSpr.getTexture() != &playerst.runFireTexture )
    {
        playerst.megamanSpr.setTexture(playerst.runFireTexture);
        playerst.megamanSpr.setOrigin(playerst.runFireW/2.f , playerst.runFireH/2.f);
        playerst.i = 0;
        playerst.timer = 0.0f;
    }
    playerst.timer += dt;
    if (playerst.timer >= playerst.runFrameDuration) 
    {
        playerst.timer = 0.0f;
        playerst.i++;
        if (playerst.i >= playerst.runFireFrames)
        {
            playerst.i =0;
        }
    }
    playerst.megamanSpr.setTextureRect(IntRect(playerst.i*playerst.runFireW,0,playerst.runFireW,playerst.runFireH  ));
}
void idleAnim(player& playerst , float dt)  {
    if(playerst.megamanSpr.getTexture() != &playerst.idleTexture )
    {
        playerst.megamanSpr.setTexture(playerst.idleTexture);
        playerst.megamanSpr.setOrigin(playerst.idleAnimW/2.f , playerst.idleAnimH/2.f);
        playerst.i = 0;
        playerst.timer = 0.0f;
    }
    playerst.timer += dt;
    if (playerst.timer >= playerst.idleAnimDuration) 
    {
        playerst.timer = 0.0f;
        playerst.i++;
        if (playerst.i >= playerst.idleAnimFrames)
        {
            playerst.i =0;
        }
    }
    playerst.megamanSpr.setTextureRect(IntRect(playerst.i*playerst.idleAnimW,0,playerst.idleAnimW,playerst.idleAnimH  ));
}
void jumpAnim(player& playerst, float dt) {
    if(playerst.megamanSpr.getTexture() != &playerst.jumpTexture) {
        playerst.megamanSpr.setTexture(playerst.jumpTexture);
        playerst.megamanSpr.setOrigin(playerst.jumpW / 2.f, playerst.jumpH / 2.f);
        playerst.i = 0;
        playerst.timer = 0.f;
    }

    playerst.timer += dt;
    if(playerst.timer >= playerst.jumpFramrDuration) {
        playerst.timer = 0.f;
        // clamp at last frame — don't loop jump animation
        if(playerst.i < playerst.jumpFrames - 1)
            playerst.i++;
    }
    playerst.megamanSpr.setTextureRect(
        IntRect(playerst.i * playerst.jumpW, 0,playerst.jumpW, playerst.jumpH)
    );
}
void jumpFireAnim(player& playerst, float dt) {
    if(playerst.megamanSpr.getTexture() != &playerst.jumpFireTexture) {
        playerst.megamanSpr.setTexture(playerst.jumpFireTexture);
        playerst.megamanSpr.setOrigin(playerst.jumpFireW / 2.f, playerst.jumpFireH / 2.f);
        playerst.i = 0;
        playerst.timer = 0.f;
    }

    playerst.timer += dt;
    if(playerst.timer >= playerst.runFrameDuration) {
        playerst.timer = 0.f;
        if(playerst.i < playerst.jumpFireFrames - 1)
            playerst.i++; // clamp, don't loop
    }
    playerst.megamanSpr.setTextureRect(
        IntRect(playerst.i * playerst.jumpFireW, 0, playerst.jumpFireW, playerst.jumpFireH)
    );
}
void standFireAnim(player& playerst, float dt)
{
    playerst.megamanSpr.setTexture(playerst.runFireTexture);
    playerst.megamanSpr.setOrigin(playerst.runFireW / 2.f, playerst.runFireH / 2.f);

    // just hold frame 0 — standing fire pose, no leg movement
    playerst.megamanSpr.setTextureRect(
        IntRect(0, 0, playerst.runFireW, playerst.runFireH));
}
void wallSlideAnim(player& playerst, float dt)
{
    if(playerst.megamanSpr.getTexture() != &playerst.wallSlideTexture)
    {
        playerst.megamanSpr.setTexture(playerst.wallSlideTexture);
        playerst.megamanSpr.setOrigin(playerst.wallSlideW / 2.f, playerst.wallSlideH / 2.f);
        playerst.i = 0;
        playerst.timer = 0.f;
    }

    playerst.timer += dt;
    if(playerst.timer >= playerst.runFrameDuration)
    {
        playerst.timer = 0.f;
        if(playerst.i < playerst.wallSlideFrames - 1)
            playerst.i++; // clamp at last frame, don't loop
    }

    // flip sprite based on which wall player is touching
    if(playerst.toucheswall == playerst.LEFT)
        playerst.megamanSpr.setScale(-2.f, 2.f);
    else
        playerst.megamanSpr.setScale(2.f, 2.f);

    playerst.megamanSpr.setTextureRect(
        IntRect(playerst.i * playerst.wallSlideW, 0, playerst.wallSlideW, playerst.wallSlideH)
    );
}

// ## bullet shooting and update
void enemy1BulletUpdate(player& playerst ,enemy1bullet& arrEnemy1Bullet , float dt )
{
        if(arrEnemy1Bullet.isthere)
        {
            arrEnemy1Bullet.enemyBulletSpr.move(arrEnemy1Bullet.speed * arrEnemy1Bullet.direction* dt , 0);

            if (arrEnemy1Bullet.enemyBulletSpr.getPosition().x > playerst.megamanSpr.getPosition().x + windowWidth  || arrEnemy1Bullet.enemyBulletSpr.getPosition().x < playerst.megamanSpr.getPosition().x - windowWidth) 
            {
    
                arrEnemy1Bullet.isthere = false;
                arrEnemy1Bullet.enemyBulletSpr.setPosition(20000,20000);
            }
        }



}
void enemy1Shooting(enemy1bullet& arrEnemy1Bullet,player& playerst,float dt , enemy1& arrEnemy1 )
{
    enemydetection(arrEnemy1, playerst );

    if(arrEnemy1.isActive)
    {

        arrEnemy1.atFireFrame = (arrEnemy1.eIndex == 11 && arrEnemy1.goingForward);
        
        if (!arrEnemy1Bullet.isthere && !arrEnemy1.hasFired && arrEnemy1.atFireFrame) 
        {
            arrEnemy1Bullet.isthere = true;
            arrEnemy1.hasFired = true; 
            Transform enemyTransform = arrEnemy1.enemySpr.getTransform();
            Vector2f spawnPos = enemyTransform.transformPoint(40.f, 45.f);
            arrEnemy1Bullet.enemyBulletSpr.setPosition(spawnPos);
                        

            
            if(playerst.megamanSpr.getPosition().x> arrEnemy1.enemySpr.getPosition().x) //the bug happened because of the sprite original direction

            {
                arrEnemy1Bullet.direction = 1;
                arrEnemy1Bullet.enemyBulletSpr.setScale(-2.0f, 2.0f);

            }
            else
            {
                arrEnemy1Bullet.direction = -1;
                arrEnemy1Bullet.enemyBulletSpr.setScale(2.0f, 2.0f);
            }
        }

        
    }
    enemy1BulletUpdate(playerst,arrEnemy1Bullet , dt);


}
bool playerBulletUpdate(bullet windowmag[] ,player& playerst ,float dt,RenderWindow& window)
{
    bool IsFiring = false;
    for(int i = 0 ; i < nbullets ; i++)// this loop after the game loop above so that after we determined the bullet and said to shoot
    //this loop reads the array and locate the slot we want to fire , then it fire it and reset this bullet condition or bool
    {   
        
        if(windowmag[i].isthere)
        {   
            IsFiring = true;
            windowmag[i].bulletSpr.move(  windowmag[i].speed * windowmag[i].direction * dt, 0);
              
            
            windowmag[i].bulletSpr.setScale( windowmag[i].direction * 2.0f, 2.0f);
               
            
            if(windowmag[i].bulletSpr.getPosition().x > playerst.megamanSpr.getPosition().x + windowWidth ||windowmag[i].bulletSpr.getPosition().x < playerst.megamanSpr.getPosition().x - windowWidth)

            {
                windowmag[i].isthere = false;
                windowmag[i].bulletSpr.setPosition(20000.f, 20000.f);
            }
            window.draw(windowmag[i].bulletSpr);
            window.setKeyRepeatEnabled(false);// this built-in event works when the player hold the button 
            //and don't release it but we want the action to happen only once in this for loop
            
        }
    }
    return IsFiring;
}

// ## enemies detection range
void enemydetection(enemy1& arrEnemy1 , player& playerst)
{

    if(playerst.megamanSpr.getPosition().x <arrEnemy1.enemySpr.getPosition().x + arrEnemy1.detectionRange && playerst.megamanSpr.getPosition().x > arrEnemy1.enemySpr.getPosition().x - arrEnemy1.detectionRange)
    {
        arrEnemy1.isActive = true;
        if(playerst.megamanSpr.getPosition().x< arrEnemy1.enemySpr.getPosition().x)
        {
            arrEnemy1.enemySpr.setScale(2.5f , 2.5f);
        }
        else
        {
            arrEnemy1.enemySpr.setScale(-2.5f , 2.5f);
        }
    }  
    else
    {
        arrEnemy1.isActive = false;
    }

}   
void enemy2Detection(enemy2& arrEnemy2, player& playerst ,float dt)
{
    if (playerst.megamanSpr.getPosition().x < arrEnemy2.enemy2Spr.getPosition().x + arrEnemy2.detectionRange && playerst.megamanSpr.getPosition().x > arrEnemy2.enemy2Spr.getPosition().x - arrEnemy2.detectionRange)
        
    {
        arrEnemy2.isActive = true;

        arrEnemy2.enemy2Spr.move(arrEnemy2.speed * dt, 0);
        arrEnemy2.hitbox.setPosition(arrEnemy2.enemy2Spr.getPosition());
    }
    else
    {
        arrEnemy2.isActive = false; // "Sleep" if player is too far away[cite: 1]
    }
}

// ## controls input
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

}


// ## detections and intersections
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


  for (int i = 0; i < 2; i++) {
    arrEnemy1[i].enemySpr.move(0, arrEnemy1[i].Vy *dt);
    if(!arrEnemy1[i].touchesground) {
        arrEnemy1[i].Vy += gravity * dt; //vf = vi + at for proper gravity that depends on dt to streamline everything.
    }
    else {
        arrEnemy1[i].Vy = 0;
    }
  }
};

void playerhitbox_pos(player& playerst){
    playerst.hitbox.setPosition(playerst.Pos_Tracker);
}
void winIntresection(winobj& winobject, RenderWindow& window,player &playerst, bool &won, bool &isPaused){
    if(playerst.megamanSpr.getGlobalBounds().intersects(winobject.winRect.getGlobalBounds())){
        isPaused = true;
        won = true;
        
        if(winsound){
            levelmusic.stop();
            win.setBuffer(winbuffer);
            win.play();
            winsound = false;
        }
    }
}

void initwinobject(winobj& winobject,RenderWindow& window){
winobject.winRect.setPosition(winobject.winX,winobject.winY);
winobject.winRect.setOrigin(winobject.width/2,winobject.height/2);
winobject.winRect.setTexture(winobject.win);
window.draw(winobject.winRect);
}

void flickerPlayer() {
    auto curr = playerst.megamanSpr.getColor();
    playerst.megamanSpr.setColor(Color(255,255,255,(130+(curr.a%255))));
}

void flickerEnemy1(int ind) {
    auto curr = arrEnemy1[ind].enemySpr.getColor();
    arrEnemy1[ind].enemySpr.setColor(Color(255,50,50,250));
}

void flickerEnemy2(int ind) {
    auto curr = arrEnemy2[ind].enemy2Spr.getColor();
    arrEnemy2[ind].enemy2Spr.setColor(Color(255,100,100,(220+(curr.a%255))));
}


void checkPlayerInvincibility(player& playerst,float &dt){
    // If the player got hit, he gets an invincibility frame and a flickering animation for 1 second
    if (playerst.isInv==true &&playerst.invTimer>=0)
    {
        int dt100 = dt*10000000;
        int inv100 = playerst.invTimer*10000000;
        if (dt100 && (inv100/dt100)%4 == 0) flickerPlayer();
        playerst.invTimer-=dt;
    }
    else{
        playerst.megamanSpr.setColor(Color(255,255,255,255));
        playerst.isInv=false;
        playerst.invTimer=1.0f;
    }
}

void checkEnemy1Invincibility(int ind, float &dt) {
    // If the enemy got hit, he gets an invincibility frame and a reddening animation for 0.5 second
    if (arrEnemy1[ind].isInv==true && arrEnemy1[ind].invTimer>=0)
    {
        int dt100 = dt*10000000;
        int inv100 = arrEnemy1[ind].invTimer*10000000;
        if (dt100 && (inv100/dt100)%8 == 0) flickerEnemy1(ind);
        arrEnemy1[ind].invTimer-=dt;
    }
    else{
        arrEnemy1[ind].enemySpr.setColor(Color(255,255,255,255));
        arrEnemy1[ind].isInv = false;
        arrEnemy1[ind].invTimer = 0.25f;
    }
}

void checkEnemy2Invincibility(int ind, float &dt) {
}

int checkPlayerWallIntersection(int ind) {
  // Checks if the player are on the same x-axis of the ground, then they intersect vertically. Otherwise, they intersect horizontally
  // 1 = Player's left touches the wall, 2 = Player's right touches the wall
  auto start = ground[ind].gnd.getPosition().x-0.5*ground[ind].blockwidth;
  auto end = ground[ind].gnd.getPosition().x+0.5*ground[ind].blockwidth;
  if (playerst.Pos_Tracker.x-end > 0.5*playerst.framewidth) return 1;
  if (start-playerst.Pos_Tracker.x > 0.5*playerst.framewidth) return 2;
  return 0;
}

void handlePlayerIntersection(float &dt) {
  playerst.touchesground = false;
  playerst.toucheswall = player::NONE;
  playerst.issliding = false;
  checkPlayerInvincibility(playerst, dt);

  // Platfrom-Player
  for(int i = 0 ; i < blocks ; i++)
  {
    if (playerst.hitbox.getGlobalBounds().intersects(ground[i].gnd.getGlobalBounds())) 
    {
      // Wall-Player: Set player vx = 0
      if (checkPlayerWallIntersection(i) == 1) {
        playerst.toucheswall = player::LEFT;
      }
      else if (checkPlayerWallIntersection(i) == 2) {
        playerst.toucheswall = player::RIGHT;
      }
      // Ground-Player: Set player vy = 0
      else if (playerst.Vy >= 0)
      {
        playerst.touchesground = true;
      }
    }
  }
  
  // Sliding
  if (playerst.touchesground == false && playerst.toucheswall != player::NONE) playerst.issliding = true;
  
  // Enemy-player : Make the player get hit
  // With Enemy 1
  for (int i = 0; i < 2; i++) {
    if (playerst.hitbox.getGlobalBounds().intersects(arrEnemy1[i].hitbox.getGlobalBounds()) || playerst.hitbox.getGlobalBounds().intersects(arrEnemy1Bullet.enemyBulletSpr.getGlobalBounds())) {
        if (!playerst.isInv) {
            playerst.health--;
            playerst.isInv = true;
        }
    }
  }
  // With Enemy 2
  for (int i = 0; i < 2; i++) {
    if (playerst.hitbox.getGlobalBounds().intersects(arrEnemy2[i].hitbox.getGlobalBounds())) {
        if (!playerst.isInv) {
            playerst.health--;
            playerst.isInv = true;
        }
    }
  }
}

void handleEnemy1Intersection(float &dt) {
  // Platfrom-Enemy
  for (int en = 0; en < 2; en++) {
    checkEnemy1Invincibility(en, dt);
    for(int i = 0; i < blocks ; i++)
    {
        if (arrEnemy1[en].hitbox.getGlobalBounds().intersects(ground[i].gnd.getGlobalBounds())) {
            // Ground-Enemy: Set enemy vy = 0
            arrEnemy1[en].touchesground = true;
        }
    }

    // Player's bullet-Enemy
    for(int i = 0; i < 10; i++) { 
        if (arrEnemy1[en].hitbox.getGlobalBounds().intersects(windowmag[i].bulletSpr.getGlobalBounds())) {
            if (!arrEnemy1[en].isInv) {
                arrEnemy1[en].hp--;
                arrEnemy1[en].isInv = 1;
            }
        }
    } 
  }
}

void handleEnemy2Intersection(float &dt) {

}

void handleIntersection(float &dt) {
    handlePlayerIntersection(dt);
    handleEnemy1Intersection(dt);
    handleEnemy2Intersection(dt);
}

// ## health and damage 
void death_timer(player& playerst,float &dt){
    if (playerst.health>=0 &&playerst.death_timer>=0)
    {
        playerst.death_timer=playerst.death_timer-dt;
        playerst.Vx = 0.f;        
    }
    else{
        playerst.death_timer=5.0f;
        playerst.megamanSpr.setPosition(MegaSpawnX, MegaSpawnY);
        playerst.health = 19;
        playerst.Vx = storedVx;
    }
    
}
void deathHandler(player& playerst ,float &dt){
    if(playerst.health <= 0){
        death_timer(playerst, dt);
    }
    if(playerst.megamanSpr.getPosition().y > windowHeight + 100){ // if player falls off the map
        playerst.health = 0;
    }
}
//damage
//heal
void health_blockout(player& playerst,RectangleShape& blackout){
   float X=(19-playerst.health)*ratio_health;
    blackout.setScale(1,X);

}



// Platform Function syntax: createBlock(block you want to start from (Starting block), number of blocks you want to add starting from aforementioned Starting Block, startingXPosition, yPos, Spacing Between Blocks);





// ## camera and map
void camBounds(float LeftOffset, float RightOffset, float UpOffset, float DownOffset){
        //cout << Mouse::getPosition(window).x << " " << Mouse::getPosition(window).y << endl; //debugging
        const float MAP_WIDTH = 15400.f; 
        const float MAP_HEIGHT = 600.f;
        const float MAP_START_X = 200.f;
        const float MAP_START_Y = 0.f;
        //enemy1BulletUpdated map logic with limits
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
void createBlock(int index, float x, float y, float width, float height) {
    ground[index].blockwidth = width;
    ground[index].blockheight = height;
    x += width / 2.0f;

    ground[index].gnd.setSize(Vector2f(width, height));
        
    ground[index].gnd.setOrigin(width / 2.0f, height / 2.0f);

    ground[index].gnd.setPosition(x, y);
}
void carMovement(Sprite& car, float carSpeed, float dt){
    car.move(carSpeed * dt, 0);
}

void resetMegaman(player& playerst) {
    playerst.megamanSpr.setPosition(MegaSpawnX, MegaSpawnY);
    playerst.Vx = storedVx;
    playerst.health = 19;
    isPaused = false;
    won = false;
}

void volumeAdjustment(MenuData& audioMenu, float &musicVolume, float increaseAmount) {
        titlesmusic.setVolume(musicVolume);
        levelmusic.setVolume(musicVolume);
        musicVolume += increaseAmount;
            if (musicVolume > 100.0f) {
                musicVolume = 100.0f;
            }
            if (musicVolume < 0.0f) {
                musicVolume = 0.0f;
            }
}