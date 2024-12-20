#include "Event.h"
#include "Core.h"

Event::Event(void) {
    this->iDelay = 0;
    this->newUnderWater = false;
    this->endGame = false;
    this->iTime = 0;
    this->bState = true;
    this->stepID = 0;
}

void Event::Normal() {
	if (bState) {
		if (vOLDDir.size() > stepID) {
			if (vOLDLength[stepID] > 0) {
				switch (vOLDDir[stepID]) {
				case eTOP: // TOP
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					break;
				case eBOT:
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() + iSpeed);
					vOLDLength[stepID] -= iSpeed;
					break;
				case eRIGHT:
					Core::getMap()->getPlayer()->setXPos((float)Core::getMap()->getPlayer()->getXPos() + iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->moveAnimation();
					Core::getMap()->getPlayer()->setMoveDirection(true);
					break;
				case eRIGHTEND:
					Core::getMap()->setXPos((float)Core::getMap()->getXPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->moveAnimation();
					Core::getMap()->getPlayer()->setMoveDirection(true);
					break;
				case eLEFT:
					Core::getMap()->getPlayer()->setXPos((float)Core::getMap()->getPlayer()->getXPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->moveAnimation();
					Core::getMap()->getPlayer()->setMoveDirection(false);
					break;
				case eBOTRIGHTEND: // BOT & RIGHT
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() + iSpeed);
					Core::getMap()->setXPos((float)Core::getMap()->getXPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->moveAnimation();
					break;
				case eENDBOT1:
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() + iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setSpriteID(10);
					break;
				case eENDBOT2:
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setMoveDirection(false);
					break;
				case eENDPOINTS:
					if (Core::getMap()->getMapTime() > 0) {
						Core::getMap()->setMapTime(Core::getMap()->getMapTime() - 1);
						Core::getMap()->getPlayer()->setScore(Core::getMap()->getPlayer()->getScore() + 50);
						if (CFG::getMusic()->musicStopped) {
							CFG::getMusic()->PlayMusic(CFG::getMusic()->mSCORERING);
						}
					}
					else {
						vOLDLength[stepID] = 0;
						CFG::getMusic()->StopMusic();
					}
					Core::getMap()->getFlag()->UpdateCastleFlag();
					break;
				case eDEATHNOTHING:
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setSpriteID(0);
					break;
				case eDEATHTOP: // DEATH TOP
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setSpriteID(0);
					break;
				case eDEATHBOT: // DEATH BOT
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() + iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setSpriteID(0);
					break;
				case eNOTHING: // NOTHING YAY
					vOLDLength[stepID] -= 1;
					break;
				case ePLAYPIPERIGHT:
					Core::getMap()->getPlayer()->setXPos((float)Core::getMap()->getPlayer()->getXPos() + iSpeed);
					vOLDLength[stepID] -= 1;
					CFG::getMusic()->PlayChunk(CFG::getMusic()->cPIPE);
					break;
				case eLOADINGMENU:
					vOLDLength[stepID] -= 1;

					if (vOLDLength[stepID] < 2) {
						Core::getMap()->setInEvent(false);
						inEvent = false;
						Core::getMap()->getPlayer()->stopMove();

						CFG::getMenuManager()->getLoadingMenu()->loadingType = true;
						CFG::getMenuManager()->getLoadingMenu()->updateTime();
						CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGameLoading);
					}
					break;
				case eGAMEOVER:
					vOLDLength[stepID] -= 1;

					if (vOLDLength[stepID] < 2) {
						Core::getMap()->setInEvent(false);
						inEvent = false;
						Core::getMap()->getPlayer()->stopMove();

						CFG::getMenuManager()->getLoadingMenu()->loadingType = false;
						CFG::getMenuManager()->getLoadingMenu()->updateTime();
						CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGameLoading);

						CFG::getMusic()->PlayChunk(CFG::getMusic()->cGAMEOVER);
					}
					break;
				case eBOSSEND1:
					for (int i = Core::getMap()->getMapWidth() - 1; i > 0; i--) {
						if (Core::getMap()->getMapBlock(i, 6)->getBlockID() == 82) {
							Core::getMap()->getMapBlock(i, 6)->setBlockID(0);
							break;
						}
					}
					//Core::getMap()->getMapBlock(Core::getMap()->getBlockIDX((int)(Core::getMap()->getPlayer()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 - Core::getMap()->getXPos()) + vOLDLength[stepID] - 1), 6)->setBlockID(0);
					Core::getMap()->clearPlatforms();
					CFG::getMusic()->PlayChunk(CFG::getMusic()->cBRIDGEBREAK);
					vOLDLength[stepID] = 0;
					Core::getMap()->getPlayer()->setMoveDirection(false);
					break;
				case eBOSSEND2:
					//Core::getMap()->getMapBlock(Core::getMap()->getBlockIDX((int)(Core::getMap()->getPlayer()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 - Core::getMap()->getXPos())) - 1, 5)->setBlockID(0);
					//Core::getMap()->getMapBlock(Core::getMap()->getBlockIDX((int)(Core::getMap()->getPlayer()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 - Core::getMap()->getXPos())) - 1, 4)->setBlockID(0);
					for (int i = Core::getMap()->getMapWidth() - 1; i > 0; i--) {
						if (Core::getMap()->getMapBlock(i, 5)->getBlockID() == 79) {
							Core::getMap()->getMapBlock(i, 5)->setBlockID(0);
							break;
						}
					}
					for (int i = Core::getMap()->getMapWidth() - 1; i > 0; i--) {
						if (Core::getMap()->getMapBlock(i, 4)->getBlockID() == 76) {
							Core::getMap()->getMapBlock(i, 4)->setBlockID(0);
							break;
						}
					}
					CFG::getMusic()->PlayChunk(CFG::getMusic()->cBRIDGEBREAK);
					vOLDLength[stepID] = 0;
					break;
				case eBOSSEND3:
					for (int i = Core::getMap()->getMapWidth() - 1; i > 0; i--) {
						if (Core::getMap()->getMapBlock(i, 4)->getBlockID() == 76) {
							Core::getMap()->getMapBlock(i, 4)->setBlockID(0);
							break;
						}
					}
					//Core::getMap()->getMapBlock(Core::getMap()->getBlockIDX((int)(Core::getMap()->getPlayer()->getXPos() + Core::getMap()->getPlayer()->getHitBoxX()/2 - Core::getMap()->getXPos())) - vOLDLength[stepID], 4)->setBlockID(0);
					CFG::getMusic()->PlayChunk(CFG::getMusic()->cBRIDGEBREAK);
					Core::getMap()->getPlayer()->setMoveDirection(true);
					vOLDLength[stepID] = 0;
					break;
				case eBOSSEND4:
					CFG::getMusic()->PlayChunk(CFG::getMusic()->cBOWSERFALL);
					vOLDLength[stepID] = 0;
					break;
				case eBOTRIGHTBOSS: // BOT & RIGHT
					Core::getMap()->getPlayer()->moveAnimation();
					Core::getMap()->getPlayer()->playerPhysics();
					Core::getMap()->setXPos((float)Core::getMap()->getXPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					break;
				case eBOSSTEXT1:
					Core::getMap()->addText(vOLDLength[stepID], CFG::GameHeight - 16 - 9 * 32, "THANK YOU MARIOz");
					vOLDLength[stepID] = 0;
					break;
				case eBOSSTEXT2:
					Core::getMap()->addText(vOLDLength[stepID] + 16, CFG::GameHeight - 16 - 7 * 32, "BUT OUR PRINCESS IS IN");
					Core::getMap()->addText(vOLDLength[stepID] + 16, CFG::GameHeight - 16 - 6 * 32, "ANOTHER CASTLEz");
					vOLDLength[stepID] = 0;
					break;
				case eENDGAMEBOSSTEXT1:
					Core::getMap()->addText(vOLDLength[stepID], CFG::GameHeight - 16 - 9 * 32, "THANK YOU MARIOz");
					vOLDLength[stepID] = 0;
					break;
				case eENDGAMEBOSSTEXT2:
					Core::getMap()->addText(vOLDLength[stepID] + 16, CFG::GameHeight - 16 - 7 * 32, "YOUR QUEST IS OVER.");
					vOLDLength[stepID] = 0;
					break;
				case eMARIOSPRITE1:
					Core::getMap()->getPlayer()->setSpriteID(1);
					vOLDLength[stepID] = 0;
					break;
				case eVINE1:
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setSpriteID(10);
					break;
				case eVINE2:
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setSpriteID(11);
					break;
				case eVINESPAWN:
					//Core::getMap()->addVine(vOLDLength[stepID], 0, 1, newLevelType == 0 || newLevelType == 4 ? 34 : 36); YET
					vOLDLength[stepID] = 0;
					break;
				}
			}
			else {
				++stepID;
				iTime = Core::coreClock.getElapsedTime().asMilliseconds();
			}
		} 
		else {
			if (!endGame) {
				if (Core::coreClock.getElapsedTime().asMilliseconds() >= iTime + iDelay) {
					bState = false;
					stepID = 0;
					newLevel();
					Core::getMap()->getPlayer()->stopMove();
					if (inEvent) {
						CFG::getMenuManager()->getLoadingMenu()->updateTime();
						CFG::getMenuManager()->getLoadingMenu()->loadingType = true;
						CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGameLoading);
						Core::getMap()->startLevelAnimation();
					}

					CFG::keySpace = false;
				}
			}
			else {
				Core::getMap()->resetGameData();
				CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eMainMenu);
				Core::getMap()->setInEvent(false);
				Core::getMap()->getPlayer()->stopMove();
				inEvent = false;
				CFG::keySpace = false;
				endGame = false;
				stepID = 0;
			}
		}
	}
	else {
		if (vNEWDir.size() > stepID) {
			if (vNEWLength[stepID] > 0) {
				switch (vNEWDir[stepID]) {
				case eTOP: // TOP
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - iSpeed);
					vNEWLength[stepID] -= iSpeed;
					break;
				case eBOT:
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() + iSpeed);
					vNEWLength[stepID] -= iSpeed;
					break;
				case eRIGHT:
					Core::getMap()->getPlayer()->setXPos((float)Core::getMap()->getPlayer()->getXPos() + iSpeed);
					vNEWLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->moveAnimation();
					break;
				case eLEFT:
					Core::getMap()->getPlayer()->setXPos((float)Core::getMap()->getPlayer()->getXPos() - iSpeed);
					vNEWLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->moveAnimation();
					break;
				case ePLAYPIPETOP:
					vNEWLength[stepID] -= 1;
					CFG::getMusic()->PlayChunk(CFG::getMusic()->cPIPE);
					break;
				case eNOTHING: // NOTHING YAY
					vNEWLength[stepID] -= 1;
					break;
				case eVINE1:
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - iSpeed);
					vNEWLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setSpriteID(10);
					break;
				case eVINE2:
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - iSpeed);
					vNEWLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setSpriteID(11);
					break;
				}
			}
			else {
				++stepID;
			}
		}
		else {
			Core::getMap()->setInEvent(false);
			Core::getMap()->getPlayer()->stopMove();
			CFG::getMusic()->changeMusic(true, true);
			inEvent = false;
			CFG::keySpace = false;
			Core::resetKeys();
		}
	}
}

