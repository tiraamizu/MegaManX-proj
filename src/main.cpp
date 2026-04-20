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

int main()
{
  int WindowWidth =1000;
  int WindowHeight =1000;

  //window
  
  RenderWindow window = {VideoMode (WindowWidth,WindowHeight), "sfml test"};
  
  //rectanle shape

  Vector2f recLengths;
  recLengths.x=50;
  recLengths.y=25;
  RectangleShape rec;
  rec.setSize(recLengths);
  rec.setFillColor(Color::Yellow);
  //rec.setScale(3,4);
  rec.setPosition(WindowWidth/2,WindowHeight/2);
  rec.setOrigin(rec.getGlobalBounds().width/2,rec.getGlobalBounds().height/2);

  //circle shape
  
  CircleShape circle1;
 circle1.setRadius(50);
 circle1.setFillColor(Color::Blue);

 //main event

 while(window.isOpen())
  {
    Event event;

    
    while(window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window.close();
        }
        
       

    }

if(Keyboard::isKeyPressed(Keyboard::Up))
       {
        circle1.move(0,-1);
       }
       
       if(Keyboard::isKeyPressed(Keyboard::Down))
       {
        circle1.move(0,1);
       }
       
       if(Keyboard::isKeyPressed(Keyboard::Left))
       {
        circle1.move(-1,0);
       }
       
       if(Keyboard::isKeyPressed(Keyboard::Right))
       {
        circle1.move(1,0);
       }


    window.clear();
    window.draw(circle1);
    window.display();
  }

  


}