#include "Pipe.h"
#include "Core.h"

/* ******************************************** */

Pipe::Pipe(int iType, int iLX, int iLY, int iRX, int iRY, int newPlayerPosX, int newPlayerPosY, int newCurrentLevel, int newLevelType, bool newMoveMap, int iDelay, int iSpeed, bool newUnderWater) {
	this->iType = iType;

	this->iLX = iLX;
	this->iLY = iLY;
	this->iRX = iRX;
	this->iRY = iRY;

	this->newPlayerPosX = newPlayerPosX;
	this->newPlayerPosY = newPlayerPosY;

	this->newCurrentLevel = newCurrentLevel;
	this->newLevelType = newLevelType;
	this->newMoveMap = newMoveMap;
	this->newUnderWater = newUnderWater;

	this->iDelay = iDelay;

	this->iSpeed = iSpeed;
}

/* ******************************************** */

void Pipe::checkUse() {
	if (iType == 0 || iType == 2) {
		if (Core::getMap()->getPlayer()->getSquat() && -(int)Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getXPos() >= iLX * 32 + 4 && -(int)Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX() < (iRX + 1) * 32 - 4) {
			setEvent();
		}
	}
	else {
		if (!Core::getMap()->getPlayer()->getSquat() && Core::getMap()->getBlockIDX(-(int)Core::getMap()->getXPos() + Core::getMap()->getPlayer()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX() / 2 + 2) == iRX - 1 && Core::getMap()->getBlockIDY(Core::getMap()->getPlayer()->getYPos() + Core::getMap()->getPlayer()->getHitBoxY() + 2) == iRY - 1) {
			setEvent();
		}
	}
}

/* ******************************************** */

