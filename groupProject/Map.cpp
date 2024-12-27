#include "Map.h"

Map::Map()
{
	this->currentLevelID = 0;

	this->iMapWidth = 0;
	this->iMapHeight = 0;
	this->iLevelType = 0;

	this->drawLines = false;
	this->fXPos = 0;
	this->fYPos = 0;

	this->inEvent = false;

	this->iSpawnPointID = 0;

	this->bMoveMap = true;

	this->iFrameID = 0;

	this->bTP = false;

	pEvent = new Event();
	pFlag = NULL;

	srand((unsigned)time(NULL));
}

Map::~Map()
{
	for (std::vector<Block*>::iterator i = vBlock.begin(); i != vBlock.end(); i++) {
		delete (*i);
	}

	for (std::vector<Block*>::iterator i = vMinion.begin(); i != vMinion.end(); i++) {
		delete (*i);
	}

	delete pEvent;
	delete pFlag;
}

Map& Map::Instance()
{
	static Map singleTon;
	return singleTon;
}

void Map::update()
{
	updateGifBlocks();
	
	for (Player* pPlayer : vCurPlayer) {
		if (!pPlayer->getInLevelAnimation()) {
			updateMinionBlocks();
			updateMinions();
			break;
		}
	}

	if (!pPlayer->getInLevelAnimation()) {
		if (!inEvent) {
			updatePlayer();

			++iFrameID;
			if (iFrameID > 32) {
				iFrameID = 0;
				if (iMapTime > 0) {
					--iMapTime;
					if (iMapTime == 90) {
						CFG::getMusic()->StopMusic();
						CFG::getMusic()->PlayChunk(CFG::getMusic()->cLOWTIME);
					}
					else if (iMapTime == 86) {
						CFG::getMusic()->changeMusic(true, true);
					}

					if (iMapTime <= 0) {
						playerDeath(true, true);
					}
				}
			}
		}
		else {
			pEvent->Animation();
		}

		for (unsigned int i = 0; i < vPlatform.size(); i++) {
			vPlatform[i]->update();
		}
	}
	else {
		pPlayer->powerUPAnimation();
	}


	// Update Block Debris
	for (unsigned int i = 0; i < lBlockDebris.size(); i++) {
		if (lBlockDebris[i]->getDebrisState() != -1) {
			lBlockDebris[i]->update();
		}
		else {
			delete lBlockDebris[i];
			lBlockDebris.erase(lBlockDebris.begin() + i);
		}
	}
	// update coin + point
	for (unsigned int i = 0; i < lCoin.size(); i++) {
		if (!lCoin[i]->getDelete()) {
			lCoin[i]->Update();
		}
		else {
			lPoints.push_back(new Points(lCoin[i]->getXPos(), lCoin[i]->getYPos(), "200"));
			delete lCoin[i];
			lCoin.erase(lCoin.begin() + i);
		}
	}

	for (unsigned int i = 0; i < lPoints.size(); i++) {
		if (!lPoints[i]->getDelete()) {
			lPoints[i]->Update();
		}
		else {
			delete lPoints[i];
			lPoints.erase(lPoints.begin() + i);
		}
	}
}

void Map::playerDeath(bool animation, bool instantDeath) {

	// 
	if ((pPlayer->getPowerLVL() == 0 && !pPlayer->getUnkillAble()) || instantDeath) {
		inEvent = true;

		pEvent->resetData();
		pPlayer->resetJump();
		pPlayer->stopMove();

		pEvent->iDelay = 150;
		pEvent->newCurrentLevel = currentLevelID;

		pEvent->newMoveMap = bMoveMap;

		pEvent->eventTypeID = pEvent->eNormal;

		pPlayer->resetPowerLVL();

		if (animation) {
			pEvent->iSpeed = 4;
			pEvent->newLevelType = iLevelType;

			pPlayer->setYPos(pPlayer->getYPos() + 4.0f);

			pEvent->vOLDDir.push_back(pEvent->eDEATHNOTHING);
			pEvent->vOLDLength.push_back(30);

			pEvent->vOLDDir.push_back(pEvent->eDEATHTOP);
			pEvent->vOLDLength.push_back(64);

			pEvent->vOLDDir.push_back(pEvent->eDEATHBOT);
			pEvent->vOLDLength.push_back(CFG::GameHeight - pPlayer->getYPos() + 128);
		}
		else {
			pEvent->iSpeed = 4;
			pEvent->newLevelType = iLevelType;

			pEvent->vOLDDir.push_back(pEvent->eDEATHTOP);
			pEvent->vOLDLength.push_back(1);
		}

		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(64);

		if (pPlayer->getNumOfLives() > 1) {
			pEvent->vOLDDir.push_back(pEvent->eLOADINGMENU);
			pEvent->vOLDLength.push_back(90);

			pPlayer->setNumOfLives(pPlayer->getNumOfLives() - 1);

			CFG::getMusic()->StopMusic();
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cDEATH);
		}
		else {
			pEvent->vOLDDir.push_back(pEvent->eGAMEOVER);
			pEvent->vOLDLength.push_back(90);

			pPlayer->setNumOfLives(pPlayer->getNumOfLives() - 1);

			CFG::getMusic()->StopMusic();
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cDEATH);
		}
	}
	else if (!pPlayer->getUnkillAble()) {
		pPlayer->setPowerLVL(pPlayer->getPowerLVL() - 1);
	}
}

void Map::updateGifBlocks()
{
	vBlock[2]->getAniSprite()->update(); // gnd_red_1
	vBlock[8]->getAniSprite()->update(); // blockq_0, 2, 1, 2
	vBlock[29]->getAniSprite()->update(); // coin_use0, 2, 1, 2
	vBlock[55]->getAniSprite()->update(); // blockq1_0, 2, 1, 2
	vBlock[57]->getAniSprite()->update(); // coin1_0, 2, 1, 2
	vBlock[70]->getAniSprite()->update(); // t_bot
	vBlock[71]->getAniSprite()->update(); // coin_use00, 02, 01, 02
	vBlock[72]->getAniSprite()->update(); // coin_use20, 22, 21, 22
	vBlock[73]->getAniSprite()->update(); // coin_use30, 32, 31, 32
	vBlock[82]->getAniSprite()->update(); // axe_0, 1, 2
}

void Map::updatePlayer()
{
	for (Player* pPlayer : vCurPlayer) 
		pPlayer->update();
}

void Map::updateMinionBlocks() {
	vMinion[0]->getAniSprite()->update();
	vMinion[4]->getAniSprite()->update();
	vMinion[6]->getAniSprite()->update();
	vMinion[7]->getAniSprite()->update();
	vMinion[8]->getAniSprite()->update();
	vMinion[10]->getAniSprite()->update();
	vMinion[12]->getAniSprite()->update();
	vMinion[14]->getAniSprite()->update();
	vMinion[15]->getAniSprite()->update();
	vMinion[17]->getAniSprite()->update();
	vMinion[18]->getAniSprite()->update();
	vMinion[19]->getAniSprite()->update();
	vMinion[20]->getAniSprite()->update();
	vMinion[21]->getAniSprite()->update();
	vMinion[22]->getAniSprite()->update();
	vMinion[23]->getAniSprite()->update();
	vMinion[24]->getAniSprite()->update();
	vMinion[30]->getAniSprite()->update();
	vMinion[31]->getAniSprite()->update();
	vMinion[43]->getAniSprite()->update();
	vMinion[44]->getAniSprite()->update();
	vMinion[45]->getAniSprite()->update();
	vMinion[46]->getAniSprite()->update();
	vMinion[47]->getAniSprite()->update();
	vMinion[48]->getAniSprite()->update();
	vMinion[51]->getAniSprite()->update();
	vMinion[52]->getAniSprite()->update();
	vMinion[53]->getAniSprite()->update();
	vMinion[55]->getAniSprite()->update();
	vMinion[57]->getAniSprite()->update();
	vMinion[62]->getAniSprite()->update();
}

void Map::updateMinions() {
	for (int i = 0; i < iMinionListSize; i++) {
		for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++) {
			if (lMinion[i][j]->updateMinion()) {
				lMinion[i][j]->Update();
			}
		}
	}

	// ----- UPDATE MINION LIST ID
	for (int i = 0; i < iMinionListSize; i++) {
		for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++) {
			if (lMinion[i][j]->minionSpawned) {
				if (lMinion[i][j]->minionState == -1) {
					if (dynamic_cast<PlayerFireBall*>(lMinion[i][j]) != lMinion[i][j]) 
						delete lMinion[i][j];
					lMinion[i].erase(lMinion[i].begin() + j);
					jSize = lMinion[i].size();
					continue;
				}

				if (floor(lMinion[i][j]->fXPos / 160) != i) {
					lMinion[(int)floor((int)lMinion[i][j]->fXPos / 160)].push_back(lMinion[i][j]);
					lMinion[i].erase(lMinion[i].begin() + j);
					jSize = lMinion[i].size();
				}
			}
		}
	}

	for (unsigned int i = 0; i < lBubble.size(); i++) {
		lBubble[i]->Update();

		if (lBubble[i]->getDestroy()) {
			delete lBubble[i];
			lBubble.erase(lBubble.begin() + i);
		}
	}
}

void Map::updateMinionsCollisions() {
	// ----- COLLISIONS
	for (int i = 0; i < iMinionListSize; i++) {
		for (unsigned int j = 0; j < lMinion[i].size(); j++) {
			if (!lMinion[i][j]->collisionOnlyWithPlayer /*&& lMinion[i][j]->minionSpawned*/ && lMinion[i][j]->deadTime < 0) {
				// ----- WITH MINIONS IN SAME LIST
				for (unsigned int k = j + 1; k < lMinion[i].size(); k++) {
					if (!lMinion[i][k]->collisionOnlyWithPlayer /*&& lMinion[i][k]->minionSpawned*/ && lMinion[i][k]->deadTime < 0) {
						if (lMinion[i][j]->getXPos() < lMinion[i][k]->getXPos()) {
							if (lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX >= lMinion[i][k]->getXPos() && lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX <= lMinion[i][k]->getXPos() + lMinion[i][k]->iHitBoxX && ((lMinion[i][j]->getYPos() <= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY) || (lMinion[i][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY))) {
								if (lMinion[i][j]->killOtherUnits && lMinion[i][j]->moveSpeed > 0 && lMinion[i][k]->minionSpawned) {
									lMinion[i][k]->setMinionState(-2);
									lMinion[i][j]->collisionWithAnotherUnit();
								}

								if (lMinion[i][k]->killOtherUnits && lMinion[i][k]->moveSpeed > 0 && lMinion[i][j]->minionSpawned) {
									lMinion[i][j]->setMinionState(-2);
									lMinion[i][k]->collisionWithAnotherUnit();
								}

								if (lMinion[i][j]->getYPos() - 4 <= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY && lMinion[i][j]->getYPos() + 4 >= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY) {
									lMinion[i][k]->onAnotherMinion = true;
								}
								else if (lMinion[i][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY) {
									lMinion[i][j]->onAnotherMinion = true;
								}
								else {
									lMinion[i][j]->collisionEffect();
									lMinion[i][k]->collisionEffect();
								}
							}
						}
						else {
							if (lMinion[i][k]->getXPos() + lMinion[i][k]->iHitBoxX >= lMinion[i][j]->getXPos() && lMinion[i][k]->getXPos() + lMinion[i][k]->iHitBoxX <= lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX && ((lMinion[i][j]->getYPos() <= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY) || (lMinion[i][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY))) {
								if (lMinion[i][j]->killOtherUnits && lMinion[i][j]->moveSpeed > 0 && lMinion[i][k]->minionSpawned) {
									lMinion[i][k]->setMinionState(-2);
									lMinion[i][j]->collisionWithAnotherUnit();
								}

								if (lMinion[i][k]->killOtherUnits && lMinion[i][k]->moveSpeed > 0 && lMinion[i][j]->minionSpawned) {
									lMinion[i][j]->setMinionState(-2);
									lMinion[i][k]->collisionWithAnotherUnit();
								}

								if (lMinion[i][j]->getYPos() - 4 <= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY && lMinion[i][j]->getYPos() + 4 >= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY) {
									lMinion[i][k]->onAnotherMinion = true;
								}
								else if (lMinion[i][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY) {
									lMinion[i][j]->onAnotherMinion = true;
								}
								else {
									lMinion[i][j]->collisionEffect();
									lMinion[i][k]->collisionEffect();
								}
							}
						}
					}
				}

				// ----- WITH MINIONS IN OTHER LIST
				if (i + 1 < iMinionListSize) {
					for (unsigned int k = 0; k < lMinion[i + 1].size(); k++) {
						if (!lMinion[i + 1][k]->collisionOnlyWithPlayer /*&& lMinion[i + 1][k]->minionSpawned*/ && lMinion[i + 1][k]->deadTime < 0) {
							if (lMinion[i][j]->getXPos() < lMinion[i + 1][k]->getXPos()) {
								if (lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX >= lMinion[i + 1][k]->getXPos() && lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX <= lMinion[i + 1][k]->getXPos() + lMinion[i + 1][k]->iHitBoxX && ((lMinion[i][j]->getYPos() <= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY) || (lMinion[i + 1][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY))) {
									if (lMinion[i][j]->killOtherUnits && lMinion[i][j]->moveSpeed > 0 && lMinion[i + 1][k]->minionSpawned) {
										lMinion[i + 1][k]->setMinionState(-2);
										lMinion[i][j]->collisionWithAnotherUnit();
									}

									if (lMinion[i + 1][k]->killOtherUnits && lMinion[i + 1][k]->moveSpeed > 0 && lMinion[i][j]->minionSpawned) {
										lMinion[i][j]->setMinionState(-2);
										lMinion[i + 1][k]->collisionWithAnotherUnit();
									}

									if (lMinion[i][j]->getYPos() - 4 <= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY && lMinion[i][j]->getYPos() + 4 >= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY) {
										lMinion[i + 1][k]->onAnotherMinion = true;
									}
									else if (lMinion[i + 1][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i + 1][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY) {
										lMinion[i][j]->onAnotherMinion = true;
									}
									else {
										lMinion[i][j]->collisionEffect();
										lMinion[i + 1][k]->collisionEffect();
									}
								}
							}
							else {
								if (lMinion[i + 1][k]->getXPos() + lMinion[i + 1][k]->iHitBoxX >= lMinion[i][j]->getXPos() && lMinion[i + 1][k]->getXPos() + lMinion[i + 1][k]->iHitBoxX < lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX && ((lMinion[i][j]->getYPos() <= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY) || (lMinion[i + 1][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY))) {
									if (lMinion[i][j]->killOtherUnits && lMinion[i][j]->moveSpeed > 0 && lMinion[i + 1][k]->minionSpawned) {
										lMinion[i + 1][k]->setMinionState(-2);
										lMinion[i][j]->collisionWithAnotherUnit();
									}

									if (lMinion[i + 1][k]->killOtherUnits && lMinion[i + 1][k]->moveSpeed > 0 && lMinion[i][j]->minionSpawned) {
										lMinion[i][j]->setMinionState(-2);
										lMinion[i + 1][k]->collisionWithAnotherUnit();
									}

									if (lMinion[i][j]->getYPos() - 4 <= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY && lMinion[i][j]->getYPos() + 4 >= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->iHitBoxY) {
										lMinion[i + 1][k]->onAnotherMinion = true;
									}
									else if (lMinion[i + 1][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i + 1][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY) {
										lMinion[i][j]->onAnotherMinion = true;
									}
									else {
										lMinion[i][j]->collisionEffect();
										lMinion[i + 1][k]->collisionEffect();
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (inEvent) return;

	// ----- COLLISION WITH PLAYER
	for (int i = 0; i < iNumOfPlayers; i++) {
		updateMinionPlayerCollisions(i);
	}

	//if (!pPlayer->getInLevelAnimation()) {
	//	for (int i = getListID(-(int)fXPos + pPlayer->getXPos()) - (getListID(-(int)fXPos + pPlayer->getXPos()) > 0 ? 1 : 0), iSize = i + 2; i < iSize; i++) {
	//		for (unsigned int j = 0, jSize = lMinion[i].size(); j < jSize; j++) {
	//			//std::cout << lMinion[i][j]->deadTime << std::endl;

	//			if (lMinion[i][j]->isAlive()) updateSkillCollisions(lMinion[i][j]);

	//			if (lMinion[i][j]->isAlive()) {
	//				int playerLeftX = pPlayer->getXPos() - fXPos;
	//				// if (minion.leftX <= player.leftX <= minion.rightX) or (minion.leftX <= player.rightX <= minion.rightX)
	//				if (lMinion[i][j]->checkVerticalOverlap(playerLeftX, playerLeftX + pPlayer->getHitBoxX())) {

	//					// if (minion.y - 2 <= player.botY <= minion.y + 16) when player is on top of minion 2 px
	//					if (lMinion[i][j]->checkHorizontalTopOverlap(pPlayer->getYPos() + pPlayer->getHitBoxY())) {
	//						lMinion[i][j]->collisionWithPlayer(true);
	//					}
	//					// if (minion.y <= player.botY <= minion.botY) or (minion.y <= player.y <= minion.botY)
	//					else if (lMinion[i][j]->checkHorizontalOverlap(pPlayer->getYPos(), pPlayer->getYPos() + pPlayer->getHitBoxY())) {
	//						lMinion[i][j]->collisionWithPlayer(false);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void Map::updateMinionPlayerCollisions(int iIDPlayer)
{
	if (vCurPlayer[iIDPlayer]->getInLevelAnimation()) return;
	
	float fXPosTMP = (iIDPlayer == 0) ? fXPos : fXPos2;
	Player* p_Player = vCurPlayer[iIDPlayer];

	for (int i = getListID(-(int)fXPosTMP + p_Player->getXPos()) - (getListID(-(int)fXPosTMP + p_Player->getXPos()) > 0 ? 1 : 0), iSize = i + 2; i < iSize; i++) {
		for (unsigned int j = 0, jSize = lMinion[i].size(); j < jSize; j++) {
			//std::cout << lMinion[i][j]->deadTime << std::endl;

			if (lMinion[i][j]->isAlive()) updateSkillCollisions(lMinion[i][j]);

			if (lMinion[i][j]->isAlive()) {
				int playerLeftX = p_Player->getXPos() - fXPosTMP;
				// if (minion.leftX <= player.leftX <= minion.rightX) or (minion.leftX <= player.rightX <= minion.rightX)
				if (lMinion[i][j]->checkVerticalOverlap(playerLeftX, playerLeftX + p_Player->getHitBoxX())) {

					// if (minion.y - 2 <= player.botY <= minion.y + 16) when player is on top of minion 2 px
					if (lMinion[i][j]->checkHorizontalTopOverlap(p_Player->getYPos() + p_Player->getHitBoxY())) {
						lMinion[i][j]->collisionWithPlayer(true, p_Player);
					}
					// if (minion.y <= player.botY <= minion.botY) or (minion.y <= player.y <= minion.botY)
					else if (lMinion[i][j]->checkHorizontalOverlap(p_Player->getYPos(), p_Player->getYPos() + p_Player->getHitBoxY())) {
						lMinion[i][j]->collisionWithPlayer(false, p_Player);
					}
				}
			}
		}
	}
}

void Map::updateSkillCollisions(Minion* pMinion)
{
	if (!pPlayer->getUseSkill()) return;
	pPlayer->useSkill(pMinion, fXPos);
}

void Map::draw(sf::RenderWindow& mainWindow)
{
	drawMap(mainWindow);

	for (unsigned int i = 0; i < vPlatform.size(); i++) {
		vPlatform[i]->draw(mainWindow);
	}

	DrawMinions(mainWindow);

	for (unsigned int i = 0; i < lPoints.size(); i++) {
		lPoints[i]->Draw(mainWindow);
	}

	for (unsigned int i = 0; i < lCoin.size(); i++) {
		lCoin[i]->Draw(mainWindow);
	}

	for (unsigned int i = 0; i < lBlockDebris.size(); i++) {
		lBlockDebris[i]->draw(mainWindow);
	}

	for (unsigned int i = 0; i < vLevelText.size(); i++) {
		CFG::getText()->Draw(mainWindow, vLevelText[i]->getText(), vLevelText[i]->getXPos() + (int)fXPos, vLevelText[i]->getYPos());
	}

	for (unsigned int i = 0; i < lBubble.size(); i++) {
		lBubble[i]->Draw(mainWindow, vBlock[lBubble[i]->getBlockID()]->getAniSprite()->getTexture());
	}

	for (Player* pPlayer : vCurPlayer)
		pPlayer->draw(mainWindow);

	if (inEvent) {
		pEvent->draw(mainWindow);
	}

	DrawGameLayout(mainWindow);
}

void Map::drawMap(sf::RenderWindow& mainWindow)
{
	if (pFlag != NULL) {
		pFlag->drawCastleFlag(mainWindow, vBlock[51]->getAniSprite()->getTexture());
	}

	for (int i = getStartBlock(), iEnd = getEndBlock(); i < iEnd && i < iMapWidth; i++) {
		for (int j = iMapHeight - 1; j >= 0; j--) {
			if (lMap[i][j]->getBlockID() != 0) {
				vBlock[lMap[i][j]->getBlockID()]->draw(mainWindow, 32 * i + (int)fXPos, CFG::GameHeight - 32 * j - 16 - lMap[i][j]->updateYPos());
			}
		}
	}

	if (pFlag != NULL) {
		pFlag->draw(mainWindow, vBlock[pFlag->iBlockID]->getAniSprite()->getTexture());
	}
}

void Map::DrawMinions(sf::RenderWindow& mainWindow) {
	for (int i = 0; i < iMinionListSize; i++) {
		for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++) {
			lMinion[i][j]->Draw(mainWindow, vMinion[lMinion[i][j]->getBloockID()]->getAniSprite()->getFrame());
			lMinion[i][j]->drawBoundingBox(mainWindow);
			//CFG::getText()->DrawWS(mainWindow, std::to_string(i), lMinion[i][j]->getXPos() + (int)fXPos, lMinion[i][j]->getYPos(), 0, 0, 0, 8);
		}
	}
}

void Map::DrawGameLayout(sf::RenderWindow& mainWindow) {
	CFG::getText()->Draw(mainWindow, "MARIO", 54, 16);

	if (pPlayer->getScore() < 100) {
		CFG::getText()->Draw(mainWindow, "00000" + std::to_string(pPlayer->getScore()), 54, 32);
	}
	else if (pPlayer->getScore() < 1000) {
		CFG::getText()->Draw(mainWindow, "000" + std::to_string(pPlayer->getScore()), 54, 32);
	}
	else if (pPlayer->getScore() < 10000) {
		CFG::getText()->Draw(mainWindow, "00" + std::to_string(pPlayer->getScore()), 54, 32);
	}
	else if (pPlayer->getScore() < 100000) {
		CFG::getText()->Draw(mainWindow, "0" + std::to_string(pPlayer->getScore()), 54, 32);
	}
	else {
		CFG::getText()->Draw(mainWindow, std::to_string(pPlayer->getScore()), 54, 32);
	}

	CFG::getText()->Draw(mainWindow, "WORLD", 462+150, 16);
	CFG::getText()->Draw(mainWindow, getLevelName(), 480+150, 32);

	if (iLevelType != 1) {
		vBlock[2]->draw(mainWindow, 268+90, 32);
	}
	else {
		vBlock[57]->draw(mainWindow, 268+90, 32); 
	}
	CFG::getText()->Draw(mainWindow, "y", 286+90, 32);
	CFG::getText()->Draw(mainWindow, (pPlayer->getCoins() < 10 ? "0" : "") + std::to_string(pPlayer->getCoins()), 302+90, 32);

	// Time
	CFG::getText()->Draw(mainWindow, "TIME", 672+200, 16);
	if (CFG::getMenuManager()->getViewID() == CFG::getMenuManager()->eGame) {
		if (iMapTime > 100) {
			CFG::getText()->Draw(mainWindow, std::to_string(iMapTime), 680+200, 32);
		}
		else if (iMapTime > 10) {
			CFG::getText()->Draw(mainWindow, "0" + std::to_string(iMapTime), 680+200, 32);
		}
		else {
			CFG::getText()->Draw(mainWindow, "00" + std::to_string(iMapTime), 680+200, 32);
		}
	}
}

void Map::moveSelectPlayer(int iD)
{
	int prevIndex = indexPlayer;
	indexPlayer += iD;
	if (indexPlayer < 0) {
		indexPlayer = vPlayer.size()-1;
	}
	else if (indexPlayer >= vPlayer.size()) {
		indexPlayer = 0;
	}
	pPlayer = vPlayer[indexPlayer];
	pPlayer->copyStats(vPlayer[prevIndex]);
}

// POS 0 = TOP, 1 = BOT
bool Map::blockUse(int nX, int nY, int iBlockID, int POS) {
	
	if (POS == 0) {
		switch (iBlockID) {
		case 8: case 55: // ----- BlockQ
			if (lMap[nX][nY]->getSpawnMushroom()) {
				if (lMap[nX][nY]->getPowerUP()) {
					if (pPlayer->getPowerLVL() == 0) {
						lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, true, nX, nY));
					}
					else {
						lMinion[getListID(32 * nX)].push_back(new Flower(32 * nX, CFG::GameHeight - 16 - 32 * nY, nX, nY));
					}
				}
				else {
					lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, false, nX, nY));
				}
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMAPPER);
			}
			else {
				lCoin.push_back(new Coin(nX * 32 + 7, CFG::GameHeight - nY * 32 - 48));
				pPlayer->setScore(pPlayer->getScore() + 200);
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);
				pPlayer->setCoins(pPlayer->getCoins() + 1);
			}

			if (lMap[nX][nY]->getNumOfUse() > 1) {
				lMap[nX][nY]->setNumOfUse(lMap[nX][nY]->getNumOfUse() - 1);
			}
			else {
				lMap[nX][nY]->setNumOfUse(0);
				lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 9 : iLevelType == 1 ? 56 : 56);
			}

			lMap[nX][nY]->startBlockAnimation();
			checkCollisionOnTopOfTheBlock(nX, nY);
			break;
		case 13: case 28: case 81: // ----- Brick
			if (lMap[nX][nY]->getSpawnStar()) {
				lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 9 : iLevelType == 1 ? 56 : 80);
				lMinion[getListID(32 * nX)].push_back(new Star(32 * nX, CFG::GameHeight - 16 - 32 * nY, nX, nY));
				lMap[nX][nY]->startBlockAnimation();
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMAPPER);
			}
			else if (lMap[nX][nY]->getSpawnMushroom()) {
				lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 9 : iLevelType == 1 ? 56 : 80);
				if (lMap[nX][nY]->getPowerUP()) {
					if (pPlayer->getPowerLVL() == 0) {
						lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, true, nX, nY));
					}
					else {
						lMinion[getListID(32 * nX)].push_back(new Flower(32 * nX, CFG::GameHeight - 16 - 32 * nY, nX, nY));
					}
				}
				else {
					lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, false, nX, nY));
				}
				lMap[nX][nY]->startBlockAnimation();
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMAPPER);
			}
			else if (lMap[nX][nY]->getNumOfUse() > 0) {
				lCoin.push_back(new Coin(nX * 32 + 7, CFG::GameHeight - nY * 32 - 48));
				pPlayer->setScore(pPlayer->getScore() + 200);
				pPlayer->setCoins(pPlayer->getCoins() + 1);

				lMap[nX][nY]->setNumOfUse(lMap[nX][nY]->getNumOfUse() - 1);
				if (lMap[nX][nY]->getNumOfUse() == 0) {
					lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 9 : iLevelType == 1 ? 56 : 80);
				}

				CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);

				lMap[nX][nY]->startBlockAnimation();
			}
			else {
				if (pPlayer->getPowerLVL() > 0) {
					lMap[nX][nY]->setBlockID(0);
					lBlockDebris.push_back(new BlockDebris(nX * 32, CFG::GameHeight - 48 - nY * 32));
					CFG::getMusic()->PlayChunk(CFG::getMusic()->cBLOCKBREAK);
				}
				else {
					lMap[nX][nY]->startBlockAnimation();
					CFG::getMusic()->PlayChunk(CFG::getMusic()->cBLOCKHIT);
				}
			}

			checkCollisionOnTopOfTheBlock(nX, nY);
			break;
		case 24: // ----- BlockQ2
			if (lMap[nX][nY]->getSpawnMushroom()) {
				if (lMap[nX][nY]->getPowerUP()) {
					if (pPlayer->getPowerLVL() == 0) {
						lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, true, nX, nY));
					}
					else {
						lMinion[getListID(32 * nX)].push_back(new Flower(32 * nX, CFG::GameHeight - 16 - 32 * nY, nX, nY));
					}
				}
				else {
					lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, false, nX, nY));
				}
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMAPPER);
			}
			else {
				lCoin.push_back(new Coin(nX * 32 + 7, CFG::GameHeight - nY * 32 - 48));
				pPlayer->setCoins(pPlayer->getCoins() + 1);
				pPlayer->setScore(pPlayer->getScore() + 200);
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);

				lMap[nX][nY]->startBlockAnimation();
			}

			lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 9 : iLevelType == 1 ? 56 : 80);
			checkCollisionOnTopOfTheBlock(nX, nY);
			break;
		case 128: case 129:
			spawnVine(nX, nY, iBlockID);
			lMap[nX][nY]->setBlockID(iBlockID == 128 ? 9 : 56);
			lMap[nX][nY]->startBlockAnimation();
			break;
		default:
			break;
		}
	}
	else if (POS == 1) {
		switch (iBlockID) {
		case 21: case 23: case 31: case 33: case 98: case 100: case 113: case 115: case 137: case 139: case 177: case 179: // Pipe
			pipeUse();
			break;
		case 40: case 41: case 123: case 124: case 182: // End
			EndUse();
			break;
		case 82:
			EndBoss();
			break;
		default:
			break;
		}
	}

	switch (iBlockID) {
		case 29: case 71: case 72: case 73:// COIN
			lMap[nX][nY]->setBlockID(iLevelType == 2 ? 94 : 0);
			pPlayer->addCoin();
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);
			return false;
			break;
		case 36: case 38: case 60: case 62: case 103: case 105: case 118: case 120: // Pipe
			pipeUse();
			break;
		case 127: // BONUS END
			EndBonus();
			break;
		case 169:
			//TPUse();
			break;
		case 170:
			//TPUse2();
			break;
		case 171:
			//TPUse3();
			break;
		default:
			break;
		}
	return true;
}

