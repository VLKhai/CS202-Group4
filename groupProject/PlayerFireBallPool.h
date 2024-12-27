#pragma once

#include "PlayerFireBall.h"

class PlayerFireBallPool
{
private:
	std::vector<Minion*> lPlayerFireBall;
	int iMaxBall;
	int iTimeUsed;

	PlayerFireBallPool();
	PlayerFireBallPool(const PlayerFireBallPool&) = delete;
	PlayerFireBallPool& operator=(const PlayerFireBallPool&) = delete;
public:
	~PlayerFireBallPool();

	static PlayerFireBallPool& Instance();
	Minion* getPlayerFireBall(int X, int Y, bool moveDirection);
	void update();
};