#pragma once
#include "IMG.h"
#include "AniSprite.h"

class Block {
public:
    Block();
    Block(int iBlockID, AniSprite* sSprite, bool bCollision, bool bDeath, bool bUse, bool bVisible);
    ~Block();

    void draw(sf::RenderWindow &mainWindow, int iX, int iY);
private:
    AniSprite* aniSprite;

	int iBlockID;

	bool bCollision;
	bool bDeath;
	bool bUse;
	bool bVisible;
};