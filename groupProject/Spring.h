#pragma once

#ifndef SPRING_H
#define SPRING_H

#include "Minion.h"

class Spring : public Minion
{
private:
	bool inAnimation;
	int nextFrameID;

	bool extraJump;
public:
	Spring(int iXPos, int iYPos);
	~Spring(void);

	void Update();
	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void minionPhysics();

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void setMinionState(int minionState);
};

#endif