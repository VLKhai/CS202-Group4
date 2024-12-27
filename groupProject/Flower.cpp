#include "Flower.h"
#include "Core.h"

/* ******************************************** */

Flower::Flower(int iXPos, int fYPos, int iX, int iY) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)fYPos;

	this->iBlockID = 6;
	this->moveSpeed = 2;
	this->inSpawnState = true;
	this->minionSpawned = true;
	this->inSpawnY = 32;
	this->moveDirection = false;
	this->collisionOnlyWithPlayer = true;

	this->iX = iX;
	this->iY = iY;
}

Flower::~Flower(void) {

}

/* ******************************************** */

void Flower::Update() {
	if (inSpawnState) {
		if (inSpawnY <= 0) {
			inSpawnState = false;
		} else {
			if (fYPos > -5) {
				inSpawnY -= 2;
				fYPos -= 2;
			} else {
				inSpawnY -= 1;
				fYPos -= 1;
			}
		}
	}
}

bool Flower::updateMinion() {
	return minionSpawned;
}

void Flower::Draw(sf::RenderWindow& window, IMG* iIMG) {
	if(minionState >= 0) {
		iIMG->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2, false);
		if (inSpawnState) {
			Core::getMap()->getBlock(Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 9 : 56)->getSprite()->getTexture()->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + (32 - inSpawnY) - Core::getMap()->getMapBlock(iX, iY)->getYPos() + 2, false);
		}
	}
}

/* ******************************************** */

void Flower::collisionWithPlayer(bool TOP, Player* pPlayer) {
	if(!inSpawnState && minionState >= 0) {
		Core::getMap()->notify(this, "P" + std::to_string(pPlayer->getIDPlayer()) + "_LVUP");
		minionState = -1;
	}
}

void Flower::setMinionState(int minionState) { }