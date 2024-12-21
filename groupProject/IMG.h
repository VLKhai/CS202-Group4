#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class IMG {
private:
    sf::Texture *tIMG;
    sf::Sprite *sIMG;
    int xHitBox, yHitBox;
public:
    IMG();
    ~IMG(); 
    IMG(std::string GfileName, sf::RenderWindow &mainWindow, std::string sType = ".bmp");
    
    void draw(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
    void draw(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset, bool bRotate);
    void draw(sf::RenderWindow& mainWindow, sf::IntRect rCrop, sf::IntRect rRect);
    void drawVert(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
	void drawFromCenter(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
	void drawFromBotCen(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);
    void drawBoundingBox(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset);

    /* ----- get & set ----- */
    void setIMG(std::string fileName, sf::RenderWindow& mainWindow, std::string sType=".bmp");
    sf::Texture* getIMG();
    void setColor(int iR, int iG, int iB);

	int getHitBoxX();
	int getHitBoxY();
};