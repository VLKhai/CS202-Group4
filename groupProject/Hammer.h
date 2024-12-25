#pragma once

#ifndef HAMMER_H
#define HAMMER_H

#include "Minion.h"

class Hammer : public Minion
{
private:

public:
	Hammer(int iXPos, int iYPos, bool moveDirection);
	~Hammer(void);

	void Update();
	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void minionPhysics();

	void collisionWithPlayer(bool TOP, Player* pPlayer);
};

#endif