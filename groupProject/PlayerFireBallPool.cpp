#include "PlayerFireBallPool.h"
#include "Core.h"

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
		if (lPlayerFireBall[i]->getActive() == false)
		{
			lPlayerFireBall[i]->setXYDir(X, Y, moveDirection);
			this->iTimeUsed = Core::coreClock.getElapsedTime().asMilliseconds();
			//std::cout << "Have fireball " << i+1 << std::endl;
			return lPlayerFireBall[i];
		}
	}
	//std::cout << "No more fireballs" << std::endl;
	return nullptr;
}

void PlayerFireBallPool::update()
{
	if (Core::coreClock.getElapsedTime().asMilliseconds() - iTimeUsed > 70000)
	{
		for (int i = 0; i < iMaxBall; ++i)
		{
			lPlayerFireBall[i]->setActive(false);
		}
	}
}