int Map::getSpawnPointXPos(int iID) {
	switch (currentLevelID) {
	case 0:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 82 * 32;
		}
	case 1:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 89 * 32;
		}
	case 2:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 66 * 32;
		}
	case 3:
		return 64;
	case 4:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 98 * 32;
		}
	case 5:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 86 * 32;
		}
	case 6:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 114 * 32;
		}
	case 7:
		return 64;
	case 8:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 90 * 32;
		}
	case 9:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 98 * 32;
		}
	case 10:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 66 * 32;
		}
	case 13:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 95 * 32;
		}
	case 14:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 65 * 32;
		}
	case 16:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 97 * 32;
		}
	case 17:
		switch (iID) {
		case 0:
			return 84 + 80 * 32;
		case 1:
			return 177 * 32;
		}
	case 18:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 66 * 32;
		}
	case 20:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 98 * 32;
		}
	case 21:
		switch (iID) {
		case 0:
			return 84 + 85 * 32;
		case 1:
			return 183 * 32;
		}
	case 22:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 98 * 32;
		}
	case 24:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 98 * 32;
		}
	case 25:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 86 * 32;
		}
	case 26:
		switch (iID) {
		case 0:
			return 84;
		case 1:
			return 113 * 32;
		}
	}

	return 84;
}

int Map::getSpawnPointYPos(int iID) {
	std::cout << "currentLevelID: " << currentLevelID << std::endl;
	switch (currentLevelID) {
	case 1:
		switch (iID) {
		case 0:
			return 64;
		}
	case 5: case 25:
		switch (iID) {
		case 0:
			return 64;
		case 1:
			return CFG::GameHeight - 48 - pPlayer->getHitBoxY();;
		}
	case 3: case 7: case 11: case 15: case 19: case 23: case 27: case 31:
		return 150+50+50;
	}

	return CFG::GameHeight - 48 - pPlayer->getHitBoxY();
}

void Map::setSpawnPoint() {
	float X = (float)getSpawnPointXPos(iSpawnPointID);

	if (X > 6 * 32) {
		fXPos = -(X - 6 * 32);
		pPlayer->setXPos(6 * 32);
		pPlayer->setYPos((float)getSpawnPointYPos(iSpawnPointID));
	}
	else {
		fXPos = 0;
		pPlayer->setXPos(X);
		pPlayer->setYPos((float)getSpawnPointYPos(iSpawnPointID));
	}

	pPlayer->setMoveDirection(true);
}

void Map::resetGameData() {
	this->currentLevelID = 0;
	this->iSpawnPointID = 0;

	pPlayer->setCoins(0);
	pPlayer->setScore(0);
	pPlayer->resetPowerLVL();

	pPlayer->setNumOfLives(3);

	setSpawnPoint();

	loadLVL();
}

void Map::startLevelAnimation() {
	pEvent->newUnderWater = false;

	//std::cout << "currentLevelID" << currentLevelID << std::endl;

	switch (currentLevelID) {
	case 0:
		break;
	case 1:
		pEvent->resetData();
		pPlayer->resetJump();
		pPlayer->stopMove();

		pEvent->iSpeed = 2;
		pEvent->newLevelType = 1;

		pEvent->iDelay = 150;
		pEvent->newCurrentLevel = 1;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 96;
		pEvent->newPlayerYPos = 64;
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(7 * 32 + 4);

		pEvent->vOLDDir.push_back(pEvent->ePLAYPIPERIGHT);
		pEvent->vOLDLength.push_back(1);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(1 * 32 - 2);

		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(75);

		pEvent->reDrawX.push_back(220);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(220);
		pEvent->reDrawY.push_back(3);
		break;
	case 5:
		pEvent->resetData();
		pPlayer->resetJump();
		pPlayer->stopMove();

		pEvent->iSpeed = 2;
		pEvent->newLevelType = 2;

		pEvent->iDelay = 150;
		pEvent->newCurrentLevel = 5;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 96;
		pEvent->newPlayerYPos = 64;
		pEvent->newMoveMap = true;
		pEvent->newUnderWater = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(7 * 32 + 4);

		pEvent->vOLDDir.push_back(pEvent->ePLAYPIPERIGHT);
		pEvent->vOLDLength.push_back(1);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(1 * 32 - 2);

		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(75);

		pEvent->reDrawX.push_back(230);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(230);
		pEvent->reDrawY.push_back(3);
		break;
	case 13:
		pEvent->resetData();
		pPlayer->resetJump();
		pPlayer->stopMove();

		pEvent->iSpeed = 2;
		pEvent->newLevelType = 1;

		pEvent->iDelay = 150;
		pEvent->newCurrentLevel = currentLevelID;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 96;
		pEvent->newPlayerYPos = 64;
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(7 * 32 + 4);

		pEvent->vOLDDir.push_back(pEvent->ePLAYPIPERIGHT);
		pEvent->vOLDLength.push_back(1);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(1 * 32 - 2);

		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(75);

		pEvent->reDrawX.push_back(250);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(250);
		pEvent->reDrawY.push_back(3);
		break;
	case 25:
		pEvent->resetData();
		pPlayer->resetJump();
		pPlayer->stopMove();

		pEvent->iSpeed = 2;
		pEvent->newLevelType = 2;

		pEvent->iDelay = 150;
		pEvent->newCurrentLevel = 25;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 96;
		pEvent->newPlayerYPos = 64;
		pEvent->newMoveMap = true;
		pEvent->newUnderWater = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(7 * 32 + 4);

		pEvent->vOLDDir.push_back(pEvent->ePLAYPIPERIGHT);
		pEvent->vOLDLength.push_back(1);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(1 * 32 - 2);

		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(75);

		pEvent->reDrawX.push_back(230);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(230);
		pEvent->reDrawY.push_back(3);
		break;
	case 26:
		pEvent->resetData();
		pPlayer->resetJump();
		pPlayer->stopMove();

		pEvent->iSpeed = 2;
		pEvent->newLevelType = 2;

		pEvent->iDelay = 150;
		pEvent->newCurrentLevel = 26;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 96;
		pEvent->newPlayerYPos = 64;
		pEvent->newMoveMap = true;
		pEvent->newUnderWater = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(7 * 32 + 4);

		pEvent->vOLDDir.push_back(pEvent->ePLAYPIPERIGHT);
		pEvent->vOLDLength.push_back(1);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(1 * 32 - 2);

		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(75);

		pEvent->reDrawX.push_back(230);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(230);
		pEvent->reDrawY.push_back(3);
		break;
	}
}

void Map::loadLVL() {
	clearPipeEvents();

	switch (currentLevelID) {
	case 0:
		loadLVL_1_1();
		break;
	case 1:
		loadLVL_1_2();
		break;
	case 2:
		loadLVL_1_3();
		break;
	case 3:
		loadLVL_1_4();
		break;
	case 4:
		loadLVL_2_1();
		break;
	case 5:
		loadLVL_2_2();
		break;
	case 6:
		loadLVL_2_3();
		break;
	case 7:
		loadLVL_2_4();
		break;
	case 8:
		loadLVL_3_1();
		break;
	case 9:
		loadLVL_3_2();
		break;
	case 10:
		loadLVL_3_3();
		break;
	case 11:
		loadLVL_3_4();
		break;
	}
}

void Map::spawnVine(int nX, int nY, int iBlockID) {
	if (iLevelType == 0 || iLevelType == 4) {
		addVine(nX, nY, 0, 34);
	}
	else {
		addVine(nX, nY, 0, 36);
	}
}

void Map::moveMap(int nX, int nY, int iIDPlayer)
{
	if (iIDPlayer == 0) {
		if (fXPos + nX > 0) {
			vCurPlayer[0]->updateXPos((int)(nX - fXPos));
			fXPos = 0;
		}
		else {
			this->fXPos += nX;
		}
		return;
	}

	if (fXPos2 + nX > 0) {
		vCurPlayer[1]->updateXPos((int)(nX - fXPos2));
		fXPos2 = 0;
	}
	else {
		this->fXPos2 += nX;
	}
	return;

}

void Map::structBush(int X, int Y, int iSize)
{
	// ----- LEFT & RIGHT
	for (int i = 0; i < iSize; i++) {
		lMap[X + i][Y + i]->setBlockID(5);
		lMap[X + iSize + 1 + i][Y + iSize - 1 - i]->setBlockID(6);
	}

	// ----- CENTER LEFT & RIGHT
	for (int i = 0, k = 1; i < iSize - 1; i++) {
		for (int j = 0; j < k; j++) {
			lMap[X + 1 + i][Y + j]->setBlockID((i % 2 == 0 ? 3 : 4));
			lMap[X + iSize * 2 - 1 - i][Y + j]->setBlockID((i % 2 == 0 ? 3 : 4));
		}
		++k;
	}

	// ----- CENTER
	for (int i = 0; i < iSize; i++) {
		lMap[X + iSize][Y + i]->setBlockID((i % 2 == 0 && iSize != 1 ? 4 : 3));
	}

	// ----- TOP
	lMap[X + iSize][Y + iSize]->setBlockID(7);
}

void Map::structGrass(int X, int Y, int iSize)
{
	lMap[X][Y]->setBlockID(10);
	for (int i = 0; i < iSize; i++) {
		lMap[X + 1 + i][Y]->setBlockID(11);
	}
	lMap[X + iSize + 1][Y]->setBlockID(12);
}

void Map::structCloud(int X, int Y, int iSize)
{
	// ----- LEFT
	lMap[X][Y]->setBlockID(iLevelType == 3 ? 148 : 14);
	lMap[X][Y + 1]->setBlockID(15);

	for (int i = 0; i < iSize; i++) {
		lMap[X + 1 + i][Y]->setBlockID(iLevelType == 3 ? 149 : 16);
		lMap[X + 1 + i][Y + 1]->setBlockID(iLevelType == 3 ? 150 : 17);
	}

	lMap[X + iSize + 1][Y]->setBlockID(18);
	lMap[X + iSize + 1][Y + 1]->setBlockID(19);
}

void Map::structGND(int X, int Y, int iWidth, int iHeight)
{
	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 1 : iLevelType == 1 ? 26 : iLevelType == 2 ? 92 : iLevelType == 6 ? 166 : iLevelType == 7 ? 181 : 75);
		}
	}
}

void Map::structGND2(int X, int Y, int iSize, bool bDir)
{
	if (bDir) {
		for (int i = 0, k = 1; i < iSize; i++) {
			for (int j = 0; j < k; j++) {
				lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 25 : iLevelType == 3 ? 167 : 27);
			}
			++k;
		}
	}
	else {
		for (int i = 0, k = 1; i < iSize; i++) {
			for (int j = 0; j < k; j++) {
				lMap[X + iSize - 1 - i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 25 : iLevelType == 3 ? 167 : 27);
			}
			++k;
		}
	}
}

void Map::structGND2(int X, int Y, int iWidth, int iHeight)
{
	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 25 : iLevelType == 3 ? 167 : 27);
		}
	}
}

void Map::structUW1(int X, int Y, int iWidth, int iHeight) {
	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			lMap[X + i][Y + j]->setBlockID(93);
		}
	}
}

