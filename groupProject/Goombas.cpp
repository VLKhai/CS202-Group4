#include "Goombas.h"
#include "Core.h"

/* ******************************************** */

Goombas::Goombas(int iX, int iY, int iBlockID, bool moveDirection) {
	this->fXPos = (float)iX;
	this->fYPos = (float)iY;
	this->iBlockID = iBlockID;
	this->moveDirection = moveDirection;
	this->moveSpeed = 1;
}

Goombas::~Goombas(void) {

}

/* ******************************************** */

void Goombas::Update() {
	if (minionState == 0) {
		updateXPos();
	}
	else if (minionState == -2) {
		Minion::minionDeathAnimation();
	}
	else if (Core::coreClock.getElapsedTime().asMilliseconds() - 500 >= deadTime) {
		minionState = -1;
	}
}

void Goombas::Draw(sf::RenderWindow& mainWindow, IMG* iIMG) {
	if (minionState != -2) {
		iIMG->draw(mainWindow, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2, false);
	}
	else {
		iIMG->drawVert(mainWindow, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2);
	}

}

/* ******************************************** */

void Goombas::collisionWithPlayer(bool TOP, Player* pPlayer) {
	if (Core::getMap()->getPlayer()->getStarEffect()) {
		setMinionState(-2);
	}
	else if (TOP) {
		if (minionState == 0) {
			minionState = 1;
			iBlockID = Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 1 : Core::getMap()->getLevelType() == 1 ? 9 : 11;
			deadTime = Core::coreClock.getElapsedTime().asMilliseconds();
			pPlayer->resetJump();
			pPlayer->startJump(1);
			points(100);
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cSTOMP);
		}
	}
	else {
		Core::getMap()->playerDeath(true, false);  // Dead
	}
}

/* ******************************************** */

void Goombas::setMinionState(int minionState) {
	this->minionState = minionState;

	if (this->minionState == 1) {
		deadTime = Core::coreClock.getElapsedTime().asMilliseconds();
	}

	Minion::setMinionState(minionState);
}