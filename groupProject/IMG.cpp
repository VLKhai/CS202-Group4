#pragma once
#include "IMG.h"

IMG::IMG(std::string fileName, sf::RenderWindow &mainWindow, std::string sType)
{
    this->setIMG(fileName, mainWindow, sType);
}

IMG::IMG(){
    this->tIMG = new sf::Texture();
    this->sIMG = new sf::Sprite();
}

IMG::~IMG()
{
    delete this->tIMG;
    delete this->sIMG;
}

void IMG::setIMG(std::string fileName, sf::RenderWindow& mainWindow, std::string sType) {
    fileName = "files/images/" + fileName + sType;

	// Load image to texture
    this->tIMG = new sf::Texture();
    if (!this->tIMG->loadFromFile(fileName)) {
        std::cout << "Error creating texture from image: " << fileName << std::endl;
        return;
    }
    this->sIMG = new sf::Sprite(*this->tIMG);

	xHitBox = sIMG->getGlobalBounds().width;
	yHitBox = sIMG->getGlobalBounds().height;
}

void IMG::draw(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset)
{
    sIMG->setPosition(iXOffset, iYOffset);
    mainWindow.draw(*sIMG);
}

void IMG::draw(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset, bool bRotate)
{
  
    if (!bRotate) {
        sIMG->setPosition(iXOffset, iYOffset);
        mainWindow.draw(*sIMG);
        return;
        
    }
    sIMG->setScale(-1.f, 1.f);
	sIMG->setPosition(iXOffset + sIMG->getGlobalBounds().width, iYOffset);
	mainWindow.draw(*sIMG);
    sIMG->setScale(1, 1);    
}


void IMG::draw(sf::RenderWindow& mainWindow, sf::IntRect rCrop, sf::IntRect rRect) {
    if (tIMG && sIMG) {
        sIMG->setTexture(*tIMG);
        sIMG->setTextureRect(rCrop);
        sIMG->setPosition(static_cast<float>(rRect.left), static_cast<float>(rRect.top));
        sIMG->setScale(
            static_cast<float>(rRect.width) / rCrop.width,
            static_cast<float>(rRect.height) / rCrop.height
        );
        mainWindow.draw(*sIMG);
    }
    else {
        std::cerr << "Error: tIMG or sIMG is not initialized in IMG::draw." << std::endl;
    }
}

void IMG::drawVert(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset) {
    if (!tIMG) return; // Ensure the texture is valid
    sIMG->setScale(-1.f, 1.f);
    sIMG->setPosition(iXOffset + sIMG->getGlobalBounds().width, iYOffset);
    mainWindow.draw(*sIMG);
    sIMG->setScale(1, 1);
}

void IMG::drawFromCenter(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset)
{
	sIMG->setOrigin(sIMG->getGlobalBounds().width / 2, sIMG->getGlobalBounds().height / 2);
	sIMG->setPosition(iXOffset, iYOffset);
	mainWindow.draw(*sIMG);
	sIMG->setOrigin(0, 0);
}

void IMG::drawFromBotCen(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset)
{
	sIMG->setOrigin(sIMG->getGlobalBounds().width / 2, sIMG->getGlobalBounds().height);
	sIMG->setPosition(iXOffset, iYOffset);
	mainWindow.draw(*sIMG);
	sIMG->setOrigin(0, 0);
}

void IMG::drawBoundingBox(sf::RenderWindow& mainWindow, int iXOffset, int iYOffset) {
    sf::FloatRect bounds = sIMG->getGlobalBounds();
    sf::RectangleShape boundingBox(sf::Vector2f(bounds.width, bounds.height));
    boundingBox.setPosition(bounds.left + iXOffset, bounds.top + iYOffset);
    boundingBox.setFillColor(sf::Color::Transparent); // Make the inside transparent
    boundingBox.setOutlineColor(sf::Color::Green); // Set the outline color
    boundingBox.setOutlineThickness(1.f); // Set the outline thickness
    mainWindow.draw(boundingBox);
}


sf::Texture* IMG::getIMG() {
    return tIMG;
}

void IMG::setColor(int iR, int iG, int iB){
    sIMG->setColor(sf::Color(iR, iG, iB));
}

int IMG::getHitBoxX()
{
    return xHitBox;
}

int IMG::getHitBoxY()
{
	return yHitBox;
}