void Map::structBonus(int X, int Y, int iWidth) {
	for (int i = 0; i < iWidth; i++) {
		lMap[X + i][Y]->setBlockID(125);
	}
}

void Map::structBonusEnd(int X) {
	for (int i = 0; i < 20; i++) {
		lMap[X + i][0]->setBlockID(127);
	}
}

void Map::structPipe(int X, int Y, int iHeight) {
	for (int i = 0; i < iHeight; i++) {
		lMap[X][Y + i]->setBlockID(iLevelType == 0 ? 20 : iLevelType == 2 ? 97 : iLevelType == 4 ? 112 : iLevelType == 5 ? 136 : iLevelType == 3 ? 176 : iLevelType == 7 ? 172 : 30);
		lMap[X + 1][Y + i]->setBlockID(iLevelType == 0 ? 22 : iLevelType == 2 ? 99 : iLevelType == 4 ? 114 : iLevelType == 5 ? 138 : iLevelType == 3 ? 178 : iLevelType == 7 ? 174 : 32);
	}

	lMap[X][Y + iHeight]->setBlockID(iLevelType == 0 ? 21 : iLevelType == 2 ? 98 : iLevelType == 4 ? 113 : iLevelType == 5 ? 137 : iLevelType == 3 ? 177 : iLevelType == 7 ? 173 : 31);
	lMap[X + 1][Y + iHeight]->setBlockID(iLevelType == 0 ? 23 : iLevelType == 2 ? 100 : iLevelType == 4 ? 115 : iLevelType == 5 ? 139 : iLevelType == 3 ? 179 : iLevelType == 7 ? 175 : 33);
}

void Map::structPipeVertical(int X, int Y, int iHeight) {
	for (int i = 0; i < iHeight + 1; i++) {
		lMap[X][Y + i]->setBlockID(iLevelType == 0 ? 20 : iLevelType == 2 ? 97 : iLevelType == 4 ? 112 : 30);
		lMap[X + 1][Y + i]->setBlockID(iLevelType == 0 ? 22 : iLevelType == 2 ? 99 : iLevelType == 4 ? 114 : 32);
	}
}

void Map::structPipeHorizontal(int X, int Y, int iWidth) {
	lMap[X][Y]->setBlockID(iLevelType == 0 ? 62 : iLevelType == 2 ? 105 : iLevelType == 4 ? 120 : 38);
	lMap[X][Y + 1]->setBlockID(iLevelType == 0 ? 60 : iLevelType == 2 ? 103 : iLevelType == 4 ? 118 : 36);

	for (int i = 0; i < iWidth; i++) {
		lMap[X + 1 + i][Y]->setBlockID(iLevelType == 0 ? 61 : iLevelType == 2 ? 104 : iLevelType == 4 ? 119 : 37);
		lMap[X + 1 + i][Y + 1]->setBlockID(iLevelType == 0 ? 59 : iLevelType == 2 ? 102 : iLevelType == 4 ? 117 : 35);
	}

	lMap[X + 1 + iWidth][Y]->setBlockID(iLevelType == 0 ? 58 : iLevelType == 2 ? 101 : iLevelType == 4 ? 116 : 34);
	lMap[X + 1 + iWidth][Y + 1]->setBlockID(iLevelType == 0 ? 63 : iLevelType == 2 ? 106 : iLevelType == 4 ? 121 : 39);
}

void Map::structCoins(int X, int Y, int iWidth, int iHeight) {
	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 71 : iLevelType == 1 ? 29 : iLevelType == 2 ? 73 : 29);
		}
	}
}

void Map::structBrick(int X, int Y, int iWidth, int iHeight)
{
	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 13 : iLevelType == 3 ? 81 : 28);
		}
	}
}

void Map::struckBlockQ(int X, int Y, int iWidth)
{
	for (int i = 0; i < iWidth; i++) {
		lMap[X + i][Y]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 8 : 55);
	}
}

void Map::struckBlockQ2(int X, int Y, int iWidth)
{
	for (int i = 0; i < iWidth; i++) {
		lMap[X + i][Y]->setBlockID(24);
	}
}

void Map::structEnd(int X, int Y, int iHeight) {
	for (int i = 0; i < iHeight; i++) {
		lMap[X][Y + i]->setBlockID(iLevelType == 4 ? 123 : 40);
	}

	pFlag = new Flag(X * 32 - 16, Y + iHeight + 72*2 + 35);

	lMap[X][Y + iHeight]->setBlockID(iLevelType == 4 ? 124 : 41);

	for (int i = Y + iHeight + 1; i < Y + iHeight + 4; i++) {
		lMap[X][i]->setBlockID(182);
	}
}

void Map::structCastleSmall(int X, int Y) {
	for (int i = 0; i < 2; i++) {
		lMap[X][Y + i]->setBlockID(iLevelType == 3 ? 155 : 43);
		lMap[X + 1][Y + i]->setBlockID(iLevelType == 3 ? 155 : 43);
		lMap[X + 3][Y + i]->setBlockID(iLevelType == 3 ? 155 : 43);
		lMap[X + 4][Y + i]->setBlockID(iLevelType == 3 ? 155 : 43);

		lMap[X + 2][Y + i]->setBlockID(iLevelType == 3 ? 159 : 47);
	}

	lMap[X + 2][Y + 1]->setBlockID(iLevelType == 3 ? 158 : 46);

	for (int i = 0; i < 5; i++) {
		lMap[X + i][Y + 2]->setBlockID(i == 0 || i == 4 ? iLevelType == 3 ? 157 : 45 : iLevelType == 3 ? 156 : 44);
	}

	lMap[X + 1][Y + 3]->setBlockID(iLevelType == 3 ? 160 : 48);
	lMap[X + 2][Y + 3]->setBlockID(iLevelType == 3 ? 155 : 43);
	lMap[X + 3][Y + 3]->setBlockID(iLevelType == 3 ? 161 : 49);

	for (int i = 0; i < 3; i++) {
		lMap[X + i + 1][Y + 4]->setBlockID(iLevelType == 3 ? 157 : 45);
	}
}

void Map::structCastleBig(int X, int Y) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			setBlockID(X + i, Y + j, iLevelType == 3 ? 155 : 43);
			setBlockID(X + i + 7, Y + j, iLevelType == 3 ? 155 : 43);
		}
	}

	for (int i = 0; i < 3; i++) {
		setBlockID(X + 2 + i * 2, Y, iLevelType == 3 ? 159 : 47);
		setBlockID(X + 2 + i * 2, Y + 1, iLevelType == 3 ? 158 : 46);
	}

	for (int i = 0; i < 9; i++) {
		setBlockID(X + i, Y + 2, iLevelType == 3 ? 155 : 43);
	}

	for (int i = 0; i < 9; i++) {
		if (i < 2 || i > 6) {
			setBlockID(X + i, Y + 5, iLevelType == 3 ? 157 : 45);
		}
		else {
			setBlockID(X + i, Y + 5, iLevelType == 3 ? 156 : 44);
		}
	}


	for (int i = 0; i < 2; i++) {
		setBlockID(X + 3 + i * 2, Y, iLevelType == 3 ? 155 : 43);
		setBlockID(X + 3 + i * 2, Y + 1, iLevelType == 3 ? 155 : 43);
	}

	for (int i = 0; i < 2; i++) {
		setBlockID(X + 3 + i * 2, Y + 3, iLevelType == 3 ? 159 : 47);
		setBlockID(X + 3 + i * 2, Y + 4, iLevelType == 3 ? 158 : 46);
	}

	for (int i = 0; i < 3; i++) {
		setBlockID(X + 2 + i * 2, Y + 3, iLevelType == 3 ? 155 : 43);
		setBlockID(X + 2 + i * 2, Y + 4, iLevelType == 3 ? 155 : 43);
	}

	for (int i = 0; i < 2; i++) {
		setBlockID(X + 2, Y + 6 + i, iLevelType == 3 ? 155 : 43);
		setBlockID(X + 3, Y + 6 + i, iLevelType == 3 ? 155 : 43);
		setBlockID(X + 5, Y + 6 + i, iLevelType == 3 ? 155 : 43);
		setBlockID(X + 6, Y + 6 + i, iLevelType == 3 ? 155 : 43);
	}

	setBlockID(X + 4, Y + 6, iLevelType == 3 ? 159 : 47);
	setBlockID(X + 4, Y + 7, iLevelType == 3 ? 158 : 46);

	for (int i = 0; i < 3; i++) {
		setBlockID(X + 3 + i, Y + 8, iLevelType == 3 ? 156 : 44);
	}

	setBlockID(X + 2, Y + 8, iLevelType == 3 ? 157 : 45);
	setBlockID(X + 6, Y + 8, iLevelType == 3 ? 157 : 45);

	setBlockID(X + 2, Y + 8, iLevelType == 3 ? 157 : 45);

	setBlockID(X + 3, Y + 9, iLevelType == 3 ? 160 : 48);
	setBlockID(X + 4, Y + 9, iLevelType == 3 ? 155 : 43);
	setBlockID(X + 5, Y + 9, iLevelType == 3 ? 161 : 49);

	for (int i = 0; i < 3; i++) {
		setBlockID(X + 3 + i, Y + 10, iLevelType == 3 ? 157 : 45);
	}
}

void Map::structCastleWall(int X, int Y, int iWidth, int iHeight) {
	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight - 1; j++) {
			lMap[X + i][Y + j]->setBlockID(iLevelType == 3 ? 155 : 43);
		}
	}

	for (int i = 0; i < iWidth; i++) {
		lMap[X + i][Y + iHeight - 1]->setBlockID(iLevelType == 3 ? 157 : 45);
	}
}

void Map::structT(int X, int Y, int iWidth, int iHeight) {
	for (int i = 0; i < iHeight - 1; i++) {
		for (int j = 1; j < iWidth - 1; j++) {
			lMap[X + j][Y + i]->setBlockID(iLevelType == 3 ? 154 : 70);
		}
	}

	for (int i = 1; i < iWidth - 1; i++) {
		lMap[X + i][Y + iHeight - 1]->setBlockID(iLevelType == 3 ? 152 : 68);
	}

	lMap[X][Y + iHeight - 1]->setBlockID(iLevelType == 3 ? 151 : 67);
	lMap[X + iWidth - 1][Y + iHeight - 1]->setBlockID(iLevelType == 3 ? 153 : 69);
}

void Map::structTMush(int X, int Y, int iWidth, int iHeight) {
	for (int i = 0; i < iHeight - 2; i++) {
		lMap[X + iWidth / 2][Y + i]->setBlockID(144);
	}

	lMap[X + iWidth / 2][Y + iHeight - 2]->setBlockID(143);

	for (int i = 1; i < iWidth - 1; i++) {
		lMap[X + i][Y + iHeight - 1]->setBlockID(141);
	}

	lMap[X][Y + iHeight - 1]->setBlockID(140);
	lMap[X + iWidth - 1][Y + iHeight - 1]->setBlockID(142);
}

void Map::structWater(int X, int Y, int iWidth, int iHeight) {
	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight - 1; j++) {
			lMap[X + i][Y + j]->setBlockID(iLevelType == 2 ? 94 : 110);
		}
		lMap[X + i][Y + iHeight - 1]->setBlockID(iLevelType == 2 ? 95 : 111);
	}
}

void Map::structLava(int X, int Y, int iWidth, int iHeight) {
	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight - 1; j++) {
			lMap[X + i][Y + j]->setBlockID(77);
		}
		lMap[X + i][Y + iHeight - 1]->setBlockID(78);
	}
}

void Map::structBridge(int X, int Y, int iWidth) {
	for (int i = 0; i < iWidth; i++) {
		lMap[X + i][Y]->setBlockID(76);
	}

	lMap[X + iWidth - 1][Y + 1]->setBlockID(79);

	lMap[X + iWidth][6]->setBlockID(82);
	lMap[X + iWidth + 1][6]->setBlockID(83);
	lMap[X + iWidth + 1][7]->setBlockID(83);
	lMap[X + iWidth + 1][8]->setBlockID(83);
}

void Map::structBridge2(int X, int Y, int iWidth) {
	for (int i = 0; i < iWidth; i++) {
		lMap[X + i][Y]->setBlockID(107);
		lMap[X + i][Y + 1]->setBlockID(iLevelType == 4 ? 122 : 108);
	}
}

void Map::structSeeSaw(int X, int Y, int iWidth) {
	lMap[X][Y]->setBlockID(iLevelType == 3 ? 162 : 132);
	lMap[X + iWidth - 1][Y]->setBlockID(iLevelType == 3 ? 163 : 133);

	for (int i = 1; i < iWidth - 1; i++) {
		lMap[X + i][Y]->setBlockID(iLevelType == 3 ? 164 : 134);
	}
}

void Map::structPlatformLine(int X) {
	for (int i = 0; i < iMapHeight; i++) {
		lMap[X][i]->setBlockID(109);
	}
}

void Map::structBulletBill(int X, int Y, int iHieght) {
	lMap[X][Y + iHieght + 1]->setBlockID(145);
	lMap[X][Y + iHieght]->setBlockID(146);

	for (int i = 0; i < iHieght; i++) {
		lMap[X][Y + i]->setBlockID(147);
	}

	addBulletBillSpawner(X, Y + iHieght + 1, 0);
}

void Map::structTree(int X, int Y, int iHeight, bool BIG) {
	for (int i = 0; i < iHeight; i++) {
		lMap[X][Y + i]->setBlockID(91);
	}

	if (BIG) {
		lMap[X][Y + iHeight]->setBlockID(iLevelType == 4 ? 88 : 85);
		lMap[X][Y + iHeight + 1]->setBlockID(iLevelType == 4 ? 89 : 86);
	}
	else {
		lMap[X][Y + iHeight]->setBlockID(iLevelType == 4 ? 87 : 84);
	}
}

void Map::structFence(int X, int Y, int iWidth) {
	for (int i = 0; i < iWidth; i++) {
		lMap[X + i][Y]->setBlockID(90);
	}
}

void Map::setBlockID(int X, int Y, int iBlockID) {
	if (X >= 0 && X < iMapWidth) {
		lMap[X][Y]->setBlockID(iBlockID);
	}
}

bool Map::checkCollision(Vector2* nV, bool checkVisible) {
	bool output = vBlock[lMap[nV->getX()][nV->getY()]->getBlockID()]->getCollision() // Check if the block is collidable
		&& (checkVisible ? vBlock[lMap[nV->getX()][nV->getY()]->getBlockID()]->getVisible() : true);
	delete nV;
	return output;
}

bool Map::checkCollisionLB(int nX, int nY, int nHitBoxY, bool checkVisible) {
	return checkCollision(getBlockID(nX, nY + nHitBoxY), checkVisible);
}

bool Map::checkCollisionLT(int nX, int nY, bool checkVisible) {
	return checkCollision(getBlockID(nX, nY), checkVisible);
}

bool Map::checkCollisionLC(int nX, int nY, int nHitBoxY, bool checkVisible) {
	return checkCollision(getBlockID(nX, nY + nHitBoxY), checkVisible);
}

bool Map::checkCollisionRC(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible) {
	return checkCollision(getBlockID(nX + nHitBoxX, nY + nHitBoxY), checkVisible);
}

bool Map::checkCollisionRB(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible) {
	return checkCollision(getBlockID(nX + nHitBoxX, nY + nHitBoxY), checkVisible);
}

bool Map::checkCollisionRT(int nX, int nY, int nHitBoxX, bool checkVisible) {
	return checkCollision(getBlockID(nX + nHitBoxX, nY), checkVisible);
}

int Map::checkCollisionWithPlatform(int nX, int nY, int iHitBoxX, int iHitBoxY) {
	for (unsigned int i = 0; i < vPlatform.size(); i++) {
		if (-fXPos + nX + iHitBoxX >= vPlatform[i]->getXPos() && -fXPos + nX <= vPlatform[i]->getXPos() + vPlatform[i]->getSize() * 16) {
			if (nY + iHitBoxY >= vPlatform[i]->getYPos() && nY + iHitBoxY <= vPlatform[i]->getYPos() + 16) {
				return i;
			}
		}
	}
	return -1;
}

Vector2* Map::getBlockID(int nX, int nY) { // Return the block ID of the block at the given position x, y
	return new Vector2((int)(nX < 0 ? 0 : nX) / 32, (int)(nY > CFG::GameHeight - 16 ? 0 : (CFG::GameHeight - 16 - nY + 32) / 32));
}

int Map::getBlockIDX(int nX) {
	return (int)(nX < 0 ? 0 : nX) / 32;
}

int Map::getBlockIDY(int nY) {
	return (int)(nY > CFG::GameHeight - 16 ? 0 : (CFG::GameHeight - 16 - nY + 32) / 32);
}

void Map::checkCollisionOnTopOfTheBlock(int nX, int nY)
{
	switch (lMap[nX][nY + 1]->getBlockID()) {
	case 29: case 71: case 72: case 73:// COIN
		lMap[nX][nY + 1]->setBlockID(0);
		lCoin.push_back(new Coin(nX * 32 + 7, CFG::GameHeight - nY * 32 - 48));
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);
		pPlayer->setCoins(pPlayer->getCoins() + 1);
		return;
		break;
	}

	for (int i = (nX - nX % 5) / 5, iEnd = i + 3; i < iEnd && i < iMinionListSize; i++) {
		for (unsigned int j = 0; j < lMinion[i].size(); j++) {
			if (!lMinion[i][j]->collisionOnlyWithPlayer && lMinion[i][j]->getMinionState() >= 0 && ((lMinion[i][j]->getXPos() >= nX * 32 && lMinion[i][j]->getXPos() <= nX * 32 + 32) || (lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX >= nX * 32 && lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX <= nX * 32 + 32))) {
				if (lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= CFG::GameHeight - 24 - nY * 32 && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY <= CFG::GameHeight - nY * 32 + 16) {
					lMinion[i][j]->moveDirection = !lMinion[i][j]->moveDirection;
					lMinion[i][j]->setMinionState(-2);
				}
			}
		}
	}
}

float Map::getXPos(int iPlayer)
{
	if (iPlayer == 0) return fXPos;
	return fXPos2;
}

void Map::setXPos(float val)
{
	fXPos = val;
}

float Map::getYPos(int iPlayer)
{
	if (iPlayer == 0) return fYPos;
	return fYPos2;
}

void Map::setYPos(float val)
{
	fYPos = val;
}

int Map::getLevelType()
{
	return iLevelType;
}

void Map::setLevelType(int iLevelType)
{
	this->iLevelType = iLevelType;
}

int Map::getCurrentLevelID()
{
	return currentLevelID;
}

void Map::setCurrentLevelID(int currentLevelID)
{
	if (this->currentLevelID != currentLevelID) {
		this->currentLevelID = currentLevelID;
		pEvent->resetRedraw();
		loadLVL();
		iSpawnPointID = 0;
	}

	this->currentLevelID = currentLevelID;
}

bool Map::getMoveMap()
{
	return bMoveMap;
}

void Map::setMoveMap(bool bMoveMap)
{
	this->bMoveMap = bMoveMap;
}

bool Map::getUnderWater() {
	return underWater;
}

void Map::setUnderWater(bool underWater) {
	this->underWater = underWater;
}

Flag* Map::getFlag()
{
	return pFlag;
}

Player* Map::getPlayer()
{
    return pPlayer;
}

Player* Map::getPlayer2()
{
	return vPlayer[1];
}

Platform* Map::getPlatform(int iID)
{
	return vPlatform[iID];
}

Block* Map::getBlock(int iID)
{
	return vBlock[iID];
}

Tile* Map::getMapBlock(int iX, int iY) {
	return lMap[iX][iY];
}

Event* Map::getEvent() {
	return pEvent;
}

Block* Map::getMinionBlock(int iID) {
	return vMinion[iID];
}

void Map::setNumOfPlayers(int iNumOfPlayers)
{
	this->iNumOfPlayers = iNumOfPlayers;
	this->vCurPlayer.resize(iNumOfPlayers);
	for (int i = 0; i < iNumOfPlayers; i++) {
		vCurPlayer[i] = vPlayer[i];
	}
	if (iNumOfPlayers == 1) vPlayer[1]->setIDPlayer(0);
	if (iNumOfPlayers == 2) vPlayer[1]->setIDPlayer(1);
}

int Map::getNumOfPlayers()
{
	return iNumOfPlayers;
}

std::string Map::getLevelName() {
	return "" + std::to_string(1 + currentLevelID / 4) + "-" + std::to_string(currentLevelID % 4 + 1);
}

int Map::getMapWidth() {
	return iMapWidth;
}

bool Map::getDrawLines() {
	return drawLines;
}

void Map::setDrawLines(bool drawLines) {
	this->drawLines = drawLines;
}

int Map::getMapTime() {
	return iMapTime;
}

void Map::setMapTime(int iMapTime) {
	this->iMapTime = iMapTime;
}



void Map::loadMap(sf::RenderWindow& mainWindow)
{
	loadGameData(mainWindow);
	loadLVL();
}

