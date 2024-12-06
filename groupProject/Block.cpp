#include "Block.h"

Block::Block(int iBlockID, AniSprite *sSprite, bool bCollision, bool bDeath, bool bUse, bool bVisible)
{
    this->iBlockID = iBlockID;
    this->aniSprite = sSprite;
    this->bCollision = bCollision;
    this->bDeath = bDeath;
    this->bUse = bUse;
    this->bVisible = bVisible;
}

Block::~Block()
{
    delete this->aniSprite;
}

void Block::draw(sf::RenderWindow& mainWindow, int iXPos, int iYPos)
{
    aniSprite->getFrame()->draw(mainWindow, iXPos, iYPos);
}

int Block::getBlockID()
{
	return iBlockID;
}

void Block::setBlockID(int iID)
{
	iBlockID = iID;
}

AniSprite* Block::getAniSprite()
{
	return aniSprite;
}

bool Block::getCollision()
{
	return bCollision;
}

bool Block::getDeath()
{
	return bDeath;
}

bool Block::getUse()
{
	return bUse;
}

bool Block::getVisible()
{
    return bVisible;
}
