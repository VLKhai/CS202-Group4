#include "HammerBro.h"
#include "Core.h"
#include "stdlib.h"
#include "time.h"

/* ******************************************** */

HammerBro::HammerBro(int iXPos, int iYPos) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos + 18;

	this->moveDIR = true;
	this->moveDirection = true;
	this->moveSpeed = 1;
	this->minionState = 0;

	this->iHitBoxX = 32;
	this->iHitBoxY = 48;

	this->iBlockID = Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 43 : 45;

	this->moveDistance = 16.0f;

	this->newY = true;

	srand((unsigned)time(NULL));

	this->nextJumpFrameID = rand()%175 + 125;
	this->currentJumpDistance = 0;
	this->jumpDistance = 0;

	this->hammerID = 0;
	this->nextHammerFrameID = 65;
}

HammerBro::~HammerBro(void) {

}

/* ******************************************** */

void HammerBro::Update() {
	if(minionState != -2) {
		moveDirection = !(Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX() / 2 > fXPos + iHitBoxX/2);

		if(moveDistance > 0) {
			moveDistance -= 0.5f;
			
			if(jumpState == 1) {
				fXPos += moveDIR ? -0.5f : 0.5f;
			}
			if(moveDIR) {
				if (!Core::getMap()->checkCollisionLB((int)fXPos - moveSpeed, (int)fYPos - 2, iHitBoxY, true) && !Core::getMap()->checkCollisionLT((int)fXPos - moveSpeed, (int)fYPos + 2, true)) {
					fXPos += -0.5f;
				}
			} else {
				if (!Core::getMap()->checkCollisionRB((int)fXPos + moveSpeed, (int)fYPos - 2, iHitBoxX, iHitBoxY, true) && !Core::getMap()->checkCollisionRT((int)fXPos + moveSpeed, (int)fYPos + 2, iHitBoxX, true)) {
					fXPos += 0.5f;
				}
			}
		} else {
			moveDistance = 1*32 + 16;
			moveDIR = !moveDIR;
		}

		if(currentJumpDistance > 0) {
			if(newY) {
				fYPos -= 4;
				currentJumpDistance -= 4;

			} else {
				if(jumpDistance > 0) {
					fYPos -= 4;
					jumpDistance -= 4;
				} else {
					fYPos += 4;
					currentJumpDistance -= 4;
				}
			}
		} else if(nextJumpFrameID < 0) {
			currentJumpDistance = 4*32 + 16;
			jumpDistance = 16;
			nextJumpFrameID = rand()%295 + 215;
			jumpState = 1;
			if(fYPos + iHitBoxY < CFG::GameHeight - 16 - 4*32) {
				newY = rand()%3 == 0;
			} else {
				newY = true;
			}
		} else {
			--nextJumpFrameID;
			jumpState = 2;
		}

		if(nextHammerFrameID < 15) {
			this->iBlockID = Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 44 : 46;
		} else {
			this->iBlockID = Core::getMap()->getLevelType() == 0 || Core::getMap()->getLevelType() == 4 ? 43 : 45;
		}

		if(nextHammerFrameID < 0) {
			nextHammerFrameID = rand()%95 + 95;
			Core::getMap()->addMinion(MinionFactory::addHammer((int)(fXPos + iHitBoxX/2), (int)(fYPos - 18), !moveDirection));
			if(hammerID < 2 - rand()%2) {
				++hammerID;
				nextHammerFrameID /= 4;
			} else {
				hammerID = 0;
			}
		} else {
			--nextHammerFrameID;
		}
	} else {
		Minion::minionDeathAnimation();
	}
}

void HammerBro::Draw(sf::RenderWindow& window, IMG* iIMG) {
	if(minionState != -2) {
		iIMG->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos - 18, moveDirection);
	} else {
		iIMG->drawVert(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos - 18);
	}
}

void HammerBro::minionPhysics() {
	if (jumpState == 1) {
		
	} else {
		if (!Core::getMap()->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY, true) && !Core::getMap()->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY, true)) {
			Minion::physicsState2();
		} else {
			jumpState = 0;
		}
	}
}

/* ******************************************** */

void HammerBro::collisionWithPlayer(bool TOP, Player* pPlayer) {
	if(pPlayer->getStarEffect() || TOP) {
		setMinionState(-2);
	} else {
		Core::getMap()->notify(this, "P" + std::to_string(pPlayer->getIDPlayer())+"_Dead_1_0");
	}
}

void HammerBro::collisionEffect() { }