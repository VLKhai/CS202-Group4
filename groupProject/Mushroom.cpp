#include "Mushroom.h"
#include "Core.h"

/* ******************************************** */

Mushroom::Mushroom(int iXPos, int fYPos, bool powerUP, int iX, int iY) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)fYPos - 2;

	this->iBlockID = powerUP ? 2 : Core::getMap()->getLevelType() == 1 ? 25 : 3;
	this->moveSpeed = 2;
	this->inSpawnState = true;
	this->minionSpawned = true;
	this->inSpawnY = 30;
	this->moveDirection = false;
	this->powerUP = powerUP;
	this->collisionOnlyWithPlayer = true;

	this->minionState = 0;

	this->iX = iX;
	this->iY = iY;
}

Mushroom::~Mushroom(void) {

}

/* ******************************************** */

void Mushroom::Update() {
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
	} else {
		updateXPos();
	}
}

bool Mushroom::updateMinion() {
	if (!inSpawnState) {
		minionPhysics();
	}

	return minionSpawned;
}

void Mushroom::Draw(sf::RenderWindow& window, IMG* iIMG) {
	if(minionState >= 0) {
		iIMG->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2, false);
		if (inSpawnState) {
			Core::getMap()->getBlock(Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 9 : 56)->getSprite()->getTexture()->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + (32 - inSpawnY) - Core::getMap()->getMapBlock(iX, iY)->getYPos() + 2, false);
		}
	}
}

/* ******************************************** */

void Mushroom::collisionWithPlayer(bool TOP, Player* pPlayer) {
	if (!inSpawnState && minionState == 0) {
		if (powerUP) {
			Core::getMap()->notify(this, "P" + std::to_string(pPlayer->getIDPlayer()) + "_LVUP");
		}
		else {
			Core::getMap()->notify(this, "LIVEUP");
			Core::getMap()->addPoints((int)fXPos, (int)fYPos, "1UP", 10, 14);
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cONEUP);
		}
		minionState = -1;
	}
}

void Mushroom::setMinionState(int minionState) {
	
}