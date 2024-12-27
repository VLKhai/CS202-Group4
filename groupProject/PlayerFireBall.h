#pragma once

#include "Minion.h"

class PlayerFireBall : public Minion
{
private:
	bool bActive;
	bool bDestroy;
	int destroyFrameID;
public:
	PlayerFireBall(int X, int Y, bool moveDirection);
	~PlayerFireBall(void);

	void Update();
	void Draw(sf::RenderWindow& mainWindow, IMG* iIMG);

	void setFireBall(int X, int Y, bool moveDirection);

	void updateXPos();

	void minionPhysics();

	void collisionWithPlayer(bool TOP, Player* pPlayer);

	void collisionWithAnotherUnit();

	void collisionEffect();

	void setMinionState(int minionState);

	bool getDestroy();
};
