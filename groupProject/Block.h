#pragma once

#include "AniSprite.h"

class Block {
public:
    Block() = default;
    Block(int iBlockID, AniSprite* sSprite, bool bCollision, bool bDeath, bool bUse, bool bVisible);
    ~Block();

	void draw(sf::RenderWindow& mainWindow, int iXPos, int iYPos);

	int getBlockID();
	void setBlockID(int iID);

	AniSprite* getAniSprite();
	bool getCollision();
	bool getDeath();
	bool getUse();
	bool getVisible();
	AniSprite* getSprite();
private:
    AniSprite* aniSprite;

	int iBlockID;

	bool bCollision;
	bool bDeath;
	bool bUse;
	bool bVisible;
};