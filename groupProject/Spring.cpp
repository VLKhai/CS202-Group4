#include "Spring.h"
#include "Core.h"

/* ******************************************** */

Spring::Spring(int iXPos, int iYPos) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos + 2;

	this->iHitBoxX = 32;
	this->iHitBoxY = 72;

	this->iBlockID = Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 37 : 40;

	this->minionState = 0;
	this->nextFrameID = 4;
	this->inAnimation = false;
}

Spring::~Spring(void) {

}

/* ******************************************** */

void Spring::Update() {
	if (inAnimation) {
		if (CFG::keySpace) {
			extraJump = true;
			Core::getMap()->getPlayer()->resetJump();
			Core::getMap()->getPlayer()->setNextFallFrameID(16);
		}
		else {
			Core::getMap()->getPlayer()->stopMove();
		}

		Core::getMap()->getPlayer()->setSpriteID(5);
		if (nextFrameID <= 0) {
			switch (minionState) {
			case 0:
				iBlockID = iBlockID == 37 ? 38 : 41;
				minionState = 1;
				Core::getMap()->getPlayer()->setYPos(Core::getMap()->getPlayer()->getYPos() + 16.0f);
				break;
			case 1:
				iBlockID = iBlockID == 38 ? 39 : 42;
				minionState = 2;
				Core::getMap()->getPlayer()->setYPos(Core::getMap()->getPlayer()->getYPos() + 16.0f);
				break;
			case 2:
				iBlockID = iBlockID == 39 ? 38 : 41;
				minionState = 3;
				Core::getMap()->getPlayer()->setYPos(Core::getMap()->getPlayer()->getYPos() - 16.0f);
				break;
			case 3:
				iBlockID = iBlockID == 38 ? 37 : 40;
				minionState = 0;
				Core::getMap()->getPlayer()->setYPos(Core::getMap()->getPlayer()->getYPos() - 16.0f);
				Core::getMap()->getPlayer()->resetJump();
				Core::getMap()->getPlayer()->startJump(4 + (extraJump ? 5 : 0));
				Core::getMap()->getPlayer()->setSpringJump(true);
				Core::getMap()->getPlayer()->startMove();
				if (extraJump) Core::getMap()->getPlayer()->setCurrentJumpSpeed(10.5f);
				inAnimation = false;
				break;
			}
			nextFrameID = 4;
		}
		else {
			--nextFrameID;
		}
	}
}

void Spring::Draw(sf::RenderWindow& window, IMG* iIMG) {
	iIMG->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos, false);
}

void Spring::minionPhysics() { }

/* ******************************************** */

void Spring::collisionWithPlayer(bool TOP) {
	if (!inAnimation) {
		if (TOP && Core::getMap()->getPlayer()->getJumpState() == 2) {
			Core::getMap()->getPlayer()->stopMove();
			Core::getMap()->getPlayer()->resetJump();
			Core::getMap()->getPlayer()->setNextFallFrameID(16);
			inAnimation = true;
			extraJump = false;
			CFG::keySpace = false;
		}
		else {
			if (Core::getMap()->getPlayer()->getMoveDirection()) {
				Core::getMap()->getPlayer()->setXPos((float)Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getPlayer()->getMoveSpeed());
			}
			else {
				Core::getMap()->getPlayer()->setXPos((float)Core::getMap()->getPlayer()->getXPos() + Core::getMap()->getPlayer()->getMoveSpeed());
			}
		}
	}
}

void Spring::setMinionState(int minionState) { }