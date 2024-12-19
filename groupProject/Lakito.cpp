#include "Lakito.h"
#include "Core.h"
#include "stdlib.h"
#include "time.h"

/* ******************************************** */

Lakito::Lakito(int iXPos, int iYPos, int iMaxXPos) {
	this->fXPos = (float)iXPos + 16;
	this->fYPos = (float)iYPos;

	this->iMaxXPos = iMaxXPos;
	this->followPlayer = false;
	this->end = false;

	this->iBlockID = 50;

	this->collisionOnlyWithPlayer = true;
	this->iHitBoxX = 32;
	this->iHitBoxY = 26;

	this->moveDirection = true;

	this->nextSpikeyFrameID = 128;

	this->minionSpawned = true;

	srand((unsigned)time(NULL));
}

Lakito::~Lakito(void) {

}

/* ******************************************** */

void Lakito::Update() {
	if(minionState == -2) {
		Minion::minionDeathAnimation();
	} else {
		if(fXPos > iMaxXPos) {
			end = true;
		}
		
		if(end) {
			fXPos -= 1;

			if(fXPos < -iHitBoxX) {
				minionState = -1;
			}

			return;
		}

		if(fXPos < -Core::getMap()->getXPos() - 64) {
			fXPos = -Core::getMap()->getXPos() - 32;
		}
		
		if(fXPos <= Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 + 32*Core::getMap()->getPlayer()->getMoveSpeed() && Core::getMap()->getPlayer()->getMove() && Core::getMap()->getPlayer()->getMoveDirection()) {
			moveSpeed = Core::getMap()->getPlayer()->getMoveSpeed() + (fXPos < Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 + 32*Core::getMap()->getPlayer()->getMoveSpeed());
			fXPos += moveSpeed;
			followPlayer = true;
			moveDirection = true;
		} else {
			if(followPlayer) {
				if(moveSpeed > 0) {
					fXPos += moveSpeed;
					--moveSpeed;
				} else {
					followPlayer = false;
				}
			} else {
				if(!moveDirection) {
					fXPos -= 1;

					if(fXPos < Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 - 128) {
						moveDirection = true;
					}
				} else {
					fXPos += 1;
					if(fXPos > Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 + 128) {
						moveDirection = false;
					}
				}
			}
		}

		if(nextSpikeyFrameID < rand()%85) {
			iBlockID = 49;
		}

		if(nextSpikeyFrameID <= 0) {
			//Core::getMap()->addSpikey((int)fXPos, (int)(fYPos - 32));
			nextSpikeyFrameID = 135 + rand()%175;
			iBlockID = 50;
		} else {
			--nextSpikeyFrameID;
		}
	}
}

void Lakito::Draw(sf::RenderWindow& window, IMG* iIMG) {
	if(minionState != -2) {
		iIMG->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos - 16, !moveDirection);
	} else {
		iIMG->drawVert(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos - 16);
	}
}

void Lakito::minionPhysics() { }

/* ******************************************** */

void Lakito::collisionWithPlayer(bool TOP) {
	if(Core::getMap()->getPlayer()->getStarEffect() || TOP) {
		setMinionState(-2);
	} else if(!end) {
		Core::getMap()->playerDeath(true, false);
	}
}