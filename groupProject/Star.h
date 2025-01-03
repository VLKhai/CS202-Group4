#pragma once

#ifndef STAR_H
#define STAR_H

#include "Minion.h"

class Star : public Minion
{
private:
	bool inSpawnState;
	int inSpawnY;
	int iX, iY; // inSpawnState draw Block
public:
	Star(int iXPos, int iYPos, int iX, int iY);
	~Star(void);

	void Update();
	bool updateMinion();

	void minionPhysics();

	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void setMinionState(int minionState);
};

#endif