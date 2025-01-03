#include "Fire.h"
#include "Core.h"

/* ******************************************** */

Fire::Fire(float fXPos, float fYPos, int toYPos) {
	this->fXPos = fXPos;
	this->fYPos = fYPos;
	this->toYPos = toYPos;

	this->collisionOnlyWithPlayer = true;

	this->moveDirection = true;

	this->iBlockID = 22;
	this->iHitBoxX = 48;
	this->iHitBoxY = 10;

	this->minionSpawned = true;
}

Fire::~Fire(void) {

}

/* ******************************************** */

void Fire::Update() {
	fXPos -= 2;

	if(fXPos + iHitBoxX < 0) {
		minionState = -1;
	}

	if(toYPos > fYPos) {
		++fYPos;
	} else if(toYPos < fYPos) {
		--fYPos;
	}
}

void Fire::Draw(sf::RenderWindow& window, IMG* iIMG) {
	iIMG->draw(window, (int)(fXPos + Core::getMap()->getXPos()), (int)fYPos - 4);
}

void Fire::minionPhysics() {

}

/* ******************************************** */

void Fire::collisionWithPlayer(bool TOP, Player* pPlayer) {
	Core::getMap()->notify(this, "P" + std::to_string(pPlayer->getIDPlayer())+"_Dead_1_0");
}