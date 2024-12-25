#pragma once

#ifndef BEETLE_H
#define BEETLE_H

#include "Minion.h"

class Beetle : public Minion
{
private:


public:
	Beetle(int iXPos, int iYPos, bool moveDirection);
	~Beetle(void);

	void Update();
	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void collisionEffect();

	void setMinionState(int minionState);
};

#endif