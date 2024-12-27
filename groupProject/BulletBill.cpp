#include "BulletBill.h"
#include "Core.h"

/* ******************************************** */

BulletBill::BulletBill(int iXPos, int iYPos, bool moveDirection, int minionState) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos - 2;

	this->moveDirection = Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/32 < fXPos + iHitBoxX/32;
	this->moveSpeed = 4;

	this->iHitBoxY = 30;

	this->collisionOnlyWithPlayer = true;

	this->iBlockID = Core::getMap()->getCurrentLevelID() == 22 ? 60 : 59;

	this->minionSpawned = true;

	this->minionState = minionState;

	this->currentJumpSpeed = (float)Core::getMap()->getBlockIDX((int)(fXPos + 16));
	this->currentFallingSpeed = (float)Core::getMap()->getBlockIDY((int)(fYPos));

	this->currentJumpDistance = (float)(Core::getMap()->getCurrentLevelID() == 17 ? 73*32 : 0); // -- MIN X
	this->jumpState = Core::getMap()->getCurrentLevelID() == 17 ? 303*32 : Core::getMap()->getMapWidth()*32; // -- MAX X

	CFG::getMusic()->PlayChunk(CFG::getMusic()->cBULLETBILL);
}

BulletBill::~BulletBill(void) {

}

/* ******************************************** */

void BulletBill::Update() {
	if(minionState != -2) {
		fXPos += moveDirection ? -moveSpeed : moveSpeed;

		if(fXPos + iHitBoxX < currentJumpDistance || fXPos > jumpState) {
			minionState = -1;
		}
	} else {
		Minion::minionDeathAnimation();
	}
}

void BulletBill::Draw(sf::RenderWindow& window, IMG* iIMG) {
	if(minionState != -2) {
		iIMG->draw(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2, moveDirection);
	} else {
		iIMG->drawVert(window, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos);
	}

	if(minionState == 0) Core::getMap()->getBlock(145)->getSprite()->getTexture()->draw(window,(int)(currentJumpSpeed*32 + Core::getMap()->getXPos()), (int)(CFG::GameHeight - 16 - currentFallingSpeed*32) + 32);
}

void BulletBill::minionPhysics() { }

/* ******************************************** */

void BulletBill::collisionWithPlayer(bool TOP, Player* pPlayer) {
	if(pPlayer->getStarEffect() || TOP) {
		setMinionState(-2);
	} else {
		Core::getMap()->notify(this, "P" + std::to_string(pPlayer->getIDPlayer())+"_Dead_1_0");
	}
}