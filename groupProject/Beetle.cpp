#include "Beetle.h"
#include "Core.h"

/* ******************************************** */

Beetle::Beetle(int iXPos, int iYPos, bool moveDirection) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos;

	this->minionState = 0;
	this->iBlockID = Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 53 : Core::getMap()->getLevelType() == 1 ? 55 : 57;
	this->moveDirection = moveDirection;
	this->moveSpeed = 1;
}

Beetle::~Beetle(void) {

}

/* ******************************************** */

void Beetle::Update() {
	if(minionState == -2) {
		Minion::minionDeathAnimation();
	} else {
		updateXPos();
	}
}

void Beetle::Draw(sf::RenderWindow& window, IMG* iIMG) {
	if(minionState != -2) {
		iIMG->draw(window, (int)fXPos + (int) Core::getMap()->getXPos(), (int)fYPos, !moveDirection);
	} else {
		iIMG->drawVert(window, (int)fXPos + (int) Core::getMap()->getXPos(), (int)fYPos);
	}
}

/* ******************************************** */

void Beetle::collisionWithPlayer(bool TOP) {
	if(Core::getMap()->getPlayer()->getStarEffect()) {
		setMinionState(-2);
	} else if(TOP) {
		if(minionState == 0) {
			minionState = 1;

			++iBlockID;
			Core::getMap()->getPlayer()->resetJump();
			Core::getMap()->getPlayer()->startJump(1);
			Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - 4);
			points(100);
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cSTOMP);
			moveSpeed = 0;
			killOtherUnits = true;
		} else {
			if(moveSpeed > 0) {
				moveSpeed = 0;
			} else {
				if((fXPos + iHitBoxX) / 2 < (Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()) / 2) {
					moveDirection = true;
				} else {
					moveDirection = false;
				}

				moveSpeed = 6;
			}

			Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - 4);
			Core::getMap()->getPlayer()->resetJump();
			Core::getMap()->getPlayer()->startJump(1);
			points(100);
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cSTOMP);
		}
	} else {
		if(minionState == 1) {
			if(moveSpeed == 0) {
				//moveDirection = !Core::getMap()->getPlayer()->getMoveDirection();
				moveDirection = (fXPos + iHitBoxX/2 < Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2);
				if(moveDirection) fXPos -= Core::getMap()->getPlayer()->getMoveSpeed() + 1;
				else fXPos += Core::getMap()->getPlayer()->getMoveSpeed() + 1;
				moveSpeed = 6;
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cSTOMP);
			} else {
				Core::getMap()->playerDeath(true, false);
			}
		} else {
			Core::getMap()->playerDeath(true, false);
		}
	}
}

void Beetle::collisionEffect() {
	if(minionState == 0) moveDirection = !moveDirection;
}

void Beetle::setMinionState(int minionState) {
	if(minionState != -2 || Core::getMap()->getPlayer()->getStarEffect()) {
		Minion::setMinionState(minionState);
	} else {
		moveDirection = !moveDirection;
	}
}