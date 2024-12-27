#include "Squid.h"
#include "Core.h"
#include "stdlib.h"
#include "time.h"

/* ******************************************** */

Squid::Squid(int iXPos, int iYPos) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos;

	this->iHitBoxX = 32;
	this->iBlockID = 29;

	this->minionState = 0;

	this->moveDirection = false;
	this->moveSpeed = 2;

	this->moveXDistance = 96;
	this->moveYDistance = 32;

	this->collisionOnlyWithPlayer = true;

	srand((unsigned)time(NULL));
}

Squid::~Squid(void) {

}

/* ******************************************** */

void Squid::Update() {
	if(Core::getMap()->getUnderWater()) {
		if(moveXDistance <= 0) {
			if(moveYDistance > 0) {
				fYPos += 1;
				moveYDistance -= 1;
				if(moveYDistance == 0) {
					changeBlockID();
				}
			} else {
				if(fYPos + 52 > Core::getMap()->getPlayer()->getYPos()) {
					moveDirection = Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 > fXPos;
					moveXDistance = 96 + rand()%32;
					changeBlockID();
				} else {
					fYPos += 1;
				}
			}
		} else {
			if(moveXDistance == 64) changeBlockID();
			fXPos += moveDirection ? 2 : -2;

			if(fYPos > CFG::GameHeight - 12*32 - 4) {
				fYPos -= 2;
			}
			moveXDistance -= 2;
			if(moveXDistance <= 0) {
				changeBlockID();
				moveYDistance = 32;
			}
		}
	}
}

void Squid::Draw(sf::RenderWindow& window, IMG* iIMG) {
	iIMG->draw(window,(int)(fXPos + Core::getMap()->getXPos()), (int)fYPos);
}

void Squid::minionPhysics() { }

/* ******************************************** */

void Squid::collisionWithPlayer(bool TOP, Player* pPlayer) {
	Core::getMap()->notify(this, "P" + std::to_string(pPlayer->getIDPlayer())+"_Dead_1_0");
}

void Squid::changeBlockID() {
	switch(iBlockID) {
		case 28:
			this->iBlockID = 29;
			this->iHitBoxY = 28;
			break;
		default:
			this->iBlockID = 28;
			this->iHitBoxY = 28;
			break;
	}
}