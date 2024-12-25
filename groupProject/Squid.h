#pragma once

#ifndef SQUID_H
#define SQUID_H

#include "Minion.h"

class Squid : public Minion
{
private:
	int moveXDistance, moveYDistance;
public:
	Squid(int iXPos, int iYPos);
	~Squid(void);
	
	void Update();
	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void minionPhysics();
	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void changeBlockID();
};

#endif