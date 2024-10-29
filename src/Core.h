#pragma once
#include <SFML/Graphics.hpp>

class Core {
public: 
    Core();
    void mainLoop();
private:
    sf::RenderWindow mainWindow;
    sf::Event mainEvent;
};