void Pipe::setEvent() {
	Core::getMap()->getEvent()->resetData();
	Core::getMap()->getPlayer()->stopMove();
	Core::getMap()->getPlayer()->resetJump();

	CFG::getMusic()->PlayChunk(CFG::getMusic()->cPIPE);

	Core::getMap()->getEvent()->eventTypeID = Core::getMap()->getEvent()->eNormal;

	Core::getMap()->getEvent()->newCurrentLevel = newCurrentLevel;
	Core::getMap()->getEvent()->newLevelType = newLevelType;
	Core::getMap()->getEvent()->newMoveMap = newMoveMap;

	Core::getMap()->getEvent()->iSpeed = iSpeed;
	Core::getMap()->getEvent()->iDelay = iDelay;

	Core::getMap()->getEvent()->inEvent = false;

	Core::getMap()->getEvent()->newUnderWater = newUnderWater;

	Core::getMap()->getEvent()->newMapXPos = (newPlayerPosX <= 32 * 2 ? 0 : -(newPlayerPosX - 32 * 2));
	Core::getMap()->getEvent()->newPlayerXPos = (newPlayerPosX <= 32 * 2 ? newPlayerPosX : 32 * 2);
	Core::getMap()->getEvent()->newPlayerYPos = newPlayerPosY;

	if (iType == 0) { // VERTICAL -> NONE
		Core::getMap()->getEvent()->newPlayerYPos -= Core::getMap()->getPlayer()->getPowerLVL() > 0 ? 32 : 0;
		Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eBOT);
		Core::getMap()->getEvent()->vOLDLength.push_back(Core::getMap()->getPlayer()->getHitBoxY());

		Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eNOTHING);
		Core::getMap()->getEvent()->vOLDLength.push_back(35);

		for (int i = 0; i < 3; i++) {
			Core::getMap()->getEvent()->reDrawX.push_back(iLX);
			Core::getMap()->getEvent()->reDrawY.push_back(iLY - i);
			Core::getMap()->getEvent()->reDrawX.push_back(iRX);
			Core::getMap()->getEvent()->reDrawY.push_back(iRY - i);
		}
	}
	else if (iType == 1) {
		Core::getMap()->getEvent()->newPlayerXPos += 32 - Core::getMap()->getPlayer()->getHitBoxX() / 2;

		Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eRIGHT);
		Core::getMap()->getEvent()->vOLDLength.push_back(Core::getMap()->getPlayer()->getHitBoxX());

		Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eNOTHING);
		Core::getMap()->getEvent()->vOLDLength.push_back(35);

		Core::getMap()->getEvent()->vNEWDir.push_back(Core::getMap()->getEvent()->ePLAYPIPETOP);
		Core::getMap()->getEvent()->vNEWLength.push_back(1);

		Core::getMap()->getEvent()->vNEWDir.push_back(Core::getMap()->getEvent()->eNOTHING);
		Core::getMap()->getEvent()->vNEWLength.push_back(35);

		Core::getMap()->getEvent()->vNEWDir.push_back(Core::getMap()->getEvent()->eTOP);
		Core::getMap()->getEvent()->vNEWLength.push_back(Core::getMap()->getPlayer()->getHitBoxY());

		for (int i = 0; i < 3; i++) {
			Core::getMap()->getEvent()->reDrawX.push_back(iLX + i);
			Core::getMap()->getEvent()->reDrawY.push_back(iLY);
			Core::getMap()->getEvent()->reDrawX.push_back(iRX + i);
			Core::getMap()->getEvent()->reDrawY.push_back(iRY);

			Core::getMap()->getEvent()->reDrawX.push_back(Core::getMap()->getBlockIDX(Core::getMap()->getEvent()->newPlayerXPos - Core::getMap()->getEvent()->newMapXPos));
			Core::getMap()->getEvent()->reDrawY.push_back(Core::getMap()->getBlockIDY(newPlayerPosY) - 1 - i);
			Core::getMap()->getEvent()->reDrawX.push_back(Core::getMap()->getBlockIDX(Core::getMap()->getEvent()->newPlayerXPos - Core::getMap()->getEvent()->newMapXPos) + 1);
			Core::getMap()->getEvent()->reDrawY.push_back(Core::getMap()->getBlockIDY(newPlayerPosY) - 1 - i);
		}
	}
	else { // -- VERT -> VERT
		Core::getMap()->getEvent()->newPlayerXPos -= Core::getMap()->getPlayer()->getPowerLVL() > 0 ? 32 : 0 - Core::getMap()->getPlayer()->getHitBoxX() / 2;
		Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eBOT);
		Core::getMap()->getEvent()->vOLDLength.push_back(Core::getMap()->getPlayer()->getHitBoxY());

		Core::getMap()->getEvent()->vOLDDir.push_back(Core::getMap()->getEvent()->eNOTHING);
		Core::getMap()->getEvent()->vOLDLength.push_back(55);

		for (int i = 0; i < 3; i++) {
			Core::getMap()->getEvent()->reDrawX.push_back(iLX);
			Core::getMap()->getEvent()->reDrawY.push_back(iLY - i);
			Core::getMap()->getEvent()->reDrawX.push_back(iRX);
			Core::getMap()->getEvent()->reDrawY.push_back(iRY - i);

			Core::getMap()->getEvent()->reDrawX.push_back(Core::getMap()->getBlockIDX(Core::getMap()->getEvent()->newPlayerXPos - Core::getMap()->getEvent()->newMapXPos));
			Core::getMap()->getEvent()->reDrawY.push_back(Core::getMap()->getBlockIDY(newPlayerPosY) - 1 - i);
			Core::getMap()->getEvent()->reDrawX.push_back(Core::getMap()->getBlockIDX(Core::getMap()->getEvent()->newPlayerXPos - Core::getMap()->getEvent()->newMapXPos) + 1);
			Core::getMap()->getEvent()->reDrawY.push_back(Core::getMap()->getBlockIDY(newPlayerPosY) - 1 - i);
		}

		Core::getMap()->getEvent()->vNEWDir.push_back(Core::getMap()->getEvent()->ePLAYPIPETOP);
		Core::getMap()->getEvent()->vNEWLength.push_back(1);

		Core::getMap()->getEvent()->vNEWDir.push_back(Core::getMap()->getEvent()->eNOTHING);
		Core::getMap()->getEvent()->vNEWLength.push_back(35);

		Core::getMap()->getEvent()->vNEWDir.push_back(Core::getMap()->getEvent()->eTOP);
		Core::getMap()->getEvent()->vNEWLength.push_back(Core::getMap()->getPlayer()->getHitBoxY());
	}

	Core::getMap()->setInEvent(true);
}