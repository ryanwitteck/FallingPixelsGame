// C++ Standard Libraries
#include <iostream>
// Third Party Libraries
#include <SFML/Graphics.hpp>
// Our Library
#include "../include/SandWorld.hpp"

// Entry point into the pro/.gram

const int TOTAL_WIDTH = 500;
const int TOTAL_HEIGHT = 500;

const int PEN_SIZE = 10; // Keeping it as a square for now 

SandWorld m_world = SandWorld(TOTAL_WIDTH, TOTAL_HEIGHT); 

int main(){

    sf::RenderWindow window(sf::VideoMode(TOTAL_WIDTH,TOTAL_HEIGHT), "Witteck - Falling Sands");

    while (window.isOpen()){

        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }

		m_world.HandleInputs(window);

        window.clear();

        m_world.Render(window);

        window.display();
    }

    return 0;
}

