#pragma once

#ifndef FIRE_H
#define FIRE_H

#include "Minion.h"

class Fire : public Minion
{
private:
	// -- minionState, 0 = Alive, -1 = Dead

	int toYPos;
public:
	Fire(float fXPos, float fYPos, int toYPos);
	~Fire(void);

	void Update();
	void Draw(sf::RenderWindow& window, IMG* iIMG);
	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void minionPhysics();
};

#endif