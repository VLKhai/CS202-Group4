#pragma once

#ifndef TEXT_H
#define TEXT_H

#include "IMG.h"

class Text
{
private:
	IMG* FONT;

	sf::IntRect rCrop;
	sf::IntRect rRect;

	int fontSize;
	int extraLeft, nextExtraLeft;

	// ----- METHODS
	int getPos(int iChar);
	void checkExtra(int iChar);
public:
	Text();
	~Text();

	void Draw(sf::RenderWindow& mainWindow, std::string sText, int X, int Y, int fontSize = 16);
	void Draw(sf::RenderWindow& mainWindow, std::string sText, int X, int Y, int fontSize, int iR, int iG, int iB);
	void DrawCenterX(sf::RenderWindow& mainWindow, std::string sText, int Y, int fontSize = 16, int iR = 255, int iG = 255, int iB = 255);
	void Draw(sf::RenderWindow& mainWindow, std::string sText, int X, int Y, int iWidth, int iHeight);
	void DrawWS(sf::RenderWindow& mainWindow, std::string sText, int X, int Y, int iR, int iG, int iB, int fontSize = 16);

	int getTextWidth(std::string sText, int fontSize = 16);

	// ----- SET FONT IMG
	void setFont(sf::RenderWindow& mainWindow, std::string fileName);
};

#endif