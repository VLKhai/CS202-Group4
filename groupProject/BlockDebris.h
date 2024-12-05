#pragma once

#include <SFML/Graphics.hpp>
#include "Vector2.h"

class BlockDebris
{
private:
	int debrisState;

	Vector2* vPositionL;
	Vector2* vPositionR;
	Vector2* vPositionL2;
	Vector2* vPositionR2;

	int iFrameID;

	float fSpeedX, fSpeedY;

	bool bRotate;
public:
	BlockDebris(int iXPos, int iYPos);
	~BlockDebris();

	void update();
	void draw(sf::RenderWindow& mainWindow);

	int getDebrisState();
};