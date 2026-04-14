#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace sf;
  const  int windowheight=800;
  const  int windowwidth=800;
   const float window_center_x=windowwidth/2;
struct ball_test{
   CircleShape circle;
   float speed=350.f;
   Vector2f tracker;
   float timer=3.0f;
   bool slowed=false;
   void update_color(ball_test* ball){
      if(ball->slowed==false){
         ball->circle.setFillColor(Color::Red);
      }
      else{ball->circle.setFillColor(Color::Blue);}
   }
      void update_speed(ball_test* ball,float dt){
      if(abs(ball->tracker.x-window_center_x)<10.f){
         ball->slowed=true;
         ball->timer=3;
      }
      if(ball->slowed==true && ball->timer>0){
         ball->timer-=dt;
         ball->speed=150;
      }
      else if(ball->timer<=0){
         ball->slowed=false;
         ball->speed=350;
      }

   }
   
};


int main(){

   ball_test test;
   test.circle.setRadius(50);
   Clock clock,cock;

    RenderWindow window ={ VideoMode(windowwidth,windowheight), "sfml"};

    while(window.isOpen()){
         float dt=clock.restart().asSeconds();
         Event event;
         while(window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }
         }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            test.circle.move(0,test.speed*dt*-1);
         }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
             test.circle.move(0,test.speed*dt*1);
         }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
          test.circle.move(test.speed*dt*-1,0);
         }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            test.circle.move(test.speed*dt*1,0);
         }
         test.tracker=test.circle.getPosition();
         test.update_color(&test);
         test.update_speed(&test,dt);
         window.clear();
         window.draw(test.circle);
         window.display();
         
      }
   }