#pragma once

#include "CFG.h"
#include "IMG.h"

class Player;

class Minion
{
public:
	Minion(void);
	~Minion(void) = default;

	int minionState;

	bool killOtherUnits;

	int iBlockID;
	float fXPos, fYPos;
	int iHitBoxX, iHitBoxY;
	bool minionSpawned;
	bool collisionOnlyWithPlayer;

	int deadTime;

	bool onAnotherMinion;

	// ----- true = LEFT, false = RIGHT
	bool moveDirection;
	int moveSpeed;

	int jumpState;

	float startJumpSpeed;
	float currentJumpSpeed;
	float jumpDistance;
	float currentJumpDistance;
	float currentFallingSpeed;

	// ---------- Methods
	virtual void Update();
	virtual void Draw(sf::RenderWindow& MainWindow, IMG* iIMG);
	void drawBoundingBox(sf::RenderWindow& MainWindow);

	virtual void updateYPos(int iN);
	virtual void updateXPos();

	virtual bool updateMinion();
	virtual void minionPhysics();

	virtual void collisionEffect();

	bool checkVerticalOverlap(float leftX, float rightX);
	bool checkHorizontalOverlap(float topY, float botY);
	bool checkHorizontalTopOverlap(float botY);

	virtual void minionDeathAnimation();

	void physicsState1();
	void physicsState2();

	void Spawn();
	void startJump(int iH);
	void resetJump();

	void killMinion();

	virtual void setXYDir(int X, int Y, bool Dir) {};

	// ----- COLLISON

	virtual void collisionWithPlayer(bool TOP, Player* pPlayer);
	virtual void points(int iPoints);

	virtual void collisionWithAnotherUnit(); // -- PLAYERFIREBALL

	virtual void lockMinion();

	// ----- get & set -----
	int getBloockID();
	void setBlockID(int iBlockID);
	int getMinionState();

	virtual void setMinionState(int minionState);
	virtual bool getPowerUP();

	int getXPos();
	int getYPos();
	void setYPos(int iYPos);

	bool isAlive();

	virtual bool getDestroy() { return false; };
};