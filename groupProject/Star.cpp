#include "Star.h"
#include "Core.h"

/* ******************************************** */

Star::Star(int iXPos, int iYPos, int iX, int iY) {
	this->fXPos = (float)iXPos + 2;
	this->fYPos = (float)iYPos;

	this->collisionOnlyWithPlayer = true;

	this->moveDirection = false;
	this->moveSpeed = 3;

	this->inSpawnState = true;
	this->minionSpawned = true;
	this->inSpawnY = 30;
	this->startJumpSpeed = 4;

	this->iX = iX;
	this->iY = iY;

	this->iBlockID = 24;

	this->iHitBoxX = 28;
	this->iHitBoxY = 32;
}

Star::~Star(void) {

}

/* ******************************************** */

void Star::Update() {
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
		if(jumpState == 0) {
			startJump(1);
			jumpDistance = 32;
		}
		updateXPos();
	}
}

bool Star::updateMinion() {
	if (!inSpawnState) {
		minionPhysics();
	}

	return minionSpawned;
}

void Star::minionPhysics() {
	if (jumpState == 1) {
		if(minionState == 0) {
			updateYPos(-4 + (currentJumpDistance > 64 ? 2 : 0));
			currentJumpDistance += 2;

			if (jumpDistance <= currentJumpDistance) {
				jumpState = 2;
			}
		}
	} else {
		if (!Core::getMap()->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY, true) && !Core::getMap()->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY, true)) {
			Minion::physicsState2();
		} else {
			jumpState = 0;
		}
	}
}

void Star::Draw(sf::RenderWindow& window, IMG* iIMG) {
	iIMG->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2, false);
	if (inSpawnState) {
		Core::getMap()->getBlock(Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 9 : 56)->getSprite()->getTexture()->draw(window, (int)fXPos + (int)Core::getMap()->getXPos() - 2, (int)fYPos + (32 - inSpawnY) - Core::getMap()->getMapBlock(iX, iY)->getYPos(), false);
	}
}

/* ******************************************** */

void Star::collisionWithPlayer(bool TOP, Player* pPlayer) {
	if(!inSpawnState) {
		Core::getMap()->notify(this, "P" + std::to_string(pPlayer->getIDPlayer()) + "_star");
		minionState = -1;
	}
}

void Star::setMinionState(int minionState) { }