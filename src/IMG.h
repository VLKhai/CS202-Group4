#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class IMG {
private:
    sf::Texture *tIMG;
    sf::Sprite *sIMG;
public:
    IMG();
    ~IMG(); 
    IMG(std::string GfileName, sf::RenderWindow &mainWindow);
    void setIMG(std::string fileName, sf::RenderWindow &mainWindow);
    void draw(int x, int y, sf::RenderWindow &mainWindow);
};