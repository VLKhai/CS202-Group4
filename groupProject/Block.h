#pragma once

#include "AniSprite.h"

class Block {
public:
    Block();
    Block(int iBlockID, AniSprite* sSprite, bool bCollision, bool bDeath, bool bUse, bool bVisible);
    ~Block();

	int getBlockID();
	void setBlockID(int iID);

	AniSprite* getAniSprite();
	bool getCollision();
	bool getDeath();
	bool getUse();
	bool getVisible();

private:
    AniSprite* aniSprite;

	int iBlockID;

	bool bCollision;
	bool bDeath;
	bool bUse;
	bool bVisible;
};