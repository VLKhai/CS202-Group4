#include "Vine.h"
#include "Core.h"

/* ******************************************** */

Vine::Vine(int iXPos, int iYPos, int minionState, int iBlockID) {
	this->fXPos = (float)iXPos * 32 + 4;
	this->fYPos = (float)(CFG::GameHeight - 16 - iYPos * 32);

	this->iX = iXPos;
	this->iY = iYPos;

	this->minionState = minionState;
	this->iBlockID = iBlockID;

	this->moveSpeed = 0;

	this->collisionOnlyWithPlayer = true;

	this->jumpDistance = 0;
	this->minionSpawned = true;

	this->iHitBoxX = 24;
	this->iHitBoxY = -32;
}

Vine::~Vine(void) {

}

/* ******************************************** */

void Vine::Update() {
	if (minionState == 0) {
		if (CFG::GameHeight + 16 - iY * 32 >= jumpDistance) {
			jumpDistance += 2;
			iHitBoxY += 2;
			fYPos -= 2;
		}
	}
	else {
		if (jumpDistance < 256) {
			jumpDistance += 2;
			iHitBoxY += 2;
			fYPos -= 2;
		}
	}
}

void Vine::Draw(sf::RenderWindow& window, IMG* iIMG) {
	if (jumpDistance < 32) {
		Core::getMap()->getMinionBlock(iBlockID - 1)->draw(window, (int)(iX * 32 + Core::getMap()->getXPos()), (int)(CFG::GameHeight - 16 - iY * 32 - jumpDistance));
	}
	else {
		Core::getMap()->getMinionBlock(iBlockID - 1)->draw(window, (int)(iX * 32 + Core::getMap()->getXPos()), (int)(CFG::GameHeight - 16 - iY * 32 - jumpDistance));
		for (int i = 0; i < jumpDistance / 32 - 1; i++) {
			iIMG->draw(window, (int)(iX * 32 + Core::getMap()->getXPos()), (int)(CFG::GameHeight + 16 - iY * 32 + i * 32 - jumpDistance), false);
		}
	}
	Core::getMap()->getBlock(Core::getMap()->getMapBlock((int)iX, (int)iY)->getBlockID())->getSprite()->getTexture()->draw(window, (int)(iX * 32 + Core::getMap()->getXPos()), (int)(CFG::GameHeight - iY * 32 - 16 - Core::getMap()->getMapBlock((int)iX, (int)iY)->getYPos()));
}

void Vine::minionPhysics() { }

/* ******************************************** */

void Vine::collisionWithPlayer(bool TOP) {
	if (minionState == 0) {
		Core::getMap()->setInEvent(true);

		Core::getMap()->getEvent()->resetData();
		Core::getMap()->getPlayer()->resetJump();
		Core::getMap()->getPlayer()->stopMove();

		Core::getMap()->getEvent()->eventTypeID = Core::getMap()->getEvent()->eNormal;

		Core::getMap()->getEvent()->iSpeed = 2;

		Core::getMap()->getEvent()->newLevelType = Core::getMap()->getLevelType();
		Core::getMap()->getEvent()->newCurrentLevel = Core::getMap()->getCurrentLevelID();
		Core::getMap()->getEvent()->newMoveMap = true;
		Core::getMap()->getEvent()->iDelay = 0;
		Core::getMap()->getEvent()->inEvent = false;

		Core::getMap()->getEvent()->newMoveMap = true;
		Core::getMap()->getEvent()->newUnderWater = false;

		if (fXPos + iHitBoxX / 2 > Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos()) {
			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eRIGHT);
			Core::getMap()->getEvent()->vOLDLength.push_back(Core::getMap()->getPlayer()->getHitBoxX() - 4);
		}
		else {
			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eLEFT);
			Core::getMap()->getEvent()->vOLDLength.push_back(Core::getMap()->getPlayer()->getHitBoxX() - 4);
		}

		for (int i = 0; i < Core::getMap()->getPlayer()->getYPos() + Core::getMap()->getPlayer()->getHitBoxY(); i += 32) {
			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eVINE1);
			Core::getMap()->getEvent()->vOLDLength.push_back(16);
			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eVINE2);
			Core::getMap()->getEvent()->vOLDLength.push_back(16);
		}

		switch (Core::getMap()->getCurrentLevelID()) {
		case 4: {
			Core::getMap()->getEvent()->newMapXPos = -270 * 32;
			Core::getMap()->getEvent()->newPlayerXPos = 128;
			Core::getMap()->getEvent()->newPlayerYPos = CFG::GameHeight - Core::getMap()->getPlayer()->getHitBoxY();
			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eVINESPAWN);
			Core::getMap()->getEvent()->vOLDLength.push_back(274);

			break;
		}
		case 8: {
			Core::getMap()->getEvent()->newMapXPos = -270 * 32;
			Core::getMap()->getEvent()->newPlayerXPos = 128;
			Core::getMap()->getEvent()->newPlayerYPos = CFG::GameHeight - Core::getMap()->getPlayer()->getHitBoxY();
			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eVINESPAWN);
			Core::getMap()->getEvent()->vOLDLength.push_back(274);

			break;
		}
		case 13: {
			Core::getMap()->getEvent()->newMapXPos = -310 * 32;
			Core::getMap()->getEvent()->newPlayerXPos = 128;
			Core::getMap()->getEvent()->newPlayerYPos = CFG::GameHeight - Core::getMap()->getPlayer()->getHitBoxY();
			Core::getMap()->getEvent()->newLevelType = 0;

			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eVINESPAWN);
			Core::getMap()->getEvent()->vOLDLength.push_back(314);
			break;
		}
		case 17: {
			Core::getMap()->getEvent()->newMapXPos = -325 * 32;
			Core::getMap()->getEvent()->newPlayerXPos = 128;
			Core::getMap()->getEvent()->newPlayerYPos = CFG::GameHeight - Core::getMap()->getPlayer()->getHitBoxY();
			Core::getMap()->getEvent()->newLevelType = 0;

			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eVINESPAWN);
			Core::getMap()->getEvent()->vOLDLength.push_back(329);
			break;
		}
		case 21: {
			Core::getMap()->getEvent()->newMapXPos = -390 * 32;
			Core::getMap()->getEvent()->newPlayerXPos = 128;
			Core::getMap()->getEvent()->newPlayerYPos = CFG::GameHeight - Core::getMap()->getPlayer()->getHitBoxY();
			Core::getMap()->getEvent()->newLevelType = 4;

			Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eVINESPAWN);
			Core::getMap()->getEvent()->vOLDLength.push_back(394);
			break;
		}
		}

		Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eNOTHING);
		Core::getMap()->getEvent()->vOLDLength.push_back(60);

		for (int i = 0; i < 64; i += 32) {
			Core::getMap()->getEvent()->vNEWDir.push_back(Core::getMap()->getEvent()->eVINE1);
			Core::getMap()->getEvent()->vNEWLength.push_back(16);
			Core::getMap()->getEvent()->vNEWDir.push_back(Core::getMap()->getEvent()->eVINE2);
			Core::getMap()->getEvent()->vNEWLength.push_back(16);
		}

	}
	else {

	}
}