#pragma once
#include "IMG.h"

IMG::IMG(std::string fileName, sf::RenderWindow &mainWindow)
{
    this->setIMG(fileName, mainWindow);
}

IMG::IMG() {}

IMG::~IMG()
{
    delete this->tIMG;
    delete this->sIMG;
}

void IMG::setIMG(std::string fileName, sf::RenderWindow &mainWindow)
{
    fileName = "../files/images/" + fileName + ".bmp";
    this->tIMG = new sf::Texture();
    if (!this->tIMG->loadFromFile(fileName)) {
        std::cout << "Error loading image: " << fileName << std::endl;
        return;
    };
    
    this->sIMG = new sf::Sprite(*this->tIMG);
}

void IMG::draw(int x, int y, sf::RenderWindow &mainWindow)
{
    this->sIMG->setPosition(x, y);
    mainWindow.draw(*this->sIMG);
}
