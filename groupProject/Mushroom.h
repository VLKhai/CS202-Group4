#pragma once

#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "Minion.h"

class Mushroom : public Minion
{
private:
	bool inSpawnState;
	int inSpawnY;
	bool powerUP;
	int iX, iY; // inSpawnState draw Block

public:
	Mushroom(int iXPos, int iYPos, bool powerUP, int iX, int iY);
	~Mushroom(void);

	void Update();
	bool updateMinion();

	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void setMinionState(int minionState);
};

#endif