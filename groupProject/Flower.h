#pragma once

#ifndef FLOWER_H
#define FLOWER_H

#include "Minion.h"

class Flower : public Minion
{
private:
	bool inSpawnState;
	int inSpawnY;
	int iX, iY; // inSpawnState draw Block
public:
	Flower(int iXPos, int iYPos, int iX, int iY);
	~Flower(void);

	void Update();
	bool updateMinion();

	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void setMinionState(int minionState);
};

#endif