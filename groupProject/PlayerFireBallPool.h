#pragma once

#include "PlayerFireBall.h"

class PlayerFireBallPool
{
private:
	std::vector<Minion*> lPlayerFireBall;
	int iMaxBall;

	PlayerFireBallPool();
public:
	~PlayerFireBallPool();

	static PlayerFireBallPool& Instance();
	Minion* getPlayerFireBall(int X, int Y, bool moveDirection);
};