void Map::loadGameDataBlock(const std::string& filename, sf::RenderWindow& mainWindow, std::vector<Block*>& vBlock,std::vector<std::string>& tSprite, std::vector<unsigned int>& iDelay) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open block data file: " << filename << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		int blockID;
		std::string textureInfo, collisionStr, deathStr, useStr, visibleStr;

		// Read the block ID
		ss >> blockID;

		while (ss >> textureInfo) {
			// Check if it's a boolean field (not a texture or delay)
			if (textureInfo == "true" || textureInfo == "false") {
				collisionStr = textureInfo; // First boolean value
				break;
			}
			size_t commaPos = textureInfo.find(",");
			std::string texture = textureInfo.substr(0, commaPos);
			int delay = std::stoi(textureInfo.substr(commaPos + 1));
			tSprite.push_back(texture);
			iDelay.push_back(delay);
		}
		std::string a, b, c;
		ss >> a >> b >> c;
		bool collision = stringToBool(collisionStr);
		bool death = stringToBool(a);
		bool use = stringToBool(b);
		bool visible = stringToBool(c);
		std::cout << blockID << " " << collision << " " << death << " " << use << " " << visible << std::endl;
		vBlock.push_back(new Block(blockID, new AniSprite(mainWindow, tSprite, iDelay, false), collision, death, use, visible));
		tSprite.clear();
		iDelay.clear();
	}

	file.close();
}

bool Map::stringToBool(const std::string& str) {
	return (str == "true"); 
}

void Map::loadGameDataMinion(const std::string& filename, sf::RenderWindow& mainWindow, std::vector<Block*>& vMinion, std::vector<std::string>& tSprite, std::vector<unsigned int>& iDelay) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open block data file: " << filename << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		int blockID;
		std::string textureInfo, collisionStr, deathStr, useStr, visibleStr;
		ss >> blockID;
		while (ss >> textureInfo) {
			if (textureInfo == "true" || textureInfo == "false") {
				collisionStr = textureInfo; //first 
				break;
			}
			size_t commaPos = textureInfo.find(",");
			std::string texture = textureInfo.substr(0, commaPos);
			int delay = std::stoi(textureInfo.substr(commaPos + 1));
			tSprite.push_back(texture);
			iDelay.push_back(delay);
		}
		std::string a, b, c;
		ss >> a >> b >> c;
		bool collision = stringToBool(collisionStr);
		bool death = stringToBool(a);
		bool use = stringToBool(b);
		bool visible = stringToBool(c);
		std::cout << blockID << " " << collision << " " << death << " " << use << " " << visible << std::endl;
		vMinion.push_back(new Block(blockID, new AniSprite(mainWindow, tSprite, iDelay, false), collision, death, use, visible));
		tSprite.clear();
		iDelay.clear();
	}

	file.close();
}

void Map::loadGameData(sf::RenderWindow& mainWindow)
{
	vPlayer.push_back(new Mario(mainWindow, 84, 480));
	vPlayer.push_back(new Luigi(mainWindow, 84, 480));
	indexPlayer = 0;
	pPlayer = vPlayer[indexPlayer];
	setNumOfPlayers(1);

	CFG::getText()->setFont(mainWindow, "font");

	std::vector<std::string> tSprite;
	std::vector<unsigned int> iDelay;
	loadGameDataBlock("files/GameData/Block.txt",mainWindow,vBlock,tSprite,iDelay);
	loadGameDataMinion("files/GameData/Minion.txt", mainWindow, vMinion, tSprite, iDelay);

	iBlockSize = vBlock.size();
	iMinionSize = vMinion.size();
}

void Map::loadLVL_1_1() {
	clearMap();

	this->iMapWidth = 260;
	this->iMapHeight = 25;
	this->iLevelType = 0;
	this->iMapTime = 400;

	// ---------- LOAD LISTS ----------
	createMap();

	// ----- MINIONS
	loadMinionsLVL_1_1();

	// ----- PIPEEVENT
	loadPipeEventsLVL_1_1();

	// ----- Bush -----

	structBush(4, 2, 1);
	structBush(16, 2, 1);
	structBush(48, 2, 2);
	structBush(64, 2, 1);
	structBush(96, 2, 2);
	structBush(112, 2, 1);
	structBush(144, 2, 2);
	structBush(160, 2, 1);
	structBush(192, 2, 2);
	structBush(208, 2, 1);

	// ----- Bush -----

	// ----- Clouds -----

	structCloud(8, 10, 1);
	structCloud(19, 11, 1);
	structCloud(27, 10, 3);
	structCloud(36, 11, 2);
	structCloud(56, 10, 1);
	structCloud(67, 11, 1);
	structCloud(75, 10, 3);
	structCloud(84, 11, 2);
	structCloud(104, 10, 1);
	structCloud(115, 11, 1);
	structCloud(123, 10, 3);
	structCloud(132, 11, 2);
	structCloud(152, 10, 1);
	structCloud(163, 11, 1);
	structCloud(171, 10, 3);
	structCloud(180, 11, 2);
	structCloud(200, 10, 1);
	structCloud(211, 11, 1);
	structCloud(219, 10, 3);

	// ----- Clouds -----

	// ----- Grass -----

	structGrass(11, 2, 3);
	structGrass(23, 2, 1);
	structGrass(41, 2, 2);
	structGrass(59, 2, 3);
	structGrass(71, 2, 1);
	structGrass(89, 2, 2);
	structGrass(107, 2, 3);
	structGrass(119, 2, 1);
	structGrass(137, 2, 2);
	structGrass(157, 2, 1);
	structGrass(167, 2, 1);
	structGrass(205, 2, 1);
	structGrass(215, 2, 1);

	// ----- Grass -----

	// ----- GND -----

	structGND(0, 0, 69, 2);

	structGND(71, 0, 15, 2);

	structGND(89, 0, 64, 2);

	structGND(155, 0, 85, 2);

	// ----- GND -----

	// ----- GND 2 -----

	structGND2(134, 2, 4, true);
	structGND2(140, 2, 4, false);
	structGND2(148, 2, 4, true);
	structGND2(152, 2, 1, 4);
	structGND2(155, 2, 4, false);
	structGND2(181, 2, 8, true);
	structGND2(189, 2, 1, 8);

	structGND2(198, 2, 1, 1);

	// ----- GND 2 -----

	// ----- BRICK -----

	struckBlockQ(16, 5, 1);
	structBrick(20, 5, 1, 1);
	struckBlockQ(21, 5, 1);
	lMap[21][5]->setSpawnMushroom(true);
	structBrick(22, 5, 1, 1);
	struckBlockQ(22, 9, 1);
	struckBlockQ(23, 5, 1);
	structBrick(24, 5, 1, 1);

	struckBlockQ2(64, 6, 1);
	lMap[64][6]->setSpawnMushroom(true);
	lMap[64][6]->setPowerUP(false);

	structBrick(77, 5, 1, 1);
	struckBlockQ(78, 5, 1);
	lMap[78][5]->setSpawnMushroom(true);
	structBrick(79, 5, 1, 1);

	structBrick(80, 9, 8, 1);
	structBrick(91, 9, 3, 1);
	struckBlockQ(94, 9, 1);
	structBrick(94, 5, 1, 1);
	lMap[94][5]->setNumOfUse(4);

	structBrick(100, 5, 2, 1);

	struckBlockQ(106, 5, 1);
	struckBlockQ(109, 5, 1);
	struckBlockQ(109, 9, 1);
	lMap[109][9]->setSpawnMushroom(true);
	struckBlockQ(112, 5, 1);

	structBrick(118, 5, 1, 1);

	structBrick(121, 9, 3, 1);

	structBrick(128, 9, 1, 1);
	struckBlockQ(129, 9, 2);
	structBrick(131, 9, 1, 1);

	structBrick(129, 5, 2, 1);

	structBrick(168, 5, 2, 1);
	struckBlockQ(170, 5, 1);
	structBrick(171, 5, 1, 1);

	lMap[101][5]->setSpawnStar(true);

	// ----- BRICK -----

	// ----- PIPES -----

	structPipe(28, 2, 1);
	structPipe(38, 2, 2);
	structPipe(46, 2, 3);
	structPipe(57, 2, 3);
	structPipe(163, 2, 1);
	structPipe(179, 2, 1);

	// ----- PIPES -----

	// ----- END

	structEnd(198, 3, 9);
	structCastleSmall(202, 2);

	// ----- MAP 1_1_2 -----

	this->iLevelType = 1;

	structGND(240, 0, 17, 2);

	structBrick(240, 2, 1, 11);
	structBrick(244, 2, 7, 3);
	structBrick(244, 12, 7, 1);

	structPipeVertical(255, 2, 10);
	structPipeHorizontal(253, 2, 1);

	structCoins(244, 5, 7, 1);
	structCoins(244, 7, 7, 1);
	structCoins(245, 9, 5, 1);

	// ----- END LEVEL

	this->iLevelType = 0;
}

void Map::loadLVL_1_2() {
	clearMap();

	this->iMapWidth = 330;
	this->iMapHeight = 25;
	this->iLevelType = 1;
	this->iMapTime = 400;

	// ---------- LOAD LISTS ----------

	createMap();

	// ---------- LOAD LISTS ----------

	// ----- MINIONS
	loadMinionsLVL_1_2();

	// ----- PIPEEVENTS
	loadPipeEventsLVL_1_2();

	vPlatform.push_back(new Platform(6, 0, 139 * 32 + 16, 139 * 32 + 16, -32, CFG::GameHeight + 32, 139 * 32 + 16, 9 * 32 + 16, true));
	vPlatform.push_back(new Platform(6, 0, 139 * 32 + 16, 139 * 32 + 16, -32, CFG::GameHeight + 32, 139 * 32 + 16, 1 * 32 + 16, true));

	vPlatform.push_back(new Platform(6, 1, 154 * 32 + 16, 154 * 32 + 16, -32, CFG::GameHeight + 32, 154 * 32 + 16, 10 * 32, true));
	vPlatform.push_back(new Platform(6, 1, 154 * 32 + 16, 154 * 32 + 16, -32, CFG::GameHeight + 32, 154 * 32 + 16, 2 * 32, true));

	vLevelText.push_back(new LevelText(178 * 32, CFG::GameHeight - 16 - 8 * 32, "WELCOME TO WARP ZONEz"));

	vLevelText.push_back(new LevelText(178 * 32 + 16, CFG::GameHeight - 6 * 32, "4"));
	vLevelText.push_back(new LevelText(182 * 32 + 16, CFG::GameHeight - 6 * 32, "3"));
	vLevelText.push_back(new LevelText(186 * 32 + 16, CFG::GameHeight - 6 * 32, "2"));

	// ----- GND -----

	structGND(0, 0, 80, 2);

	structGND(83, 0, 37, 2);

	structGND(122, 0, 2, 2);

	structGND(126, 0, 12, 2);

	structGND(145, 0, 8, 2);

	structGND(160, 0, 43, 2);

	// ----- GND -----

	// ----- Brick -----

	structBrick(0, 2, 1, 11);
	structBrick(122, 2, 2, 3);

	structBrick(6, 12, 132, 1);

	structBrick(29, 6, 1, 1);
	lMap[29][6]->setNumOfUse(10);

	structBrick(39, 5, 1, 3);
	structBrick(40, 5, 1, 1);
	structBrick(41, 5, 1, 3);
	structBrick(42, 7, 2, 1);
	structBrick(44, 5, 1, 3);
	structBrick(45, 5, 1, 1);
	structBrick(46, 5, 1, 3);

	structBrick(52, 5, 2, 5);
	structBrick(54, 3, 2, 3);
	structBrick(54, 10, 2, 2);
	structBrick(58, 10, 6, 2);
	structBrick(58, 5, 6, 1);
	structBrick(62, 6, 2, 4);

	structBrick(66, 10, 4, 2);
	structBrick(67, 5, 1, 5);
	structBrick(68, 5, 1, 1);
	structBrick(69, 5, 1, 2);
	lMap[69][6]->setSpawnMushroom(true);

	structBrick(72, 5, 2, 5);
	lMap[73][6]->setNumOfUse(10);

	structBrick(76, 5, 4, 1);
	structBrick(76, 10, 4, 2);

	structBrick(84, 6, 6, 2);
	lMap[89][12]->setSpawnMushroom(true);
	lMap[89][12]->setPowerUP(false);

	structBrick(145, 6, 6, 1);
	lMap[150][6]->setSpawnMushroom(true);

	structBrick(160, 2, 17, 3);
	structBrick(170, 5, 7, 8);
	structBrick(161, 12, 7, 1);
	structBrick(177, 12, 10, 1);

	structBrick(190, 2, 13, 17);

	lMap[46][7]->setSpawnStar(true);

	// ----- Brick -----

	// ----- GND2 -----

	structGND2(17, 2, 1, 1);
	structGND2(19, 2, 1, 2);
	structGND2(21, 2, 1, 3);
	structGND2(23, 2, 1, 4);
	structGND2(25, 2, 1, 4);
	structGND2(27, 2, 1, 3);
	structGND2(31, 2, 1, 3);
	structGND2(33, 2, 1, 2);

	structGND2(133, 2, 4, true);
	structGND2(137, 2, 1, 4);

	// ----- GND2 -----

	// ----- BLOCKQ -----

	struckBlockQ(10, 5, 5);
	lMap[10][5]->setSpawnMushroom(true);

	// ----- BLOCKQ -----

	// ----- PIPES -----

	structPipe(103, 2, 2);
	structPipe(109, 2, 3);
	structPipe(115, 2, 1);

	structPipeVertical(168, 5, 7);
	structPipeHorizontal(166, 5, 1);

	structPipe(178, 2, 2);
	structPipe(182, 2, 2);
	structPipe(186, 2, 2);

	// ----- PIPES -----

	// ----- COIN -----

	structCoins(40, 6, 1, 1);
	structCoins(41, 9, 4, 1);
	structCoins(45, 6, 1, 1);
	structCoins(58, 6, 4, 1);
	structCoins(68, 6, 1, 1);

	structCoins(84, 9, 6, 1);

	// ----- MAP 1-1-2

	this->iLevelType = 0;

	structGND(210, 0, 24, 2);

	structCastleSmall(210, 2);

	structCloud(213, 10, 2);
	structCloud(219, 6, 1);
	structCloud(228, 11, 2);

	structPipe(222, 2, 3);
	structPipeHorizontal(220, 2, 1);

	// ----- MAP 1-1-2
	// ----- MAP 1-1-3

	this->iLevelType = 1;

	structGND(240, 0, 17, 2);

	structBrick(240, 2, 1, 11);
	structBrick(243, 5, 10, 1);
	structBrick(243, 9, 10, 4);
	structBrick(253, 4, 2, 9);

	lMap[252][5]->setNumOfUse(10);

	structCoins(243, 2, 9, 1);
	structCoins(244, 6, 8, 1);

	structPipeVertical(255, 2, 10);
	structPipeHorizontal(253, 2, 1);

	// ----- MAP 1-1-3
	// ----- MAP 1-1-4

	this->iLevelType = 0;

	structCloud(285, 11, 2);
	structCloud(305, 10, 1);
	structCloud(316, 11, 1);

	structGrass(310, 2, 1);
	structGrass(320, 2, 1);

	structBush(297, 2, 2);
	structBush(313, 2, 1);

	structGND(281, 0, 42, 2);

	structPipe(284, 2, 1);

	structGND2(286, 2, 8, true);
	structGND2(294, 2, 1, 8);

	structGND2(303, 2, 1, 1);
	structEnd(303, 3, 9);

	structCastleSmall(307, 2);

	// ----- MAP 1-1-4

	// ----- END

	this->iLevelType = 1;
}

void Map::loadLVL_1_3() {
	clearMap();

	this->iMapWidth = 270;
	this->iMapHeight = 25;
	this->iLevelType = 0;
	this->iMapTime = 300;

	// ---------- LOAD LISTS ----------

	createMap();

	loadMinionsLVL_1_3();

	// ---------- LOAD LISTS ----------

	vPlatform.push_back(new Platform(6, 2, 55 * 32, 55 * 32, CFG::GameHeight - 16 - 9 * 32, CFG::GameHeight - 32, 55 * 32, (float)CFG::GameHeight - 32, true));

	vPlatform.push_back(new Platform(6, 3, 83 * 32 - 16, 86 * 32 + 16, CFG::GameHeight - 16 - 6 * 32, CFG::GameHeight - 16 - 6 * 32, 83 * 32 - 16, (float)CFG::GameHeight - 16 - 6 * 32, true));

	vPlatform.push_back(new Platform(6, 3, 91 * 32 - 16, 93 * 32 + 16, CFG::GameHeight - 16 - 5 * 32, CFG::GameHeight - 16 - 5 * 32, 93 * 32 + 16, (float)CFG::GameHeight - 16 - 5 * 32, false));

	vPlatform.push_back(new Platform(6, 3, 127 * 32 - 16, 131 * 32 + 16, CFG::GameHeight - 16 - 8 * 32, CFG::GameHeight - 16 - 8 * 32, 127 * 32 - 16, (float)CFG::GameHeight - 16 - 8 * 32, false));

	// ----- CLOUDS

	structCloud(3, 10, 2);
	structCloud(9, 6, 1);
	structCloud(11, 18, 2);
	structCloud(18, 11, 2);
	structCloud(35, 6, 1);
	structCloud(38, 7, 1);
	structCloud(46, 2, 1);
	structCloud(51, 10, 2);
	structCloud(57, 6, 1);
	structCloud(66, 11, 2);
	structCloud(76, 2, 1);
	structCloud(83, 6, 1);
	structCloud(86, 7, 1);
	structCloud(94, 2, 1);
	structCloud(99, 10, 2);
	structCloud(131, 6, 1);
	structCloud(134, 7, 1);
	structCloud(142, 2, 1);
	structCloud(147, 10, 2);
	structCloud(153, 6, 1);
	structCloud(162, 11, 2);

	// ----- GND

	structGND(0, 0, 16, 2);
	structGND(129, 0, 47, 2);
	// ----- T

	structT(18, 0, 4, 3);
	structT(24, 0, 8, 6);
	structT(26, 6, 5, 4);
	structT(32, 0, 3, 3);
	structT(35, 0, 5, 7);
	structT(40, 0, 7, 11);
	structT(50, 0, 4, 2);
	structT(59, 0, 5, 2);
	structT(60, 2, 4, 8);
	structT(50, 0, 4, 2);
	structT(65, 0, 5, 2);
	structT(70, 0, 3, 6);
	structT(76, 0, 6, 9);
	structT(98, 0, 4, 4);
	structT(104, 0, 8, 8);
	structT(113, 0, 3, 2);
	structT(116, 0, 4, 6);
	structT(122, 0, 4, 6);

	// ----- GND2

	struckBlockQ(59, 4, 1);
	lMap[59][4]->setSpawnMushroom(true);

	structGND2(138, 2, 2, 4);
	structGND2(140, 2, 2, 6);
	structGND2(142, 2, 2, 8);

	// ----- COINS

	structCoins(27, 10, 3, 1);
	structCoins(33, 3, 1, 1);
	structCoins(37, 12, 2, 1);
	structCoins(50, 8, 2, 1);
	structCoins(60, 10, 4, 1);
	structCoins(85, 9, 2, 1);
	structCoins(93, 10, 2, 1);
	structCoins(97, 10, 2, 1);
	structCoins(113, 2, 3, 1);
	structCoins(120, 9, 2, 1);

	// ----- END

	structGND2(152, 2, 1, 1);
	structEnd(152, 3, 9);

	// ----- CASTLE

	structCastleSmall(0, 2);
	structCastleBig(155, 2);
	structCastleWall(164, 2, 12, 6);

	this->iLevelType = 0;
}

