#include "Block.h"
#include "Core.h"

Block::Block() {}

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

void Block::draw(sf::RenderWindow &mainWindow, int iX, int iY)
{
    this->aniSprite->getFrame()->draw(iX, iY, mainWindow);
}
