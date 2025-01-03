#pragma once

#ifndef POINTS_H
#define POINTS_H

#include <string>
#include <SFML/Graphics.hpp>
class Points
{
private:
	int iXPos, iYPos, iYLEFT;

	std::string sText;

	int iW, iH;

	bool bDelete;
public:
	Points(int iXPos, int iYPos, std::string sText);
	Points(int iXPos, int iYPos, std::string sText, int iW, int iH);
	~Points(void);

	void Update();
	void Draw(sf::RenderWindow& mainWindow);

	bool getDelete();
};

#endif