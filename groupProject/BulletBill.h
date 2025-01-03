#pragma once

#ifndef BULLETBILL_H
#define BULLETBILL_H	

#include "Minion.h"

class BulletBill : public Minion
{
private:

public:
	BulletBill(int iXPos, int iYPos, bool moveDirection, int minionState);
	~BulletBill(void);

	void Update();
	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void minionPhysics();

	void collisionWithPlayer(bool TOP, Player* pPlayer);
};

#endif