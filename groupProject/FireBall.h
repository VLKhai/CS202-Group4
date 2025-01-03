#pragma once

#ifndef FIREBALL_H
#define FIREBALL_H

#include "Minion.h"

class FireBall : public Minion
{
private:
	int radius; // -- r
	double angle;

	int iCenterX, iCenterY;

	double slice;

	int sliceID;

public:
	FireBall(int iXPos, int iYPos, int radius, int nSliceID, bool moveDirection);
	~FireBall(void);

	void Draw(sf::RenderWindow& window, IMG* iIMG);
	void Update();

	void updateXPos();

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void minionPhysics();
};

#endif