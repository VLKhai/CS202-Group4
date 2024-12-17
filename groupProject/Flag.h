#pragma once

#include "IMG.h"

class Flag
{
private:

public:
	Flag(int iXPos, int iYPos);
	~Flag(void) = default;

	int iXPos, iYPos;
	int iBlockID;

	int iYTextPos;
	int iPoints;

	int castleFlagExtraXPos;
	int castleFlagY;

	void Update();
	void UpdateCastleFlag();
	void draw(sf::RenderWindow& window, IMG* iIMG);
	void drawCastleFlag(sf::RenderWindow& window, IMG* iIMG);
};
