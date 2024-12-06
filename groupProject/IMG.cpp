#pragma once
#include "IMG.h"

IMG::IMG(std::string fileName, sf::RenderWindow &mainWindow)
{
    this->setIMG(fileName, mainWindow);
}

IMG::~IMG()
{
    delete this->tIMG;
    delete this->sIMG;
}

void IMG::setIMG(std::string fileName, sf::RenderWindow& mainWindow) {
    fileName = "files/images/" + fileName + ".bmp";

	// Handle delete background magenta color
    sf::Image image;
    if (!image.loadFromFile(fileName)) {
        std::cout << "Error loading image: " << fileName << std::endl;
        return;
    }
    // Set magenta (255, 0, 255) to be transparent
    sf::Color transparentColor = sf::Color(255, 0, 255);
    for (unsigned int x = 0; x < image.getSize().x; ++x) {
        for (unsigned int y = 0; y < image.getSize().y; ++y) {
            if (image.getPixel(x, y) == transparentColor) {
                image.setPixel(x, y, sf::Color(0, 0, 0, 0)); // Set to transparent
            }
        }
    }

	// Load image to texture
    this->tIMG = new sf::Texture();
    if (!this->tIMG->loadFromImage(image)) {
        std::cout << "Error creating texture from image: " << fileName << std::endl;
        return;
    }
    this->sIMG = new sf::Sprite(*this->tIMG);
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
	    drawBoundingBox(mainWindow, iXOffset, iYOffset);
        return;
        
    }
    sIMG->setScale(-1.f, 1.f);
	sIMG->setPosition(iXOffset + sIMG->getGlobalBounds().width, iYOffset);
	mainWindow.draw(*sIMG);
	drawBoundingBox(mainWindow, iXOffset, iYOffset);
    sIMG->setScale(1, 1);
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