void Map::loadLVL_1_4() {
	clearMap();

	this->iMapWidth = 180;
	this->iMapHeight = 25;
	this->iLevelType = 3;
	this->iMapTime = 300;

	// ---------- LOAD LISTS ----------

	createMap();

	loadMinionsLVL_1_4();

	vPlatform.push_back(new Platform(4, 3, 136 * 32 - 16, 138 * 32 + 16, CFG::GameHeight - 16 - 8 * 32, CFG::GameHeight - 16 - 8 * 32, 136 * 32 - 16, (float)CFG::GameHeight - 16 - 8 * 32, false));

	structGND(0, 0, 3, 8);
	structGND(3, 0, 1, 7);
	structGND(4, 0, 1, 6);
	structGND(5, 0, 8, 5);
	structGND(15, 0, 11, 5);
	structGND(29, 0, 3, 5);
	structGND(35, 0, 37, 6);
	structGND(72, 0, 32, 5);
	structGND(104, 0, 24, 2);
	structGND(116, 2, 4, 3);
	structGND(123, 2, 5, 3);
	structGND(141, 0, 3, 6);
	structGND(144, 0, 19, 2);

	structGND(0, 10, 24, 3);
	structGND(23, 9, 1, 4);
	structGND(24, 12, 13, 1);
	structGND(37, 9, 35, 4);
	structGND(72, 12, 91, 1);
	structGND(80, 11, 1, 1);
	structGND(88, 11, 1, 1);
	structGND(97, 10, 7, 2);
	structGND(123, 10, 5, 2);
	structGND(142, 9, 2, 3);

	structBridge(128, 4, 13);

	structLava(13, 0, 2, 3);
	structLava(26, 0, 3, 2);
	structLava(32, 0, 3, 2);
	structLava(128, 0, 13, 2);

	lMap[23][8]->setBlockID(56);
	lMap[37][8]->setBlockID(56);
	lMap[80][10]->setBlockID(56);
	lMap[92][5]->setBlockID(56);

	lMap[30][4]->setBlockID(56);
	lMap[49][8]->setBlockID(56);
	lMap[60][8]->setBlockID(56);
	lMap[67][8]->setBlockID(56);
	lMap[76][5]->setBlockID(56);
	lMap[84][5]->setBlockID(56);
	lMap[88][10]->setBlockID(56);

	struckBlockQ(30, 8, 1);
	lMap[30][8]->setSpawnMushroom(true);

	struckBlockQ2(106, 5, 1);
	struckBlockQ2(109, 5, 1);
	struckBlockQ2(112, 5, 1);
	struckBlockQ2(107, 9, 1);
	struckBlockQ2(110, 9, 1);
	struckBlockQ2(113, 9, 1);

	this->iLevelType = 3;
}

void Map::loadLVL_2_1() {
	clearMap();

	this->iMapWidth = 365;
	this->iMapHeight = 25;
	this->iLevelType = 0;
	this->iMapTime = 400;

	// ---------- LOAD LISTS ----------

	createMap();

	structGND(0, 0, 92, 2);
	structGND(96, 0, 10, 2);
	structGND(109, 0, 30, 2);
	structGND(142, 0, 10, 2);
	structGND(154, 0, 70, 2);

	loadMinionsLVL_2_1();
	loadPipeEventsLVL_2_1();

	structCloud(18, 10, 1);
	structCloud(27, 11, 1);
	structCloud(30, 10, 2);
	structCloud(45, 11, 1);
	structCloud(48, 10, 2);
	structCloud(66, 10, 1);
	structCloud(75, 11, 1);
	structCloud(78, 10, 2);
	structCloud(93, 11, 1);
	structCloud(96, 10, 2);
	structCloud(114, 10, 1);
	structCloud(123, 11, 1);
	structCloud(126, 10, 2);
	structCloud(141, 11, 1);
	structCloud(144, 10, 2);
	structCloud(162, 10, 1);
	structCloud(171, 11, 1);
	structCloud(174, 10, 2);
	structCloud(189, 11, 1);
	structCloud(192, 10, 2);
	structCloud(210, 10, 1);
	structCloud(219, 11, 1);

	structFence(14, 2, 4);
	structFence(38, 2, 2);
	structFence(41, 2, 1);
	structFence(62, 2, 4);
	structFence(86, 2, 2);
	structFence(89, 2, 1);
	structFence(110, 2, 4);
	structFence(134, 2, 2);
	structFence(137, 2, 1);
	structFence(158, 2, 4);
	structFence(182, 2, 2);
	structFence(185, 2, 1);
	structFence(209, 2, 1);

	structPipe(46, 2, 3);
	structPipe(74, 2, 3);
	structPipe(103, 2, 3);
	structPipe(115, 2, 1);
	structPipe(122, 2, 3);
	structPipe(126, 2, 2);
	structPipe(130, 2, 4);
	structPipe(176, 2, 2);

	structCastleBig(-2, 2);

	structTree(11, 2, 1, false);
	structTree(13, 2, 1, true);
	structTree(21, 2, 1, true);
	structTree(40, 2, 1, false);
	structTree(43, 2, 1, true);
	structTree(59, 2, 1, false);
	structTree(61, 2, 1, true);
	structTree(69, 2, 1, true);
	structTree(71, 2, 1, false);
	structTree(72, 2, 1, false);
	structTree(88, 2, 1, false);
	structTree(91, 2, 1, true);
	structTree(109, 2, 1, true);
	structTree(117, 2, 1, true);
	structTree(119, 2, 1, false);
	structTree(120, 2, 1, false);
	structTree(136, 2, 1, false);
	structTree(155, 2, 1, false);
	structTree(157, 2, 1, true);
	structTree(165, 2, 1, true);
	structTree(167, 2, 1, false);
	structTree(168, 2, 1, false);
	structTree(184, 2, 1, false);
	structTree(187, 2, 1, true);
	structTree(203, 2, 1, false);
	structTree(213, 2, 1, true);
	structTree(215, 2, 1, false);
	structTree(216, 2, 1, false);

	structBrick(15, 5, 3, 1);
	structBrick(28, 9, 4, 1);
	structBrick(68, 5, 1, 1);
	structBrick(69, 9, 4, 1);
	structBrick(81, 9, 5, 1);
	structBrick(92, 9, 4, 1);
	structBrick(125, 9, 4, 1);
	structBrick(161, 5, 1, 1);
	lMap[161][5]->setNumOfUse(7);
	structBrick(164, 9, 5, 1);
	structBrick(172, 9, 1, 1);
	structBrick(185, 5, 2, 1);

	lMap[69][9]->setSpawnStar(true);

	lMap[16][5]->setSpawnMushroom(true);
	lMap[28][9]->setSpawnMushroom(true);
	lMap[28][9]->setPowerUP(false);
	lMap[53][5]->setSpawnMushroom(true);
	lMap[125][9]->setSpawnMushroom(true);
	lMap[172][9]->setSpawnMushroom(true);

	struckBlockQ(53, 5, 5);
	struckBlockQ(53, 9, 5);
	struckBlockQ(79, 5, 4);
	struckBlockQ(85, 5, 3);
	struckBlockQ(170, 5, 1);

	struckBlockQ2(28, 5, 1);
	struckBlockQ2(186, 9, 1);

	structGND2(20, 2, 5, true);
	structGND2(34, 2, 1, 4);
	structGND2(35, 2, 1, 2);
	structGND2(154, 2, 1, 3);
	structGND2(190, 2, 2, 10);

	structGND2(200, 2, 1, 1);
	structEnd(200, 3, 9);
	structCastleSmall(204, 2);

	// -- MAP 2-1-2

	this->iLevelType = 1;

	structGND(230, 0, 17, 2);

	structBrick(230, 2, 1, 11);
	structBrick(234, 2, 7, 3);
	structBrick(234, 12, 7, 1);

	structPipeVertical(245, 2, 10);
	structPipeHorizontal(243, 2, 1);

	structCoins(234, 5, 7, 1);
	structCoins(234, 7, 7, 1);
	structCoins(235, 9, 5, 1);

	// -- MAP 2-1-3

	this->iLevelType = 0;

	lMap[83][9]->setBlockID(128);

	structBonus(270, 1, 4);
	structBonus(275, 1, 57);

	lMap[274][0]->setBlockID(130); // VINE
	lMap[274][1]->setBlockID(83);

	structCoins(285, 8, 16, 1);
	structCoins(302, 10, 3, 1);
	structCoins(306, 9, 16, 1);
	structCoins(323, 10, 3, 1);
	structCoins(333, 2, 3, 1);

	structBonusEnd(332);

	vPlatform.push_back(new Platform(6, 4, 286 * 32 + 16, 335 * 32 + 16, CFG::GameHeight - 16 - 4 * 32, CFG::GameHeight - 16 - 4 * 32, 286 * 32.0f + 16, CFG::GameHeight - 16.0f - 4 * 32, true));

	// -- BONUS END

	this->iLevelType = 0;
}

void Map::loadLVL_2_2() {
	clearMap();

	this->iMapWidth = 320;
	this->iMapHeight = 25;
	this->iLevelType = 2;
	this->iMapTime = 400;

	// ---------- LOAD LISTS ----------

	createMap();

	loadMinionsLVL_2_2();
	loadPipeEventsLVL_2_2();

	this->underWater = true;

	structWater(0, 0, 200, 13);

	structGND(0, 0, 66, 2);
	structGND(71, 0, 60, 2);
	structGND(140, 0, 17, 2);
	structGND(164, 0, 38, 2);

	structGND(64, 2, 1, 3);
	structGND(65, 2, 1, 5);
	structGND(71, 2, 1, 5);
	structGND(72, 2, 1, 3);

	structGND(78, 2, 2, 3);
	structGND(78, 10, 2, 3);
	structGND(82, 9, 3, 1);

	structGND(129, 2, 1, 4);
	structGND(130, 2, 1, 2);
	structGND(141, 2, 1, 4);
	structGND(140, 2, 1, 2);

	structGND(131, 10, 1, 3);
	structGND(132, 10, 8, 1);

	structGND(18, 5, 3, 1);
	structGND(42, 5, 2, 1);
	structGND(102, 5, 2, 1);
	structGND(115, 6, 2, 1);

	structGND(156, 2, 1, 8);
	structGND(157, 9, 2, 1);
	structGND(164, 2, 1, 8);
	structGND(162, 9, 2, 1);

	structGND(172, 5, 5, 1);
	structGND(172, 9, 5, 1);
	structGND(180, 5, 4, 1);
	structGND(180, 9, 4, 1);

	structCoins(14, 2, 2, 1);
	structCoins(27, 9, 3, 1);
	structCoins(36, 2, 3, 1);
	structCoins(67, 4, 3, 1);
	structCoins(101, 3, 3, 1);
	structCoins(113, 8, 3, 1);
	structCoins(133, 3, 1, 1);
	structCoins(134, 2, 3, 1);
	structCoins(137, 3, 1, 1);
	structCoins(159, 5, 3, 1);
	structCoins(159, 2, 3, 1);

	structUW1(11, 2, 1, 3);
	structUW1(33, 2, 1, 5);
	structUW1(42, 6, 1, 2);
	structUW1(50, 2, 1, 4);
	structUW1(83, 10, 1, 2);
	structUW1(89, 2, 1, 3);
	structUW1(102, 6, 1, 4);
	structUW1(120, 2, 1, 4);
	structUW1(147, 2, 1, 2);
	structUW1(149, 2, 1, 3);
	structUW1(173, 10, 1, 2);

	structPipeHorizontal(189, 6, 1);

	structGND(185, 2, 17, 1);
	structGND(186, 3, 16, 1);
	structGND(187, 4, 15, 1);
	structGND(188, 5, 14, 1);
	structGND(190, 6, 12, 3);
	structGND(188, 9, 14, 4);

	// -- MAP 2-2-2

	this->iLevelType = 0;

	structGND(220, 0, 24, 2);

	structCastleSmall(220, 2);

	structCloud(223, 10, 2);
	structCloud(229, 6, 1);
	structCloud(238, 11, 2);

	structPipe(232, 2, 3);
	structPipeHorizontal(230, 2, 1);

	// -- MAP 2-2-3

	structCloud(275, 11, 2);
	structCloud(295, 10, 1);
	structCloud(306, 11, 1);

	structGrass(300, 2, 1);
	structGrass(310, 2, 1);

	structBush(287, 2, 2);
	structBush(303, 2, 1);

	structGND(271, 0, 42, 2);

	structPipe(274, 2, 1);

	structGND2(276, 2, 8, true);
	structGND2(284, 2, 1, 8);

	structGND2(293, 2, 1, 1);
	structEnd(293, 3, 9);

	structCastleSmall(297, 2);

	// -- END

	this->iLevelType = 2;
}

void Map::loadLVL_2_3() {
	clearMap();

	this->iMapWidth = 250;
	this->iMapHeight = 25;
	this->iLevelType = 0;
	this->iMapTime = 300;

	// ---------- LOAD LISTS ----------

	createMap();

	loadMinionsLVL_2_3();

	structGND(0, 0, 7, 2);
	structGND(207, 0, 43, 2);

	structCloud(3, 10, 2);
	structCloud(9, 6, 1);
	structCloud(18, 11, 2);
	structCloud(28, 2, 1);
	structCloud(35, 6, 1);
	structCloud(38, 7, 1);
	structCloud(46, 2, 1);
	structCloud(51, 10, 2);
	structCloud(57, 6, 1);
	structCloud(66, 11, 2);
	structCloud(76, 2, 1);
	structCloud(83, 6, 1);
	structCloud(86, 7, 1);
	structCloud(94, 2, 1);
	structCloud(99, 10, 2);
	structCloud(105, 6, 1);
	structCloud(114, 11, 2);
	structCloud(124, 2, 1);
	structCloud(131, 6, 1);
	structCloud(134, 7, 1);
	structCloud(142, 2, 1);
	structCloud(147, 10, 2);
	structCloud(153, 6, 1);
	structCloud(162, 11, 2);
	structCloud(172, 2, 1);
	structCloud(179, 6, 1);
	structCloud(182, 7, 1);
	structCloud(190, 2, 1);
	structCloud(195, 10, 2);
	structCloud(201, 6, 1);
	structCloud(210, 11, 2);
	structCloud(220, 2, 1);
	structCloud(227, 6, 1);
	structCloud(239, 9, 2);

	structT(8, 0, 8, 2);
	structGND2(10, 2, 3, true);
	structGND2(13, 2, 2, 3);

	structBridge2(15, 4, 16);
	structBridge2(32, 4, 15);
	structBridge2(48, 4, 15);
	structBridge2(69, 4, 10);
	structBridge2(85, 4, 10);
	structBridge2(100, 5, 5);

	structT(112, 0, 8, 2);

	structBridge2(122, 4, 3);
	structBridge2(128, 4, 15);
	structBridge2(147, 2, 8);
	structBridge2(160, 4, 8);

	structBridge2(171, 4, 2);
	structBridge2(175, 4, 2);
	structBridge2(179, 4, 2);

	structBridge2(184, 4, 9);

	structGND2(31, 0, 1, 5);
	structGND2(47, 0, 1, 5);
	structGND2(63, 0, 1, 5);
	structGND2(68, 0, 1, 5);
	structGND2(79, 0, 1, 5);
	structGND2(84, 0, 1, 5);
	structGND2(95, 0, 1, 5);
	structGND2(99, 0, 1, 6);
	structGND2(105, 0, 1, 6);
	structGND2(127, 0, 1, 5);
	structGND2(143, 0, 1, 5);
	structGND2(146, 0, 1, 3);
	structGND2(155, 0, 1, 3);
	structGND2(159, 0, 1, 5);
	structGND2(168, 0, 1, 5);
	structGND2(183, 0, 1, 5);

	structT(192, 0, 13, 2);

	structGND2(193, 2, 1, 3);
	structGND2(194, 2, 3, false);

	structGND2(208, 2, 8, true);
	structGND2(216, 2, 1, 8);

	structCoins(36, 9, 4, 1);
	structCoins(55, 9, 1, 1);
	structCoins(57, 9, 1, 1);
	structCoins(59, 9, 1, 1);
	structCoins(56, 8, 1, 1);
	structCoins(58, 8, 1, 1);
	structCoins(72, 9, 1, 1);
	structCoins(73, 10, 2, 1);
	structCoins(75, 9, 1, 1);
	structCoins(97, 9, 3, 1);
	structCoins(108, 9, 3, 1);
	structCoins(133, 9, 6, 1);
	structCoins(149, 6, 4, 1);
	structCoins(173, 8, 6, 1);

	struckBlockQ(102, 9, 1);
	lMap[102][9]->setSpawnMushroom(true);

	structCastleSmall(0, 2);

	structGND2(225, 2, 1, 1);
	structEnd(225, 3, 9);

	structCastleBig(228, 2);
	structCastleWall(237, 2, 13, 6);

	// -- END

	this->iLevelType = 0;
}

void Map::loadLVL_2_4() {
	clearMap();

	this->iMapWidth = 190;
	this->iMapHeight = 25;
	this->iLevelType = 3;
	this->iMapTime = 300;

	// ---------- LOAD LISTS ----------
	createMap();

	// ----- MINIONS
	loadMinionsLVL_2_4();

	vPlatform.push_back(new Platform(3, 1, 85 * 32 + 24, 85 * 32 + 16, -32, CFG::GameHeight + 32, 85 * 32 + 24, 9 * 32 + 16, true));
	vPlatform.push_back(new Platform(3, 1, 85 * 32 + 24, 85 * 32 + 16, -32, CFG::GameHeight + 32, 85 * 32 + 24, 1 * 32 + 16, true));

	vPlatform.push_back(new Platform(3, 0, 88 * 32 + 24, 88 * 32 + 16, -32, CFG::GameHeight + 32, 88 * 32 + 24, 10 * 32, true));
	vPlatform.push_back(new Platform(3, 0, 88 * 32 + 24, 88 * 32 + 16, -32, CFG::GameHeight + 32, 88 * 32 + 24, 2 * 32, true));

	vPlatform.push_back(new Platform(4, 3, 136 * 32 - 16, 138 * 32 + 16, CFG::GameHeight - 16 - 8 * 32, CFG::GameHeight - 16 - 8 * 32, 136 * 32 - 16, (float)CFG::GameHeight - 16 - 8 * 32, false));

	structGND(32, 0, 52, 2);
	structGND(92, 0, 17, 2);
	structGND(115, 0, 13, 2);
	structGND(141, 0, 40, 2);

	structGND(0, 0, 16, 5);
	structGND(0, 5, 5, 1);
	structGND(0, 6, 4, 1);
	structGND(0, 7, 3, 1);
	structGND(18, 5, 2, 1);
	structGND(22, 7, 3, 1);
	structGND(27, 5, 2, 1);

	structGND(32, 2, 2, 3);
	structGND(37, 5, 37, 1);
	structGND(80, 2, 4, 3);
	structGND(92, 2, 7, 3);
	structGND(108, 2, 1, 3);
	structGND(111, 0, 2, 5);
	structGND(115, 2, 5, 3);
	structGND(122, 2, 2, 3);
	structGND(126, 2, 2, 3);
	structGND(141, 2, 3, 4);

	structGND(0, 10, 16, 3);
	structGND(34, 9, 49, 4);
	structGND(93, 10, 6, 3);
	structGND(115, 10, 13, 3);
	structGND(128, 12, 42, 1);
	structGND(142, 9, 2, 3);

	lMap[23][7]->setBlockID(56);
	lMap[43][1]->setBlockID(56);
	lMap[49][5]->setBlockID(56);
	lMap[55][9]->setBlockID(56);
	lMap[55][1]->setBlockID(56);
	lMap[61][5]->setBlockID(56);
	lMap[67][1]->setBlockID(56);
	lMap[73][5]->setBlockID(56);
	lMap[82][8]->setBlockID(56);
	lMap[92][4]->setBlockID(56);
	lMap[103][3]->setBlockID(56);

	structCoins(102, 2, 3, 1);
	structCoins(102, 6, 3, 1);

	struckBlockQ(23, 11, 1);
	lMap[23][11]->setSpawnMushroom(true);

	structBridge(128, 4, 13);

	structBrick(128, 9, 6, 1);

	structLava(16, 0, 16, 2);
	structLava(109, 0, 2, 2);
	structLava(113, 0, 2, 2);
	structLava(128, 0, 13, 2);

	structPlatformLine(86);
	structPlatformLine(89);

	this->iLevelType = 3;
}

