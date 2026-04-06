#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

int main() {
    const float gravity = 3.0f;    
    

    int windowWidth, windowHeight;
    windowWidth = 1200.0f;
    windowHeight = 1000.0f;
    RenderWindow window(VideoMode(windowWidth,windowHeight), "your window");
    CircleShape circle;
    circle.setRadius(20);
    circle.setFillColor(Color::Blue);
    circle.setPosition(windowWidth/2,windowHeight/2);
    circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
    while (window.isOpen()) 
    {
        float ballBottom = circle.getPosition().y + circle.getRadius();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
            //this ::Closed works when the user click on the x to close the wondow
            {    
                window.close();
            }
            if(event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard ::Space)
                {
                    circle.setFillColor(sf::Color::Red); 
                }
                if(event.key.code == Keyboard ::Tab)
                {
                    circle.setFillColor(sf::Color::Blue); 
                }
            }
    
        }   
        if (ballBottom < windowHeight) 
        {

            circle.move(0, gravity); 
        }
        else 
        {
            // 3. Keep it exactly on the floor if it tries to go past
            circle.setPosition(circle.getPosition().x, windowHeight - circle.getRadius());
        }
        if(Keyboard::isKeyPressed(Keyboard::Right))
        {
            circle.move(1,0);
            //move() make object move a certain movement in x and y coordinates
        }
        if(Keyboard::isKeyPressed(Keyboard::Left))
        {
            circle.move(-1,0);
            
        }
        if(Keyboard::isKeyPressed(Keyboard::Up))
        {   
            circle.move(0,-5);

        }
        if(Keyboard::isKeyPressed(Keyboard::Down))
        {
            circle.move(0,1);

        }

        window.clear();
        
        window.draw(circle);
   
        window.display();
    }
    return 0;
}   