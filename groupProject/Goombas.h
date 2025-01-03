#pragma once

#ifndef GOOMBAS_H
#define GOOMBAS_H

#include "Minion.h"

class Goombas : public Minion
{
private:
	// ----- MinionState, 0 = Alive, 1 = Dead, -1 = Destroy
public:
	Goombas(int iX, int iY, int iBlockID, bool moveDirection);
	~Goombas(void);

	void Update();
	void Draw(sf::RenderWindow& mainWindow, IMG* iIMG);

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void setMinionState(int minionState);
};


#endif 