void Map::loadLVL_3_1() {
	clearMap();

	this->iMapWidth = 440;
	this->iMapHeight = 25;
	this->iLevelType = 0;
	this->iMapTime = 300;

	// ---------- LOAD LISTS ----------

	createMap();

	loadMinionsLVL_3_1();
	loadPipeEventsLVL_3_1();

	structGND(0, 0, 46, 2);
	structGND(47, 0, 1, 2);
	structGND(49, 0, 2, 2);
	structGND(52, 0, 2, 2);
	structGND(55, 0, 2, 2);
	structGND(58, 0, 111, 2);
	structGND(170, 0, 1, 2);
	structGND(172, 0, 2, 2);
	structGND(175, 0, 1, 2);
	structGND(177, 0, 2, 2);
	structGND(180, 0, 17, 2);
	structGND(198, 0, 1, 2);
	structGND(200, 0, 1, 2);
	structGND(202, 0, 19, 2);
	structGND(227, 0, 10, 2);
	structGND(238, 0, 2, 2);
	structGND(242, 0, 2, 2);
	structGND(246, 0, 44, 2);
	structGND(293, 0, 2, 2);
	structGND(298, 0, 16, 2);
	structGND(319, 0, 1, 2);
	structGND(324, 0, 35, 2);
	structGND(360, 0, 1, 2);
	structGND(362, 0, 1, 2);
	structGND(364, 0, 1, 2);
	structGND(366, 0, 34, 2);

	structCastleBig(-2, 2);
	structCastleSmall(380, 2);

	structCloud(18, 10, 1);
	structCloud(27, 11, 1);
	structCloud(30, 10, 2);
	structCloud(45, 11, 1);
	structCloud(48, 10, 2);
	structCloud(66, 10, 1);
	structCloud(75, 11, 1);
	structCloud(78, 10, 2);
	structCloud(93, 11, 1);
	structCloud(96, 10, 2);
	structCloud(114, 10, 1);
	structCloud(123, 11, 1);
	structCloud(126, 10, 2);
	structCloud(141, 11, 1);
	structCloud(144, 10, 2);
	structCloud(162, 10, 1);
	structCloud(171, 11, 1);
	structCloud(174, 10, 2);
	structCloud(189, 11, 1);
	structCloud(192, 10, 2);
	structCloud(210, 10, 1);
	structCloud(219, 11, 1);
	structCloud(222, 10, 2);
	structCloud(237, 11, 1);
	structCloud(240, 10, 2);
	structCloud(258, 10, 1);
	structCloud(267, 11, 1);
	structCloud(270, 10, 2);
	structCloud(285, 11, 1);
	structCloud(288, 10, 2);
	structCloud(306, 10, 1);
	structCloud(315, 11, 1);
	structCloud(318, 10, 2);
	structCloud(333, 11, 1);
	structCloud(336, 10, 2);
	structCloud(354, 10, 1);
	structCloud(363, 11, 1);
	structCloud(366, 10, 2);
	structCloud(381, 11, 1);
	structCloud(384, 10, 2);

	structTree(11, 2, 1, false);
	structTree(13, 2, 1, true);
	structTree(21, 2, 1, true);
	structTree(23, 2, 1, false);
	structTree(24, 2, 1, false);
	structTree(40, 2, 1, false);
	structTree(43, 2, 1, true);
	structTree(59, 2, 1, false);
	structTree(61, 2, 1, true);
	structTree(69, 2, 1, true);
	structTree(71, 2, 1, false);
	structTree(72, 2, 1, false);
	structTree(88, 2, 1, false);
	structTree(91, 2, 1, true);
	structTree(107, 2, 1, false);
	structTree(109, 2, 1, false);
	structTree(117, 2, 1, true);
	structTree(119, 2, 1, false);
	structTree(120, 2, 1, false);
	structTree(136, 2, 1, false);
	structTree(139, 2, 1, true);
	structTree(155, 2, 1, false);
	structTree(157, 2, 1, true);
	structTree(165, 2, 1, true);
	structTree(167, 2, 1, false);
	structTree(168, 2, 1, false);
	structTree(184, 2, 1, false);
	structTree(187, 2, 1, true);

	structTree(379, 2, 1, true);
	structTree(357, 2, 1, true);
	structTree(349, 2, 1, true);
	structTree(347, 2, 1, false);
	structTree(331, 2, 1, true);
	structTree(328, 2, 1, false);
	structTree(309, 2, 1, true);
	structTree(311, 2, 1, false);
	structTree(312, 2, 1, false);
	structTree(299, 2, 1, false);
	structTree(301, 2, 1, true);
	structTree(283, 2, 1, true);
	structTree(261, 2, 1, true);
	structTree(263, 2, 1, false);
	structTree(264, 2, 1, false);
	structTree(253, 2, 1, true);
	structTree(251, 2, 1, false);
	structTree(232, 2, 1, false);
	structTree(235, 2, 1, true);
	structTree(213, 2, 1, true);
	structTree(215, 2, 1, false);
	structTree(216, 2, 1, false);
	structTree(203, 2, 1, false);
	structTree(205, 2, 1, true);

	structFence(185, 2, 1);
	structFence(182, 2, 2);
	structFence(158, 2, 4);
	structFence(134, 2, 2);
	structFence(137, 2, 1);
	structFence(110, 2, 4);
	structFence(86, 2, 2);
	structFence(89, 2, 1);
	structFence(62, 2, 4);
	structFence(38, 2, 2);
	structFence(41, 2, 1);
	structFence(14, 2, 4);

	structFence(206, 2, 4);
	structFence(230, 2, 2);
	structFence(233, 2, 1);
	structFence(254, 2, 4);
	structFence(281, 2, 1);
	structFence(302, 2, 4);
	structFence(326, 2, 2);
	structFence(329, 2, 1);
	structFence(350, 2, 4);
	structFence(374, 2, 4);

	structPipe(35, 2, 3);
	structPipe(76, 2, 3);
	structPipe(82, 2, 2);
	structPipe(94, 2, 3);
	structPipe(104, 2, 3);
	structPipe(115, 2, 1);
	structPipe(140, 2, 2);

	struckBlockQ2(80, 6, 1);
	lMap[80][6]->setSpawnMushroom(true);
	struckBlockQ2(158, 5, 1);

	structCoins(64, 6, 1, 1);
	structCoins(89, 6, 1, 1);
	structCoins(98, 6, 1, 1);
	structCoins(109, 10, 2, 1);

	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(184, 6, 8, 1);
	structBrick(154, 9, 8, 1);
	lMap[158][9]->setNumOfUse(8);
	structBrick(184, 6, 8, 1);
	lMap[186][6]->setSpawnStar(true);

	structGND2(153, 2, 1, 4);
	structGND2(163, 2, 1, 4);

	structGND2(303, 2, 1, 2);
	structGND2(307, 2, 1, 2);

	structGND2(210, 2, 1, 2);
	structGND2(360, 2, 1, 2);
	structGND2(362, 2, 1, 4);
	structGND2(364, 2, 1, 6);
	structGND2(366, 2, 2, 8);
	structGND2(275, 2, 6, true);

	structPipe(344, 2, 2);
	structPipe(355, 2, 1);
	structPipe(246, 2, 4);
	structPipe(242, 2, 3);
	structPipe(238, 2, 2);

	structCoins(223, 6, 2, 1);
	structCoins(283, 6, 2, 1);
	structCoins(291, 6, 1, 1);
	structCoins(296, 6, 1, 1);
	structCoins(316, 6, 2, 1);
	structCoins(321, 6, 2, 1);

	structGND2(376, 2, 1, 1);
	structEnd(376, 3, 9);

	this->iLevelType = 1;

	structGND(410, 0, 17, 2);

	structBrick(410, 2, 1, 11);
	structBrick(413, 5, 10, 1);
	structBrick(413, 9, 10, 4);
	structBrick(423, 4, 2, 9);

	lMap[422][5]->setNumOfUse(10);

	structCoins(413, 2, 9, 1);
	structCoins(414, 6, 8, 1);

	structPipeVertical(425, 2, 10);
	structPipeHorizontal(423, 2, 1);

	this->iLevelType = 0;
}

void Map::loadLVL_3_2() {
	clearMap();

	this->iMapWidth = 400;
	this->iMapHeight = 25;
	this->iLevelType = 0;
	this->iMapTime = 400;

	// ---------- LOAD LISTS ----------

	createMap();

	loadMinionsLVL_3_2();
	loadPipeEventsLVL_3_2();

	structBulletBill(85, 2, 0);
	structBulletBill(93, 3, 0);
	structBulletBill(93, 1, 0);
	structBulletBill(105, 2, 0);
	structBulletBill(115, 1, 0);
	structBulletBill(119, 6, 0);
	structBulletBill(125, 2, 1);
	structBulletBill(175, 2, 0);
	structBulletBill(191, 1, 0);
	structBulletBill(191, 3, 0);

	structGND(0, 0, 15, 2);
	structGND(16, 0, 5, 2);
	structGND(22, 0, 14, 2);
	structGND(37, 0, 8, 2);
	structGND(46, 0, 4, 2);
	structGND(51, 0, 1, 2);
	structGND(53, 0, 3, 2);
	structGND(57, 0, 6, 2);
	structGND(64, 0, 14, 2);
	structGND(80, 0, 4, 2);
	structGND(85, 0, 53, 2);
	structGND(139, 0, 5, 2);
	structGND(145, 0, 1, 2);
	structGND(147, 0, 1, 2);
	structGND(154, 0, 20, 2);
	structGND(175, 0, 1, 2);
	structGND(179, 0, 23, 2);
	structGND(203, 0, 1, 2);
	structGND(206, 0, 31, 2);

	structCloud(0, 10, 2);
	structCloud(18, 10, 1);
	structCloud(27, 11, 1);
	structCloud(30, 10, 2);
	structCloud(45, 11, 1);
	structCloud(48, 10, 2);
	structCloud(66, 10, 1);
	structCloud(75, 11, 1);
	structCloud(78, 10, 1);
	structCloud(93, 11, 1);
	structCloud(96, 10, 2);
	structCloud(114, 10, 1);
	structCloud(123, 11, 1);
	structCloud(126, 10, 2);
	structCloud(141, 11, 1);
	structCloud(144, 10, 2);
	structCloud(162, 10, 1);
	structCloud(171, 11, 1);
	structCloud(174, 10, 2);
	structCloud(189, 11, 1);
	structCloud(192, 10, 2);
	structCloud(210, 10, 1);
	structCloud(219, 11, 1);
	structCloud(222, 10, 2);

	structFence(14, 2, 1);
	structFence(16, 2, 1);
	structFence(38, 2, 4);
	structFence(62, 2, 1);
	structFence(64, 2, 2);
	structFence(86, 2, 4);
	structFence(110, 2, 4);
	structFence(134, 2, 4);
	structFence(158, 2, 4);
	structFence(208, 2, 2);
	structFence(230, 2, 2);

	structCastleSmall(0, 2);
	structCastleSmall(220, 2);

	structTree(232, 2, 1, false);
	structTree(215, 2, 1, false);
	structTree(213, 2, 1, true);
	structTree(187, 2, 1, true);
	structTree(165, 2, 1, true);
	structTree(167, 2, 1, false);
	structTree(168, 2, 1, false);
	structTree(155, 2, 1, false);
	structTree(136, 2, 1, false);
	structTree(139, 2, 1, true);
	structTree(117, 2, 1, true);
	structTree(119, 2, 1, false);
	structTree(120, 2, 1, false);
	structTree(107, 2, 1, false);
	structTree(109, 2, 1, true);
	structTree(88, 2, 1, false);
	structTree(91, 2, 1, true);
	structTree(69, 2, 1, true);
	structTree(71, 2, 1, false);
	structTree(72, 2, 1, false);
	structTree(59, 2, 1, false);
	structTree(61, 2, 1, true);
	structTree(40, 2, 1, false);
	structTree(43, 2, 1, true);
	structTree(11, 2, 1, false);
	structTree(13, 2, 1, true);

	structGND2(206, 2, 2, 8);
	structGND2(203, 2, 1, 5);
	structGND2(199, 2, 3, true);
	structGND2(182, 2, 5, true);

	structPipe(163, 2, 1);
	structPipe(156, 2, 3);
	structPipe(142, 2, 1);
	structPipe(131, 2, 1);

	structBrick(118, 5, 1, 1);
	structBrick(120, 5, 1, 1);
	lMap[120][5]->setNumOfUse(8);
	structGND2(119, 5, 1, 1);
	structBrick(110, 5, 2, 1);
	structBrick(99, 5, 2, 1);
	lMap[100][5]->setSpawnMushroom(true);

	structBrick(77, 5, 2, 1);
	structBrick(43, 9, 33, 1);
	lMap[44][9]->setSpawnMushroom(true);
	lMap[44][9]->setPowerUP(false);

	addSpring(44 * 32, 336);

	struckBlockQ(29, 5, 4);

	structGND2(17, 2, 4, true);
	structGND2(22, 2, 1, 6);
	structGND2(23, 2, 1, 7);
	structGND2(24, 2, 2, 8);

	structGND2(216, 2, 1, 1);
	structEnd(216, 3, 9);

	// -- MAP 8-2-2

	this->iLevelType = 1;

	structGND(240, 0, 17, 2);

	structBrick(239, 2, 1, 18);
	structBrick(240, 2, 1, 11);
	structBrick(244, 7, 1, 2);
	structBrick(245, 7, 5, 1);
	structBrick(250, 7, 1, 6);
	structBrick(251, 8, 2, 1);
	structBrick(244, 12, 6, 1);

	structBrick(254, 7, 1, 1);
	lMap[254][7]->setNumOfUse(8);

	structCoins(245, 8, 5, 2);

	structPipeVertical(255, 2, 10);
	structPipeHorizontal(253, 2, 1);

	this->iLevelType = 0;
}

void Map::loadLVL_3_3() {
	clearMap();

	this->iMapWidth = 240;
	this->iMapHeight = 25;
	this->iLevelType = 0;
	this->iMapTime = 300;

	// ---------- LOAD LISTS ----------

	createMap();

	loadMinionsLVL_3_3();

	structGND(0, 0, 69, 2);
	structGND(71, 0, 4, 2);
	structGND(77, 0, 47, 2);
	structGND(126, 0, 2, 2);
	structGND(130, 0, 67, 2);
	structGND(208, 0, 31, 2);

	structBulletBill(18, 2, 0);
	structBulletBill(34, 2, 1);
	structBulletBill(86, 2, 0);
	structBulletBill(86, 2, 0);
	structBulletBill(86, 2, 0);
	structBulletBill(86, 2, 0);
	structBulletBill(86, 2, 0);

	structCloud(0, 10, 2);
	structCloud(18, 10, 1);
	structCloud(27, 11, 1);
	structCloud(30, 10, 2);
	structCloud(45, 11, 1);
	structCloud(48, 10, 2);
	structCloud(66, 10, 1);
	structCloud(75, 11, 1);
	structCloud(78, 10, 2);
	structCloud(93, 11, 1);
	structCloud(96, 10, 2);
	structCloud(114, 10, 1);
	structCloud(123, 11, 1);
	structCloud(126, 10, 2);
	structCloud(141, 11, 1);
	structCloud(144, 10, 2);
	structCloud(162, 10, 1);
	structCloud(171, 11, 1);
	structCloud(174, 10, 2);
	structCloud(189, 11, 1);
	structCloud(192, 10, 2);
	structCloud(210, 10, 1);
	structCloud(219, 11, 1);

	structCastleSmall(0, 2);

	structTree(11, 2, 1, false);
	structTree(13, 2, 1, true);
	structTree(21, 2, 1, true);
	structTree(23, 2, 1, false);
	structTree(59, 2, 1, false);
	structTree(61, 2, 1, true);
	structTree(107, 2, 1, false);
	structTree(117, 2, 1, true);
	structTree(119, 2, 1, false);
	structTree(120, 2, 1, false);
	structTree(167, 2, 1, false);
	structTree(213, 2, 1, true);
	structTree(215, 2, 1, false);
	structTree(216, 2, 1, false);

	structGND2(195, 2, 1, 2);
	structGND2(198, 3, 1, 1);
	structGND2(200, 5, 1, 1);
	structGND2(202, 7, 1, 1);
	structGND2(204, 9, 2, 1);

	structGND2(95, 2, 1, 3);
	structGND2(109, 2, 2, 4);
	structGND2(71, 2, 4, false);

	structBrick(115, 5, 8, 1);
	structBrick(115, 9, 8, 1);
	lMap[116][9]->setSpawnMushroom(true);
	structBrick(60, 5, 8, 1);
	structBrick(60, 9, 8, 1);
	lMap[66][9]->setSpawnMushroom(true);

	structFence(208, 2, 2);
	structFence(87, 2, 1);
	structFence(62, 2, 4);
	structFence(14, 2, 4);
	structFence(111, 2, 3);

	structPipe(53, 2, 3);
	structPipe(126, 2, 3);
	structPipe(168, 2, 2);

	structCastleWall(24, 2, 8, 6);
	structCastleWall(37, 2, 14, 6);
	structCastleWall(79, 2, 6, 6);
	structCastleWall(88, 2, 6, 6);
	structCastleWall(97, 2, 10, 6);
	structCastleWall(132, 2, 34, 6);
	structCastleWall(172, 2, 20, 6);

	structBrick(190, 5, 1, 1);
	lMap[190][5]->setNumOfUse(10);

	structCastleBig(220, 2);
	structCastleWall(229, 2, 10, 6);

	structGND2(214, 2, 1, 1);
	structEnd(214, 3, 9);
}

void Map::loadLVL_3_4() {
	clearMap();

	this->iMapWidth = 480;
	this->iMapHeight = 25;
	this->iLevelType = 3;
	this->iMapTime = 400;

	// ---------- LOAD LISTS ----------

	createMap();

	loadMinionsLVL_3_4();
	loadPipeEventsLVL_3_4();

	structGND(0, 0, 369, 2);
	structGND(0, 12, 369, 1);

	structGND(0, 0, 6, 5);
	structGND(0, 5, 5, 1);
	structGND(0, 6, 4, 1);
	structGND(0, 7, 3, 1);

	structLava(6, 0, 5, 2);

	structGND(11, 0, 55, 2);

	structLava(66, 0, 9, 2);

	structGND(62, 2, 4, 3);

	structGND(75, 0, 21, 5);
	structGND(76, 8, 4, 1);

	structPipe(19, 0, 3);
	structPipe(51, 0, 3);
	structPipe(81, 0, 6);
	structPipe(90, 0, 7);
	structPipe(126, 0, 3);
	structPipe(133, 0, 4);
	structPipe(143, 0, 3);
	structPipe(153, 0, 4);
	structPipe(215, 0, 3);

	structLava(155, 0, 3, 2);

	struckBlockQ2(161, 5, 1);

	structGND(163, 5, 2, 1);
	structPipe(163, 6, 2);

	vPlatform.push_back(new Platform(4, 3, 68 * 32 - 16, 71 * 32 + 16, CFG::GameHeight - 16 - 32, CFG::GameHeight - 16 - 32, 68 * 32 - 16, (float)CFG::GameHeight - 16 - 30, true));

	for (int i = 0; i < 10; i++) {
		lMap[110][2 + i]->setBlockID(169);
	}

	for (int i = 0; i < 10; i++) {
		lMap[191][2 + i]->setBlockID(170);
	}

	structGND(218, 2, 22, 3);
	structGND(244, 2, 16, 3);

	structLava(240, 0, 4, 2);

	for (int i = 0; i < 10; i++) {
		lMap[274][2 + i]->setBlockID(171);
	}

	structPipe(302, 0, 3);

	structPipe(224, 0, 6);
	structPipe(232, 0, 7);
	structPipe(248, 0, 6);

	structPipe(309, 0, 3);

	structLava(320, 0, 5, 2);

	structGND(325, 2, 6, 3);
	structGND(325, 10, 6, 2);

	structBridge(331, 4, 13);
	structLava(331, 0, 13, 2);

	structGND(344, 2, 3, 4);
	structGND(345, 9, 2, 3);

	this->iLevelType = 2;

	structWater(392, 2, 67, 11);
	structPipeHorizontal(458, 6, 1);

	this->iLevelType = 7;

	structGND(385, 0, 7, 13);
	structGND(392, 0, 79, 2);
	structGND(396, 2, 12, 3);
	structGND(401, 5, 7, 1);

	structGND(396, 10, 12, 3);
	structGND(401, 9, 7, 1);

	structGND(408, 12, 63, 1);

	structGND(429, 2, 3, 3);
	structGND(429, 9, 3, 3);

	structGND(457, 2, 14, 4);
	structGND(459, 6, 12, 3);
	structGND(457, 9, 14, 3);

	structPipe(393, 0, 3);

	this->iLevelType = 3;
}

void Map::loadMinionsLVL_1_1() {
	clearMinions();

	addGoombas(704, 480, true);

	addGoombas(1280, 480, true);

	addGoombas(1632, 480, true);
	addGoombas(1680, 480, true);

	addGoombas(2560, 112, true);
	addGoombas(2624, 112, true);

	addGoombas(3104, 480, true);
	addGoombas(3152, 480, true);

	addKoppa(107 * 32, 480, 1, true);

	addGoombas(3648, 480, true);
	addGoombas(3696, 480, true);

	addGoombas(3968, 480, true);
	addGoombas(4016, 480, true);

	addGoombas(4096, 480, true);	addGoombas(4144, 480, true);

	addGoombas(5568, 480, true);
	addGoombas(5612, 480, true);
}

