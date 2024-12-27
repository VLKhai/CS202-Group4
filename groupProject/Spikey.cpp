#include "Spikey.h"
#include "Core.h"

/* ******************************************** */

Spikey::Spikey(int iXPos, int iYPos) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos + 6;

	this->iHitBoxX = 32;
	this->iHitBoxY = 26;

	this->iBlockID = 52;

	this->startJump(1);

	this->moveSpeed = 0;
}

Spikey::~Spikey(void) {

}

/* ******************************************** */

void Spikey::Update() {
	if (minionState == 0) {
		updateXPos();
	} else if(minionState == -2) {
		Minion::minionDeathAnimation();
	}
}

void Spikey::Draw(sf::RenderWindow& window, IMG* iIMG) {
	if(minionState != -2) {
		iIMG->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos - 4, !moveDirection);
	} else {
		iIMG->drawVert(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2);
	}
}

void Spikey::minionPhysics() {
	if (jumpState == 1) {
		physicsState1();
	} else {
		if (!Core::getMap()->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY, true) && !Core::getMap()->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY, true) && !onAnotherMinion) {
			physicsState2();
		} else {
			jumpState = 0;
			onAnotherMinion = false;

			if(iBlockID == 52) {
				iBlockID = 51;
				moveDirection = fXPos + iHitBoxX/2 > Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2;
				moveSpeed = 1;
			}
		}
	}
}

/* ******************************************** */

void Spikey::collisionWithPlayer(bool TOP, Player* pPlayer) {
	if(pPlayer->getStarEffect()) {
		setMinionState(-2);
	} else {
		Core::getMap()->notify(this, "Dead_1_0");
	}
}