void Event::end() {
    if (Core::getMap()->getFlag() != NULL && Core::getMap()->getFlag()->iYPos < CFG::GameHeight - 16 - 3 * 32 - 4) {
        Core::getMap()->getFlag()->Update();
    }
}

void Event::draw(sf::RenderWindow& window)
{
    for (unsigned int i = 0; i < reDrawX.size(); i++) {
        if (reDrawX[i] < Core::getMap()->getMapWidth())
            Core::getMap()->getBlock(Core::getMap()->getMapBlock(reDrawX[i], reDrawY[i])->getBlockID())->draw(window, 32 * reDrawX[i] + (int)Core::getMap()->getXPos(), CFG::GameHeight - 32 * reDrawY[i] - 16);
    }
}

void Event::Animation()
{
    switch (eventTypeID) {
        case eNormal: {
            Normal();
            break;
        }
        case eEnd: {
            Normal();
            end();
            break;
        }
        case eBossEnd: {
            Normal();
            break;
        }
        default:
            Normal();
            break;
    }
}

void Event::newLevel() {
    Core::getMap()->setXPos((float)newMapXPos);
    Core::getMap()->getPlayer()->setXPos((float)newPlayerXPos);
    Core::getMap()->getPlayer()->setYPos((float)newPlayerYPos);
    Core::getMap()->setMoveMap(newMoveMap);
    if (Core::getMap()->getCurrentLevelID() != newCurrentLevel) {
        CFG::getMenuManager()->getLoadingMenu()->updateTime();
        CFG::getMenuManager()->getLoadingMenu()->loadingType = true;
        CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGameLoading);
        Core::getMap()->getPlayer()->setCoins(0);
    }
    Core::getMap()->setCurrentLevelID(newCurrentLevel);
    Core::getMap()->setLevelType(newLevelType);
    if (newUnderWater) {
        Core::getMap()->getPlayer()->resetRun();
    }
    Core::getMap()->setUnderWater(newUnderWater);

    Core::getMap()->lockMinions();
}

void Event::resetData() {
    vNEWDir.clear();
    vNEWLength.clear();
    vOLDDir.clear();
    vOLDLength.clear();
    resetRedraw();

    this->eventTypeID = eNormal;

    this->bState = true;
    this->stepID = 0;
    this->inEvent = false;
    this->endGame = false;
    this->newUnderWater = false;
}

void Event::resetRedraw()
{
    reDrawX.clear();
    reDrawY.clear();
}
