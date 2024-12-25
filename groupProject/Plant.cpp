#include "Plant.h"
#include "Core.h"

/* ******************************************** */

Plant::Plant(int fXPos, int fYPos, int iBlockID) {
	this->fXPos = (float)fXPos + 2;
	this->fYPos = (float)fYPos + 6;

	this->iHitBoxX = 28;
	this->iHitBoxY = 38;

	this->iBlockID = iBlockID;

	this->bWait = true;
	this->iTime = Core::coreClock.getElapsedTime().asMilliseconds();

	this->lockPlant = false;

	// -- true = TOP, false = BOT
	this->moveDirection = true;

	this->iLength = 50;

	this->X = Core::getMap()->getBlockIDX((int)fXPos);
	this->Y = Core::getMap()->getBlockIDY((int)fYPos);
}

Plant::~Plant(void) {

}

/* ******************************************** */

void Plant::Update() {
	lockPlant = Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() >= fXPos - 48 && Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() <= fXPos + iHitBoxX + 48;

	if(bWait) {
		if((!lockPlant || !moveDirection) && iTime + (moveDirection ? 1000 : 985) <= Core::coreClock.getElapsedTime().asMilliseconds()) {
			bWait = false;
		} else if(lockPlant && moveDirection) {
			iTime = Core::coreClock.getElapsedTime().asMilliseconds();
		}
	} else if(moveDirection) {
		if(iLength > 0) {
			fYPos -= 1;
			iLength -= 1;
		} else {
			moveDirection = false;
			iLength = 50;
			bWait = true;
			iTime = Core::coreClock.getElapsedTime().asMilliseconds();
		}
	} else {
		if(iLength > 0) {
			fYPos += 1;
			iLength -= 1;
		} else if(!lockPlant) {
			moveDirection = true;
			iLength = 50;
			bWait = true;
			iTime = Core::coreClock.getElapsedTime().asMilliseconds();
		}
	}
}

void Plant::Draw(sf::RenderWindow& window, IMG* iIMG) {
	iIMG->draw(window, (int)(fXPos + Core::getMap()->getXPos()) - 2, (int)fYPos - 6, false);

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			Core::getMap()->getBlock(Core::getMap()->getMapBlock(X + j, Y - i)->getBlockID())->draw(window, (int)((X + j) * 32 + Core::getMap()->getXPos()), CFG::GameHeight - 32 * (Y - i) - 16);
		}
	}
}

void Plant::minionPhysics() {

}

/* ******************************************** */

void Plant::collisionWithPlayer(bool TOP, Player* pPlayer) {
	if(pPlayer->getStarEffect()) {
		minionState = -1;
		points(200);
	} else {
		Core::getMap()->playerDeath(true, false);
	}
}

void Plant::collisionEffect() { }

void Plant::lockMinion() {
	if(moveDirection) {
		fYPos += 50 - iLength;
	} else {
		fYPos += iLength;
	}
	moveDirection = true;
	iLength = 50;
	lockPlant = true;
	bWait = true;
	iTime = Core::coreClock.getElapsedTime().asMilliseconds();
}

void Plant::setMinionState(int minionState) {
	if(minionState < 0) {
		this->minionState = -1;
		points(200);
	} else {
		this->minionState = minionState;
	}
}

/* ******************************************** */