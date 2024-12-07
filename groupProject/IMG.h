#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class IMG {
private:
    sf::Texture *tIMG;
    sf::Sprite *sIMG;

public:
    IMG() = default;
    ~IMG(); 
    IMG(std::string GfileName, sf::RenderWindow &mainWindow);
    void setIMG(std::string fileName, sf::RenderWindow &mainWindow);
    void draw(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
    void draw(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset, bool bRotate);
    void drawVert(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
    void drawBoundingBox(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
};