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
	std::cout << lPlayerFireBall.size() << std::endl;
	for (auto& ball : lPlayerFireBall)
	{	
		if (!ball->getDestroy()) {
			ball->setXYDir(X, Y, moveDirection);
			return ball;
		}
	}
	std::cout << "No more fireballs" << std::endl;
	return nullptr;
}
