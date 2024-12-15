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
    void draw(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
    void draw(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset, bool bRotate);
    void draw(sf::RenderWindow& mainWindow, sf::IntRect rCrop, sf::IntRect rRect);
    void drawVert(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
    void drawBoundingBox(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);

    /* ----- get & set ----- */
    void setIMG(std::string fileName, sf::RenderWindow& mainWindow);
    sf::Texture* getIMG();
    void setColor(int iR, int iG, int iB);
};