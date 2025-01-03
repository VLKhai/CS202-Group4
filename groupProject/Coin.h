#pragma once

#ifndef COIN_H
#define COIN_H

#include <SFML/Graphics.hpp>

class Coin
{
private:
	int iXPos, iYPos;
	int iLEFT;

	int iSpriteID;
	int iStepID;

	bool bTOP;

	bool bDelete;
public:
	Coin(int iXPos, int iYPos);
	~Coin(void);

	void Update();
	void Draw(sf::RenderWindow& window);

	int getXPos();
	int getYPos();
	bool getDelete();
};

#endif