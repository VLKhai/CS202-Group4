#include "PlayerFireBallPool.h"

PlayerFireBallPool::PlayerFireBallPool()
{
	iMaxBall = 2;
	for (int i = 0; i < iMaxBall; ++i)
	{
		lPlayerFireBall.push_back(new PlayerFireBall(0, 0, 0));
	}
}

PlayerFireBallPool::~PlayerFireBallPool()
{
	for (auto& i : lPlayerFireBall)
	{
		delete i;
	}
}

PlayerFireBallPool& PlayerFireBallPool::Instance()
{
	static PlayerFireBallPool instance;
	return instance;
}

Minion* PlayerFireBallPool::getPlayerFireBall(int X, int Y, bool moveDirection)
{
	for (int i = 0; i < iMaxBall; ++i)
	{
		if (!lPlayerFireBall[i]->getActive())
		{
			lPlayerFireBall[i]->setXYDir(X, Y, moveDirection);
			//std::cout << "Have fireball " << i+1 << std::endl;
			return lPlayerFireBall[i];
		}
	}
	//std::cout << "No more fireballs" << std::endl;
	return nullptr;
}