void Map::loadMinionsLVL_1_2() {
	clearMinions();

	this->iLevelType = 1;

	addGoombas(16 * 32, 368, true);
	addGoombas(17 * 32 + 8, 368 - 32, true);

	addGoombas(29 * 32, 368, true);

	addKoppa(44 * 32, 400, 1, true);
	addKoppa(45 * 32 + 16, 400, 1, true);

	addKoppa(59 * 32, 400, 1, true);

	addGoombas(62 * 32, 368, true);
	addGoombas(64 * 32, 368, true);

	addGoombas(73 * 32, 368 - 8 * 32, true);

	addGoombas(76 * 32, 368 - 4 * 32, true);
	addGoombas(77 * 32 + 16, 368 - 4 * 32, true);

	addGoombas(99 * 32, 368, true);
	addGoombas(100 * 32 + 16, 368, true);
	addGoombas(102 * 32, 368, true);

	addGoombas(113 * 32, 368, true);

	addGoombas(135 * 32, 368 - 4 * 32, true);
	addGoombas(136 * 32 + 16, 368 - 4 * 32, true);

	this->iLevelType = 3;

	addKoppa(146 * 32, 400, 1, false);

	this->iLevelType = 1;
	addPlant(103 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);
	addPlant(109 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(115 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);

	this->iLevelType = 0;
	addPlant(284 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);

	this->iLevelType = 1;
}

void Map::loadMinionsLVL_1_3() {
	clearMinions();

	this->iLevelType = 3;

	addKoppa(30 * 32 - 8, CFG::GameHeight - 16 - 10 * 32, 1, true);
	addKoppa(110 * 32 - 8, CFG::GameHeight - 16 - 8 * 32, 1, true);

	addKoppa(74 * 32 - 8, CFG::GameHeight - 16 - 10 * 32, 3, false);
	addKoppa(114 * 32 - 8, CFG::GameHeight - 16 - 9 * 32, 3, false);

	addKoppa(133 * 32 - 8, CFG::GameHeight - 16 - 2 * 32, 1, true);
	this->iLevelType = 0;

	addGoombas(44 * 32, CFG::GameHeight - 16 - 11 * 32, true);
	addGoombas(46 * 32, CFG::GameHeight - 16 - 11 * 32, true);
	addGoombas(80 * 32, CFG::GameHeight - 16 - 9 * 32, true);
}

void Map::loadMinionsLVL_1_4() {
	clearMinions();

	addBowser(135 * 32, CFG::GameHeight - 16 - 6 * 32);

	addToad(153 * 32, CFG::GameHeight - 3 * 32, false);

	addFireBall(30 * 32, CFG::GameHeight - 16 - 4 * 32, 6, rand() % 360, true);
	addFireBall(49 * 32, CFG::GameHeight - 16 - 8 * 32, 6, rand() % 360, true);
	addFireBall(60 * 32, CFG::GameHeight - 16 - 8 * 32, 6, rand() % 360, true);
	addFireBall(67 * 32, CFG::GameHeight - 16 - 8 * 32, 6, rand() % 360, true);
	addFireBall(76 * 32, CFG::GameHeight - 16 - 5 * 32, 6, rand() % 360, true);
	addFireBall(84 * 32, CFG::GameHeight - 16 - 5 * 32, 6, rand() % 360, true);
	addFireBall(88 * 32, CFG::GameHeight - 16 - 10 * 32, 6, rand() % 360, false);
}

void Map::loadMinionsLVL_2_1() {
	clearMinions();

	addSpring(188 * 32, 336);

	addGoombas(24 * 32, CFG::GameHeight - 16 - 7 * 32, true);

	addGoombas(42 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(43 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);

	addGoombas(59 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(60 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);

	addGoombas(68 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(69 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(71 * 32, CFG::GameHeight - 16 - 2 * 32, true);

	addGoombas(87 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(88 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(90 * 32, CFG::GameHeight - 16 - 2 * 32, true);

	addGoombas(102 * 32 + 16, CFG::GameHeight - 16 - 6 * 32, true);
	addGoombas(114 * 32 + 16, CFG::GameHeight - 16 - 4 * 32, true);

	addGoombas(120 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);

	addGoombas(162 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(163 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);

	addKoppa(32 * 32 - 2, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(33 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);

	addKoppa(55 * 32, CFG::GameHeight - 16 - 6 * 32, 1, true);
	addKoppa(66 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);

	addKoppa(137 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(151 * 32, CFG::GameHeight - 16 - 4 * 32, 0, true);
	addKoppa(169 * 32, CFG::GameHeight - 16 - 2 * 32, 0, true);
	addKoppa(171 * 32, CFG::GameHeight - 16 - 2 * 32, 0, true);

	addKoppa(185 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);

	addPlant(46 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(74 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(103 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(115 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(122 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(130 * 32 + 16, CFG::GameHeight - 10 - 6 * 32);
	addPlant(176 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);
}

void Map::loadMinionsLVL_2_2() {
	clearMinions();

	addSquid(22 * 32, CFG::GameHeight - 16 - 3 * 32);
	addSquid(46 * 32, CFG::GameHeight - 16 - 5 * 32);
	addSquid(55 * 32, CFG::GameHeight - 16 - 4 * 32);
	addSquid(83 * 32, CFG::GameHeight - 16 - 6 * 32);
	addSquid(94 * 32, CFG::GameHeight - 16 - 11 * 32);
	addSquid(105 * 32, CFG::GameHeight - 16 - 3 * 32);

	addCheep(75 * 32 + 28, CFG::GameHeight - 16 - 4 * 32, 0, 1);
	addCheep(78 * 32 + 28, CFG::GameHeight - 16 - 7 * 32, 0, 1);
	addCheep(81 * 32 + 28, CFG::GameHeight - 16 - 2 * 32 - 28, 0, 1);
	addCheep(94 * 32 + 14, CFG::GameHeight - 16 - 8 * 32, 0, 1);
	addCheep(101 * 32 + 28, CFG::GameHeight - 16 - 4 * 32, 0, 1);
	addCheep(97 * 32 + 8, CFG::GameHeight - 16 - 11 * 32, 1, 1);
	addCheep(117 * 32 + 8, CFG::GameHeight - 16 - 10 * 32, 0, 1);
	addCheep(127 * 32 + 24, CFG::GameHeight - 16 - 4 * 32, 1, 1);
	addCheep(131 * 32 + 8, CFG::GameHeight - 16 - 3 * 32 - 4, 0, 1);
	addCheep(136 * 32 + 16, CFG::GameHeight - 16 - 6 * 32, 0, 1);
	addCheep(145 * 32 + 8, CFG::GameHeight - 16 - 4 * 32, 0, 1);
	addCheep(149 * 32 + 28, CFG::GameHeight - 16 - 8 * 32 - 4, 1, 1);
	addCheep(164 * 32, CFG::GameHeight - 16 - 11 * 32, 0, 1);
	addCheep(167 * 32, CFG::GameHeight - 16 - 3 * 32, 1, 1);
	addCheep(175 * 32, CFG::GameHeight - 16 - 6 * 32 - 4, 0, 1);
	addCheep(183 * 32, CFG::GameHeight - 16 - 10 * 32, 1, 1);
	addCheep(186 * 32 + 16, CFG::GameHeight - 16 - 7 * 32, 1, 1);

	this->iLevelType = 0;
	addPlant(274 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);

	this->iLevelType = 2;
}

void Map::loadMinionsLVL_2_3() {
	clearMinions();

	addCheepSpawner(5 * 32, 200 * 32);
}

void Map::loadMinionsLVL_2_4() {
	clearMinions();

	addBowser(135 * 32, CFG::GameHeight - 16 - 6 * 32);

	addToad(153 * 32, CFG::GameHeight - 3 * 32, false);

	addFireBall(49 * 32, CFG::GameHeight - 16 - 5 * 32, 6, rand() % 360, true);
	addFireBall(55 * 32, CFG::GameHeight - 16 - 9 * 32, 6, rand() % 360, true);
	addFireBall(61 * 32, CFG::GameHeight - 16 - 5 * 32, 6, rand() % 360, true);
	addFireBall(73 * 32, CFG::GameHeight - 16 - 5 * 32, 6, rand() % 360, true);
	addFireBall(82 * 32, CFG::GameHeight - 16 - 8 * 32, 6, rand() % 360, true);
	addFireBall(92 * 32, CFG::GameHeight - 16 - 4 * 32, 6, rand() % 360, false);

	addUpFire(16 * 32 + 4, 9 * 32);
	addUpFire(30 * 32, 9 * 32);
}

void Map::loadMinionsLVL_3_1() {
	clearMinions();

	addGoombas(23 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(24 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(26 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(30 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(31 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(33 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(69 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(70 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(72 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(108 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(109 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(111 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(148 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(149 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(151 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(232 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(233 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(235 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(257 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(258 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(260 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(264 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(265 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(267 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(272 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(273 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);

	addKoppa(43 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(44 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(61 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(119 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(124 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(125 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(127 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(130 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(131 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(133 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(161 * 32, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(172 * 32, CFG::GameHeight - 16 - 4 * 32, 0, true);
	addKoppa(177 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(207 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(208 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(305 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(332 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(339 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(340 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, 1, true);
	addKoppa(342 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);


	addBeetle(18 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addBeetle(81 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addBeetle(254 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addBeetle(283 * 32, CFG::GameHeight - 16 - 2 * 32, true);

	addPlant(35 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(76 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(82 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);
	addPlant(94 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(104 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(140 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);
	addPlant(238 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);
	addPlant(242 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(344 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);
	addPlant(355 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
}

void Map::loadMinionsLVL_3_2() {
	clearMinions();

	addGoombas(184 * 32, CFG::GameHeight - 16 - 5 * 32, true);
	addGoombas(186 * 32, CFG::GameHeight - 16 - 7 * 32, true);

	addKoppa(18 * 32 - 8, CFG::GameHeight - 16 - 5 * 32, 0, true);
	addKoppa(24 * 32, CFG::GameHeight - 16 - 10 * 32, 0, true);
	addKoppa(57 * 32, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(66 * 32, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(69 * 32, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(92 * 32, CFG::GameHeight - 16 - 4 * 32, 0, true);
	addKoppa(95 * 32, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(139 * 32, CFG::GameHeight - 16 - 2 * 32, 0, true);
	addKoppa(170 * 32, CFG::GameHeight - 16 - 4 * 32, 0, true);
	addKoppa(172 * 32, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(175 * 32, CFG::GameHeight - 16 - 6 * 32, 0, true);
	addKoppa(203 * 32, CFG::GameHeight - 16 - 8 * 32, 0, true);

	addBeetle(111 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addBeetle(121 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addBeetle(123 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addBeetle(189 * 32, CFG::GameHeight - 16 - 2 * 32, true);

	addLakito(16 * 32, CFG::GameHeight - 16 - 11 * 32, 216 * 32);

	addPlant(131 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(142 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(156 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(163 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(131 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
}

void Map::loadMinionsLVL_3_3() {
	clearMinions();

	addKoppa(30 * 32, CFG::GameHeight - 16 - 4 * 32, 0, true);
	addKoppa(93 * 32, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(137 * 32, CFG::GameHeight - 16 - 2 * 32, 1, true);

	addPlant(53 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(126 * 32 + 16, CFG::GameHeight - 10 - 5 * 32);
	addPlant(168 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);

	addHammerBro(63 * 32, CFG::GameHeight - 16 - 3 * 32);
	addHammerBro(65 * 32, CFG::GameHeight - 16 - 7 * 32);
	addHammerBro(117 * 32, CFG::GameHeight - 16 - 7 * 32);
	addHammerBro(119 * 32, CFG::GameHeight - 16 - 3 * 32);
	addHammerBro(146 * 32, CFG::GameHeight - 16 - 3 * 32);
	addHammerBro(159 * 32, CFG::GameHeight - 16 - 3 * 32);
	addHammerBro(177 * 32, CFG::GameHeight - 16 - 3 * 32);
	addHammerBro(185 * 32, CFG::GameHeight - 16 - 3 * 32);
}

void Map::loadMinionsLVL_3_4() {
	clearMinions();

	addPlant(19 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(51 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(81 * 32 + 16, CFG::GameHeight - 10 - 6 * 32);
	addPlant(126 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(133 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);
	addPlant(143 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(153 * 32 + 16, CFG::GameHeight - 10 - 4 * 32);
	addPlant(163 * 32 + 16, CFG::GameHeight - 10 - 8 * 32);
	addPlant(215 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(302 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);
	addPlant(224 * 32 + 16, CFG::GameHeight - 10 - 6 * 32);
	addPlant(232 * 32 + 16, CFG::GameHeight - 10 - 7 * 32);
	addPlant(248 * 32 + 16, CFG::GameHeight - 10 - 6 * 32);
	addPlant(309 * 32 + 16, CFG::GameHeight - 10 - 3 * 32);

	addBeetle(139 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addBeetle(141 * 32, CFG::GameHeight - 16 - 2 * 32, true);

	addGoombas(56 * 32, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(57 * 32 + 16, CFG::GameHeight - 16 - 2 * 32, true);
	addGoombas(59 * 32, CFG::GameHeight - 16 - 2 * 32, true);

	addHammerBro(316 * 32, CFG::GameHeight - 16 - 3 * 32);

	this->iLevelType = 1;

	addKoppa(150 * 32 - 8, CFG::GameHeight - 16 - 4 * 32, 0, true);
	addKoppa(152 * 32 - 8, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(165 * 32 - 8, CFG::GameHeight - 16 - 3 * 32, 0, true);
	addKoppa(167 * 32 - 8, CFG::GameHeight - 16 - 4 * 32, 0, true);

	this->iLevelType = 3;

	addCheepSpawner(224 * 32, 237 * 32);

	addBowser(338 * 32, CFG::GameHeight - 16 - 6 * 32, true);

	addToad(356 * 32, CFG::GameHeight - 3 * 32, true);

	addUpFire(332 * 32, 9 * 32);

	addFireBall(410 * 32, CFG::GameHeight - 16 - 7 * 32, 6, rand() % 360, true);
	addFireBall(421 * 32, CFG::GameHeight - 16 - 4 * 32, 6, rand() % 360, false);
	addFireBall(430 * 32, CFG::GameHeight - 16 - 8 * 32, 6, rand() % 360, true);
	addFireBall(446 * 32, CFG::GameHeight - 16 - 6 * 32, 6, rand() % 360, true);
	addFireBall(454 * 32, CFG::GameHeight - 16 - 7 * 32, 6, rand() % 360, false);

	addSquid(418 * 32, CFG::GameHeight - 16 - 3 * 32);
	addSquid(441 * 32, CFG::GameHeight - 16 - 4 * 32);
	addSquid(443 * 32, CFG::GameHeight - 16 - 8 * 32);
}

void Map::clearPipeEvents() {
	for (unsigned int i = 0; i < lPipe.size(); i++) {
		delete lPipe[i];
	}

	lPipe.clear();
}

void Map::loadPipeEventsLVL_1_1() {
	lPipe.push_back(new Pipe(0, 57, 5, 58, 5, 242 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 253, 3, 253, 2, 163 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_1_2() {
	lPipe.push_back(new Pipe(0, 103, 4, 104, 4, 242 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 253, 3, 253, 2, 115 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 1, true, 350, 1, false));

	lPipe.push_back(new Pipe(1, 166, 6, 166, 5, 284 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));

	// -- WARP ZONE 2
	lPipe.push_back(new Pipe(0, 186, 4, 187, 4, 84, CFG::GameHeight - 80, 4, 0, true, 350, 1, false));
	// -- WARP ZONE 3
	lPipe.push_back(new Pipe(0, 182, 4, 183, 4, 84, CFG::GameHeight - 80, 8, 4, true, 350, 1, false));
	// -- WARP ZONE 4
	lPipe.push_back(new Pipe(0, 178, 4, 179, 4, 84, CFG::GameHeight - 80, 12, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_2_1() {
	lPipe.push_back(new Pipe(0, 103, 5, 104, 5, 232 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 243, 3, 243, 2, 115 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_2_2() {
	lPipe.push_back(new Pipe(1, 189, 7, 189, 6, 274 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_3_1() {
	lPipe.push_back(new Pipe(0, 104, 5, 105, 5, 412 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 423, 3, 423, 2, 115 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_3_2() {
	lPipe.push_back(new Pipe(0, 156, 5, 157, 5, 242 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 253, 3, 253, 2, 163 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_3_4() {
	lPipe.push_back(new Pipe(2, 81, 6, 82, 6, 126 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 3, true, 250, 1, false));

	lPipe.push_back(new Pipe(2, 163, 8, 164, 8, 215 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 3, true, 250, 1, false));

	lPipe.push_back(new Pipe(2, 248, 6, 249, 6, 393 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 2, true, 250, 1, true));
	lPipe.push_back(new Pipe(1, 458, 7, 458, 6, 302 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 3, true, 350, 1, false));

	lPipe.push_back(new Pipe(2, 51, 4, 52, 4, 19 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 3, true, 250, 1, false));
	lPipe.push_back(new Pipe(2, 143, 3, 144, 3, 19 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 3, true, 250, 1, false));
	lPipe.push_back(new Pipe(2, 232, 7, 232, 7, 19 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 3, true, 250, 1, false));
	lPipe.push_back(new Pipe(2, 309, 3, 310, 3, 19 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 3, true, 250, 1, false));
}

void Map::createMap()
{
	//	----- MIONION LIST -----
	for (int i = 0; i < iMapWidth; i += 5) {
		std::vector<Minion*> temp;
		lMinion.push_back(temp);
	}

	iMinionListSize = lMinion.size();

	// ----- CREATE MAP -----
	for (int i = 0; i < iMapWidth; i++) {
		std::vector<Tile*> temp;
		for (int i = 0; i < iMapHeight; i++) {
			temp.push_back(new Tile(0));
		}

		lMap.push_back(temp);
	}

	this->underWater = false;
	//this->bTP = false;
}

int Map::getStartBlock()
{
	return (int)(-fXPos - (-(int)fXPos) % 32) / 32;
}

int Map::getEndBlock()
{
	return (int)(-fXPos - (-(int)fXPos) % 32 + CFG::GameWidth) / 32 + 2;
}

int Map::getListID(int nX) {
	return (int)(nX / 160);
}

int Map::getNumOfMinions()
{
	int iOutput = 0;

	for (int i = 0, size = lMinion.size(); i < size; i++) {
		iOutput += lMinion[i].size();
	}

	return iOutput;
}

void Map::setBackgroundColor(sf::RenderWindow& mainWindow) {
	sf::Color backgroundColor;

	switch (iLevelType) {
	case 0: case 2:
		backgroundColor = sf::Color(93, 148, 252); // Light blue
		break;
	case 1: case 3: case 4:
		backgroundColor = sf::Color(0, 0, 0); // Black
		break;
	default:
		backgroundColor = sf::Color(93, 148, 252); // Default light blue
		break;
	}

	mainWindow.clear(backgroundColor);
}

bool Map::getInEvent() {
	return inEvent;
}

void Map::setInEvent(bool inEvent) {
	this->inEvent = inEvent;
}

void Map::setSpawnPointID(int iSpawnPointID) {
	this->iSpawnPointID = iSpawnPointID;
}

void Map::clearMap()
{
	for (int i = 0; i < iMapWidth; i++) {
		for (int j = 0; j < iMapHeight; j++) {
			delete lMap[i][j];
		}
		lMap[i].clear();
	}
	lMap.clear();

	this->iMapWidth = this->iMapHeight = 0;

	if (pFlag != NULL) {
		delete pFlag;
		pFlag = NULL;
	}

	pEvent->eventTypeID = pEvent->eNormal;

	clearLevelText();
}

void Map::clearLevelText() {
	for (unsigned int i = 0; i < vLevelText.size(); i++) {
		delete vLevelText[i];
	}

	vLevelText.clear();
}

void Map::pipeUse() {
	for (unsigned int i = 0; i < lPipe.size(); i++) {
		lPipe[i]->checkUse();
	}
}

void Map::EndUse() {
	inEvent = true;

	pEvent->resetData();
	pPlayer->resetJump();
	pPlayer->stopMove();

	pEvent->newUnderWater = false;

	CFG::getMusic()->StopMusic();
	CFG::getMusic()->PlayChunk(CFG::getMusic()->cLEVELEND);

	pEvent->eventTypeID = pEvent->eEnd;

	if (pPlayer->getYPos() < CFG::GameHeight - 16 - 10 * 32) { 
		pFlag->iPoints = 5000;
	}
	else if (pPlayer->getYPos() < CFG::GameHeight - 16 - 8 * 32) {
		pFlag->iPoints = 2000;
	}
	else if (pPlayer->getYPos() < CFG::GameHeight - 16 - 6 * 32) {
		pFlag->iPoints = 500;
	}
	else if (pPlayer->getYPos() < CFG::GameHeight - 16 - 4 * 32) {
		pFlag->iPoints = 200;
	}
	else {
		pFlag->iPoints = 100;
	}

	pEvent->vOLDDir.push_back(pEvent->eRIGHTEND);
	pEvent->vOLDLength.push_back(pPlayer->getHitBoxX());

	pEvent->vOLDDir.push_back(pEvent->eENDBOT1);
	pEvent->vOLDLength.push_back((CFG::GameHeight - 16 - 32 * 2) - pPlayer->getYPos() - pPlayer->getHitBoxY() - 2);

	pEvent->vOLDDir.push_back(pEvent->eENDBOT2);
	pEvent->vOLDLength.push_back((CFG::GameHeight - 16 - 32 * 2) - pPlayer->getYPos() - pPlayer->getHitBoxY() - 2);

	pEvent->vOLDDir.push_back(pEvent->eRIGHTEND);
	pEvent->vOLDLength.push_back(pPlayer->getHitBoxX());

	pEvent->vOLDDir.push_back(pEvent->eBOTRIGHTEND);
	pEvent->vOLDLength.push_back(32);

	pEvent->vOLDDir.push_back(pEvent->eRIGHTEND);
	pEvent->vOLDLength.push_back(132);

	pEvent->iSpeed = 3;

	//std::cout << "currentLevelID" << currentLevelID << std::endl;

	switch (currentLevelID) {
	case 0: { 
		pEvent->newLevelType = 100;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = 1;
		pEvent->inEvent = true;

		pEvent->newMapXPos = -210 * 32;
		pEvent->newPlayerXPos = 64;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = false;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(204);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(204);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(205);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(205);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 1: {
		pEvent->newLevelType = 0;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = 2;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(309);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(309);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(310);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(310);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 2: {
		pEvent->newLevelType = 3;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = 3;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = 150;
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pEvent->reDrawX.push_back(159);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(159);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(160);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(160);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 4: {
		pEvent->newLevelType = 100;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = 5;
		pEvent->inEvent = true;

		pEvent->newMapXPos = -220 * 32;
		pEvent->newPlayerXPos = 64;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY() + 50;
		pEvent->newMoveMap = false;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(206);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(206);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(207);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(207);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 5: {
		pEvent->newLevelType = 0;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = 6;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pEvent->reDrawX.push_back(299);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(299);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(300);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(300);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 6:
		pEvent->newLevelType = 3;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = 150;
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pEvent->reDrawX.push_back(232);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(232);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(234);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(234);
		pEvent->reDrawY.push_back(3);
		break;
	case 8:
		pEvent->newLevelType = 4;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(206);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(206);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(207);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(207);
		pEvent->reDrawY.push_back(3);
		break;
	case 9:
		pEvent->newLevelType = 4;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(215);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(215);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(216);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(216);
		pEvent->reDrawY.push_back(3);
		break;
	case 10:
		pEvent->newLevelType = 3;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = 150;
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(158);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(158);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(159);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(159);
		pEvent->reDrawY.push_back(3);
		break;
	case 12: {
		pEvent->newLevelType = 100;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = true;

		pEvent->newMapXPos = -240 * 32;
		pEvent->newPlayerXPos = 64;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = false;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(231);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(231);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(232);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(232);
		pEvent->reDrawY.push_back(3);
		break;
	case 13:
		pEvent->newLevelType = 0;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(419);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(419);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(420);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(420);
		pEvent->reDrawY.push_back(3);
		break;
	case 14:
		pEvent->newLevelType = 3;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = 150;
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(154);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(154);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(155);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(155);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 16: {
		pEvent->newLevelType = 0;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = -80 * 32;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(205);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(205);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(206);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(206);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 17: {
		pEvent->newLevelType = 0;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(286);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(286);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(287);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(287);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 18: {
		pEvent->newLevelType = 3;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = 150;
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(159);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(159);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(160);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(160);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 20: {
		pEvent->newLevelType = 4;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = -85 * 32;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(96);

		pFlag->castleFlagExtraXPos = 96;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(194);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(194);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(195);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(195);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 21: {
		pEvent->newLevelType = 4;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(307);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(307);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(308);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(308);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 22: {
		pEvent->newLevelType = 3;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = 150;
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(174);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(174);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(175);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(175);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 24: {
		pEvent->newLevelType = 100;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = true;

		pEvent->newMapXPos = -220 * 32;
		pEvent->newPlayerXPos = 64;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = false;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(185);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(185);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(186);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(186);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 25: {
		pEvent->newLevelType = 0;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pEvent->reDrawX.push_back(299);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(299);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(300);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(300);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 26:
		pEvent->newLevelType = 3;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = 150;
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		pEvent->reDrawX.push_back(232);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(232);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(233);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(233);
		pEvent->reDrawY.push_back(3);
		break;
	case 28: {
		pEvent->newLevelType = 0;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(382);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(382);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(383);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(383);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 29: {
		pEvent->newLevelType = 0;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->reDrawX.push_back(222);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(222);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(223);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(223);
		pEvent->reDrawY.push_back(3);
		break;
	}
	case 30: {
		pEvent->newLevelType = 3;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = currentLevelID + 1;
		pEvent->inEvent = false;

		pEvent->newMapXPos = 0;
		pEvent->newPlayerXPos = 84;
		pEvent->newPlayerYPos = 150;
		pEvent->newMoveMap = true;

		pPlayer->setMoveDirection(true);

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(128);

		pFlag->castleFlagExtraXPos = 128;

		pEvent->reDrawX.push_back(224);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(224);
		pEvent->reDrawY.push_back(3);
		pEvent->reDrawX.push_back(225);
		pEvent->reDrawY.push_back(2);
		pEvent->reDrawX.push_back(225);
		pEvent->reDrawY.push_back(3);
		break;
	}
	}

	pEvent->vOLDDir.push_back(pEvent->eENDPOINTS);
	pEvent->vOLDLength.push_back(iMapTime);

	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(128);
}

void Map::EndBoss() {


	inEvent = true;

	pEvent->resetData();
	pPlayer->resetJump();
	pPlayer->stopMove();

	pEvent->endGame = false;

	switch (currentLevelID) {
	case 11:
		pEvent->endGame = true;
		break;
	default:
		CFG::getMusic()->StopMusic();
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cCASTLEEND);
		break;
	}

	pEvent->eventTypeID = pEvent->eBossEnd;

	pEvent->iSpeed = 3;

	pEvent->newLevelType = 0;
	pEvent->newCurrentLevel = currentLevelID + 1;
	pEvent->newMoveMap = true;
	pEvent->iDelay = 500;
	pEvent->inEvent = false;

	pEvent->newMapXPos = 0;
	pEvent->newPlayerXPos = 64;
	pEvent->newPlayerYPos = CFG::GameHeight - 48 - pPlayer->getHitBoxY();
	pEvent->newMoveMap = true;

	switch (currentLevelID) {
	case 7:
		pEvent->newLevelType = 4;
		break;
	case 19:
		pEvent->newLevelType = 4;
		break;
	}

	bool addOne = false;

	if (lMap[getBlockIDX((int)(pPlayer->getXPos() + pPlayer->getHitBoxX() / 2 - fXPos))][6]->getBlockID() == 82) {
		pEvent->vOLDDir.push_back(pEvent->eBOT);
		pEvent->vOLDLength.push_back(CFG::GameHeight - 16 - 5 * 32 - (pPlayer->getYPos() + pPlayer->getHitBoxY()));
	}
	else {
		pEvent->vOLDDir.push_back(pEvent->eBOTRIGHTEND);
		pEvent->vOLDLength.push_back(CFG::GameHeight - 16 - 5 * 32 - (pPlayer->getYPos() + pPlayer->getHitBoxY()));

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32 - CFG::GameHeight - 16 - 5 * 32 - (pPlayer->getYPos() + pPlayer->getHitBoxY()));
		addOne = true;
	}

	pEvent->vOLDDir.push_back(pEvent->eBOSSEND1);
	pEvent->vOLDLength.push_back(1);
	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(10);

	pEvent->vOLDDir.push_back(pEvent->eBOSSEND2);
	pEvent->vOLDLength.push_back(1);
	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(3);

	for (int i = 0; i < 6; i++) {
		pEvent->vOLDDir.push_back(pEvent->eBOSSEND3);
		pEvent->vOLDLength.push_back(2 + i);
		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(3);
	}

	pEvent->vOLDDir.push_back(pEvent->eBOSSEND4);
	pEvent->vOLDLength.push_back(1);

	for (int i = 6; i < 12; i++) {
		pEvent->vOLDDir.push_back(pEvent->eBOSSEND3);
		pEvent->vOLDLength.push_back(2 + i);
		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(3);
	}

	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(90);

	if (currentLevelID == 11) {
		CFG::getMusic()->StopMusic();
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cPRINCESSMUSIC);
	}

	pEvent->vOLDDir.push_back(pEvent->eBOTRIGHTBOSS);
	pEvent->vOLDLength.push_back(8 * 32);

	switch (currentLevelID) {
	case 11:
		pEvent->vOLDDir.push_back(pEvent->eENDGAMEBOSSTEXT1);
		pEvent->vOLDLength.push_back(getBlockIDX((int)(pPlayer->getXPos() + pPlayer->getHitBoxX() / 2 - fXPos)) * 32 + 7 * 32 + (addOne ? 32 : 0));
		break;
	default:
		pEvent->vOLDDir.push_back(pEvent->eBOSSTEXT1);
		pEvent->vOLDLength.push_back(getBlockIDX((int)(pPlayer->getXPos() + pPlayer->getHitBoxX() / 2 - fXPos)) * 32 + 7 * 32 + (addOne ? 32 : 0));
		break;
	}

	pEvent->vOLDDir.push_back(pEvent->eRIGHT);
	pEvent->vOLDLength.push_back(2 * 32 + 16);

	pEvent->vOLDDir.push_back(pEvent->eMARIOSPRITE1);
	pEvent->vOLDLength.push_back(1);

	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(90);

	switch (currentLevelID) {
	case 11:
		pEvent->vOLDDir.push_back(pEvent->eENDGAMEBOSSTEXT2);
		pEvent->vOLDLength.push_back(getBlockIDX((int)(pPlayer->getXPos() + pPlayer->getHitBoxX() / 2 - fXPos)) * 32 + 5 * 32 + (addOne ? 32 : 0) + 28);
		break;
	default:
		pEvent->vOLDDir.push_back(pEvent->eBOSSTEXT2);
		pEvent->vOLDLength.push_back(getBlockIDX((int)(pPlayer->getXPos() + pPlayer->getHitBoxX() / 2 - fXPos)) * 32 + 5 * 32 + (addOne ? 32 : 0));
		break;
	}

	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(300 + (currentLevelID == 11 ? 100 : 0));

	switch (currentLevelID) {
	case 11:
		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(90);
		break;
	}
	
}

void Map::EndBonus() {
		
	inEvent = true;

	pEvent->resetData();
	pPlayer->resetJump();
	pPlayer->stopMove();

	pEvent->eventTypeID = pEvent->eNormal;

	pEvent->iSpeed = 3;

	pEvent->newLevelType = iLevelType;
	pEvent->newCurrentLevel = currentLevelID;
	pEvent->newMoveMap = true;
	pEvent->iDelay = 0;
	pEvent->inEvent = false;

	pEvent->newMoveMap = true;

	switch (currentLevelID) {
	case 4: {
		pEvent->newMapXPos = -158 * 32 + 16;
		pEvent->newPlayerXPos = 128;
		pEvent->newPlayerYPos = -pPlayer->getHitBoxY();

		break;
	}
	case 8: {
		pEvent->newMapXPos = -158 * 32 + 16;
		pEvent->newPlayerXPos = 128;
		pEvent->newPlayerYPos = -pPlayer->getHitBoxY();
		break;
	}
	case 17: {
		pEvent->newMapXPos = -207 * 32 + 16;
		pEvent->newPlayerXPos = 128;
		pEvent->newPlayerYPos = -pPlayer->getHitBoxY();
		break;
	}
	case 21: {
		pEvent->newMapXPos = -243 * 32 + 16;
		pEvent->newPlayerXPos = 128;
		pEvent->newPlayerYPos = -pPlayer->getHitBoxY();
		break;
	}
	}
	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(1);
	
}

void Map::clearMinions()
{
	for (int i = 0; i < iMinionListSize; i++) {
		for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++) {
			if (dynamic_cast<PlayerFireBall*>(lMinion[i][j]) != lMinion[i][j]) 
				delete lMinion[i][j];
			jSize = lMinion[i].size();
		}
		lMinion[i].clear();
	}

	clearPlatforms();
	return;
}

void Map::clearPlatforms() {
	for (unsigned int i = 0; i < vPlatform.size(); i++) {
		delete vPlatform[i];
	}
	vPlatform.clear();
}

void Map::clearBubbles() {
	for (unsigned int i = 0; i < lBubble.size(); i++) {
		delete lBubble[i];
	}

	lBubble.clear();
}

void Map::checkSpawnPoint() {
	if (getNumOfSpawnPoints() > 1) {
		for (int i = iSpawnPointID + 1; i < getNumOfSpawnPoints(); i++) {
			if (getSpawnPointXPos(i) > pPlayer->getXPos() - fXPos && getSpawnPointXPos(i) < pPlayer->getXPos() - fXPos + 128) {
				iSpawnPointID = i;
				//pPlayer->getCoins() - fcloseall() = fXPos + pPlayer;
			}
		}
	}
}

int Map::getNumOfSpawnPoints() {
	switch (currentLevelID) {
	case 0: case 1: case 2: case 4: case 5: case 8: case 9: case 10: case 13: case 14: case 16: case 17: case 18: case 20: case 21: case 22: case 24: case 25: case 26:
		return 2;
	}

	return 1;
}

// -- Add -- 

void Map::addPoints(int X, int Y, std::string sText, int iW, int iH) {
	lPoints.push_back(new Points(X, Y, sText, iW, iH));
}

void Map::addText(int X, int Y, std::string sText) {
	vLevelText.push_back(new LevelText(X, Y, sText));
}

// -- Minions --
void Map::addGoombas(int iX, int iY, bool moveDirection) {
	lMinion[getListID(iX)].push_back(new Goombas(iX, iY, iLevelType == 0 || iLevelType == 4 ? 0 : iLevelType == 1 ? 8 : 10, moveDirection));
}

void Map::addKoppa(int iX, int iY, int minionState, bool moveDirection) {
	int tempBlock;

	switch (minionState) {
	case 0: case 3:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 7 : iLevelType == 1 ? 14 : 17;
		break;
	case 1:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 4 : iLevelType == 1 ? 12 : 15;
		break;
	case 2:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 5 : iLevelType == 1 ? 13 : 16;
		break;
	}

	lMinion[getListID(iX)].push_back(new Koppa(iX, iY, minionState, moveDirection, tempBlock));
}

void Map::addBeetle(int X, int Y, bool moveDirection) {
	lMinion[getListID(X)].push_back(new Beetle(X, Y, moveDirection));
}

void Map::addBowser(int X, int Y, bool spawnHammer) {
	lMinion[getListID(X)].push_back(new Bowser((float)X, (float)Y, spawnHammer));
}

void Map::addPlant(int iX, int iY) {
	lMinion[getListID(iX)].push_back(new Plant(iX, iY, iLevelType == 0 || iLevelType == 4 ? 18 : 19));
}

void Map::addToad(int X, int Y, bool peach) {
	lMinion[getListID(X)].push_back(new Toad(X, Y, peach));
}

void Map::addSquid(int X, int Y) {
	lMinion[getListID(X)].push_back(new Squid(X, Y));
}

void Map::addHammer(int X, int Y, bool moveDirection) {
	lMinion[getListID(X)].push_back(new Hammer(X, Y, moveDirection));
}

void Map::addHammerBro(int X, int Y) {
	lMinion[getListID(X)].push_back(new HammerBro(X, Y));
}

void Map::addFireBall(int X, int Y, int iWidth, int iSliceID, bool DIR) {
	for (int i = 0; i < iWidth; i++) {
		lMinion[getListID((int)X)].push_back(new FireBall(X + 8, Y + 8, 14 * i, iSliceID, DIR));
	}
}

void Map::addSpikey(int X, int Y) {
	lMinion[getListID(X)].push_back(new Spikey(X, Y));
}

void Map::addPlayerFireBall(int X, int Y, bool moveDirection) {
	Minion* tmp = PlayerFireBallPool::Instance().getPlayerFireBall(X, Y, moveDirection);
	if (tmp != nullptr) {
		lMinion[getListID(X)].push_back(tmp);
	}
	//lMinion[getListID(X)].push_back(new PlayerFireBall(X, Y, moveDirection));
}

void Map::addUpFire(int X, int iYEnd) {
	lMinion[getListID(X)].push_back(new UpFire(X, iYEnd));
}

void Map::addFire(float fX, float fY, int toYPos) {
	lMinion[getListID((int)fX)].push_back(new Fire(fX, fY, toYPos));
}

void Map::addCheep(int X, int Y, int minionType, int moveSpeed, bool moveDirection) {
	lMinion[getListID(X)].push_back(new Cheep(X, Y, minionType, moveSpeed, moveDirection));
}

void Map::addCheepSpawner(int X, int XEnd) {
	lMinion[getListID(X)].push_back(new CheepSpawner(X, XEnd));
}

void Map::addBubble(int X, int Y) {
	lBubble.push_back(new Bubble(X, Y));
}

void Map::addLakito(int X, int Y, int iMaxXPos) {
	lMinion[getListID(X)].push_back(new Lakito(X, Y, iMaxXPos));
}

void Map::addVine(int X, int Y, int minionState, int iBlockID) {
	lMinion[getListID(X)].push_back(new Vine(X, Y, minionState, iBlockID));
	if (minionState == 0) {
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cVINE);
	}
}

void Map::addSpring(int X, int Y) {
	lMinion[getListID(X)].push_back(new Spring(X, Y));
	//lMap[X/32][(CCFG::GAME_HEIGHT - 16 - Y)/32 - 1]->setBlockID(83);
}

void Map::addBulletBillSpawner(int X, int Y, int minionState) {
	lMinion[getListID(X * 32)].push_back(new BulletBillSpawner(X * 32, CFG::GameHeight - Y * 32, minionState));
}

void Map::addBulletBill(int X, int Y, bool moveDirection, int minionState) {
	lMinion[getListID(X)].push_back(new BulletBill(X, Y, moveDirection, minionState));
}

void Map::lockMinions() {
	for (unsigned int i = 0; i < lMinion.size(); i++) {
		for (unsigned int j = 0; j < lMinion[i].size(); j++) {
			lMinion[i][j]->lockMinion();
		}
	}
}

void Map::Save(const std::string& filename) const {
	std::ofstream outFile(filename, std::ios::binary);
	if (!outFile) {
		std::cerr << "Error opening file for saving!\n";
		return;
	}
	outFile.write(reinterpret_cast<const char*>(&fXPos), sizeof(fXPos));
	outFile.write(reinterpret_cast<const char*>(&fYPos), sizeof(fYPos));
	outFile.write(reinterpret_cast<const char*>(&currentLevelID), sizeof(currentLevelID));
	outFile.write(reinterpret_cast<const char*>(&iMapWidth), sizeof(iMapWidth));
	outFile.write(reinterpret_cast<const char*>(&iMapHeight), sizeof(iMapHeight));
	outFile.write(reinterpret_cast<const char*>(&iMapTime), sizeof(iMapTime));
	outFile.write(reinterpret_cast<const char*>(&iLevelType), sizeof(iLevelType));
	outFile.write(reinterpret_cast<const char*>(&inEvent), sizeof(inEvent));
	outFile.write(reinterpret_cast<const char*>(&iMapTime), sizeof(iMapTime));
	pPlayer->Save(outFile);
	outFile.close();
}

Map* Map::Load(const std::string& filename) {
	std::ifstream inFile(filename, std::ios::binary);
	if (!inFile) {
		std::cerr << "Error opening file for loading!\n";
		return nullptr;
	}
	Map* tem = new Map();
	inFile.read(reinterpret_cast<char*>(&tem->fXPos), sizeof(tem->fXPos));
	inFile.read(reinterpret_cast<char*>(&tem->fYPos), sizeof(tem->fYPos));
	inFile.read(reinterpret_cast<char*>(&tem->currentLevelID), sizeof(tem->currentLevelID));
	inFile.read(reinterpret_cast<char*>(&tem->iMapWidth), sizeof(tem->iMapWidth));
	inFile.read(reinterpret_cast<char*>(&tem->iMapHeight), sizeof(tem->iMapHeight));
	inFile.read(reinterpret_cast<char*>(&tem->iMapTime), sizeof(tem->iMapTime));
	inFile.read(reinterpret_cast<char*>(&tem->iLevelType), sizeof(tem->iLevelType));
	inFile.read(reinterpret_cast<char*>(&tem->inEvent), sizeof(tem->inEvent));
	inFile.read(reinterpret_cast<char*>(&tem->iMapTime), sizeof(tem->iMapTime));
	tem->pPlayer = vPlayer[indexPlayer];
	tem->pPlayer->Load(inFile);
	inFile.close();
	return tem;
}

void Map::change(Map* other) {
	this->fXPos = other->fXPos;
	this->fYPos = other->fYPos;
	this->currentLevelID = other->currentLevelID;
	this->iMapWidth = other->iMapWidth;
	this->iMapHeight = other->iMapHeight;
	this->iMapTime = other->iMapTime;
	this->iLevelType = other->iLevelType;
	this->inEvent = other->inEvent;
	this->iMapTime = other->iMapTime;

	// Assigning the player object and loading its data
	this->pPlayer = other->pPlayer;
}
