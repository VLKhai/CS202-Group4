#pragma once

#ifndef PLANT_H
#define PLANT_H

#include "Minion.h"

class Plant : public Minion
{
private:
	bool bWait;
	unsigned int iTime;

	bool lockPlant;

	int iLength;

	int X, Y;

public:
	Plant(int fXPos, int fYPos, int iBlockID);
	~Plant(void);

	void Update();
	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void minionPhysics();

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void collisionEffect();

	void lockMinion();

	void setMinionState(int minionState);
};

#endif