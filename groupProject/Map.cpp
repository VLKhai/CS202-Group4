#include "Map.h"

Map::Map(sf::RenderWindow& mainWindow)
{
	player = new Player(mainWindow, 84, 480);

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

	CFG::getText()->setFont(mainWindow, "font");

	pEvent = new Event();
	pFlag = NULL;

	srand((unsigned)time(NULL));

	loadGameData(mainWindow);
	loadLVL();
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

void Map::update()
{
	updateGifBlocks();
	if (!player->getInLevelAnimation()) {
		updateMinionBlocks();

		updateMinions();

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
		player->powerUPAnimation();
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
	if ((player->getPowerLVL() == 0 && !player->getUnkillAble()) || instantDeath) {
		inEvent = true;

		pEvent->resetData();
		player->resetJump();
		player->stopMove();

		pEvent->iDelay = 150;
		pEvent->newCurrentLevel = currentLevelID;

		pEvent->newMoveMap = bMoveMap;

		pEvent->eventTypeID = pEvent->eNormal;

		player->resetPowerLVL();

		if (animation) {
			pEvent->iSpeed = 4;
			pEvent->newLevelType = iLevelType;

			player->setYPos(player->getYPos() + 4.0f);

			pEvent->vOLDDir.push_back(pEvent->eDEATHNOTHING);
			pEvent->vOLDLength.push_back(30);

			pEvent->vOLDDir.push_back(pEvent->eDEATHTOP);
			pEvent->vOLDLength.push_back(64);

			pEvent->vOLDDir.push_back(pEvent->eDEATHBOT);
			pEvent->vOLDLength.push_back(CFG::GameHeight - player->getYPos() + 128);
		}
		else {
			pEvent->iSpeed = 4;
			pEvent->newLevelType = iLevelType;

			pEvent->vOLDDir.push_back(pEvent->eDEATHTOP);
			pEvent->vOLDLength.push_back(1);
		}

		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(64);

		if (player->getNumOfLives() > 1) {
			pEvent->vOLDDir.push_back(pEvent->eLOADINGMENU);
			pEvent->vOLDLength.push_back(90);

			player->setNumOfLives(player->getNumOfLives() - 1);

			CFG::getMusic()->StopMusic();
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cDEATH);
		}
		else {
			pEvent->vOLDDir.push_back(pEvent->eGAMEOVER);
			pEvent->vOLDLength.push_back(90);

			player->setNumOfLives(player->getNumOfLives() - 1);

			CFG::getMusic()->StopMusic();
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cDEATH);
		}
	}
	else if (!player->getUnkillAble()) {
		player->setPowerLVL(player->getPowerLVL() - 1);
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
	player->update();
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

	/*for (unsigned int i = 0; i < lBubble.size(); i++) {
		lBubble[i]->Update();

		if (lBubble[i]->getDestroy()) {
			delete lBubble[i];
			lBubble.erase(lBubble.begin() + i);
		}
	}*/
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

	if (!inEvent && !player->getInLevelAnimation()) {
		// ----- COLLISION WITH PLAYER
		for (int i = getListID(-(int)fXPos + player->getXPos()) - (getListID(-(int)fXPos + player->getXPos()) > 0 ? 1 : 0), iSize = i + 2; i < iSize; i++) {
			for (unsigned int j = 0, jSize = lMinion[i].size(); j < jSize; j++) {
				if (lMinion[i][j]->deadTime < 0) {
					if ((player->getXPos() - fXPos >= lMinion[i][j]->getXPos() && player->getXPos() - fXPos <= lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX) || (player->getXPos() - fXPos + player->getHitBoxX() >= lMinion[i][j]->getXPos() && player->getXPos() - fXPos + player->getHitBoxX() <= lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX)) {
						if (lMinion[i][j]->getYPos() - 2 <= player->getYPos() + player->getHitBoxY() && lMinion[i][j]->getYPos() + 16 >= player->getYPos() + player->getHitBoxY()) {
							lMinion[i][j]->collisionWithPlayer(true);
						}
						else if ((lMinion[i][j]->getYPos() <= player->getYPos() + player->getHitBoxY() && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= player->getYPos() + player->getHitBoxY()) || (lMinion[i][j]->getYPos() <= player->getYPos() && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= player->getYPos())) {
							lMinion[i][j]->collisionWithPlayer(false);
						}
					}
				}
			}
		}
	}
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

	/*for (unsigned int i = 0; i < lBubble.size(); i++) {
		lBubble[i]->Draw(mainWindow, vBlock[lBubble[i]->getBlockID()]->getAniSprite()->getTexture());
	}*/

	player->draw(mainWindow);

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
			//CFG::getText()->DrawWS(mainWindow, std::to_string(i), lMinion[i][j]->getXPos() + (int)fXPos, lMinion[i][j]->getYPos(), 0, 0, 0, 8);
		}
	}
}

void Map::DrawGameLayout(sf::RenderWindow& mainWindow) {
	CFG::getText()->Draw(mainWindow, "MARIO", 54, 16);

	if (player->getScore() < 100) {
		CFG::getText()->Draw(mainWindow, "00000" + std::to_string(player->getScore()), 54, 32);
	}
	else if (player->getScore() < 1000) {
		CFG::getText()->Draw(mainWindow, "000" + std::to_string(player->getScore()), 54, 32);
	}
	else if (player->getScore() < 10000) {
		CFG::getText()->Draw(mainWindow, "00" + std::to_string(player->getScore()), 54, 32);
	}
	else if (player->getScore() < 100000) {
		CFG::getText()->Draw(mainWindow, "0" + std::to_string(player->getScore()), 54, 32);
	}
	else {
		CFG::getText()->Draw(mainWindow, std::to_string(player->getScore()), 54, 32);
	}

	CFG::getText()->Draw(mainWindow, "WORLD", 462, 16);
	CFG::getText()->Draw(mainWindow, getLevelName(), 480, 32);

	if (iLevelType != 1) {
		vBlock[2]->draw(mainWindow, 268, 32);
	}
	else {
		vBlock[57]->draw(mainWindow, 268, 32);
	}
	CFG::getText()->Draw(mainWindow, "y", 286, 32);
	CFG::getText()->Draw(mainWindow, (player->getCoins() < 10 ? "0" : "") + std::to_string(player->getCoins()), 302, 32);

	CFG::getText()->Draw(mainWindow, "TIME", 672, 16);
	if (CFG::getMenuManager()->getViewID() == CFG::getMenuManager()->eGame) {
		if (iMapTime > 100) {
			CFG::getText()->Draw(mainWindow, std::to_string(iMapTime), 680, 32);
		}
		else if (iMapTime > 10) {
			CFG::getText()->Draw(mainWindow, "0" + std::to_string(iMapTime), 680, 32);
		}
		else {
			CFG::getText()->Draw(mainWindow, "00" + std::to_string(iMapTime), 680, 32);
		}
	}
}

// POS 0 = TOP, 1 = BOT
bool Map::blockUse(int nX, int nY, int iBlockID, int POS) {
	if (POS == 0) {
		switch (iBlockID) {
		case 8: case 55: // ----- BlockQ
			if (lMap[nX][nY]->getSpawnMushroom()) {
				if (lMap[nX][nY]->getPowerUP()) {
					if (player->getPowerLVL() == 0) {
						//lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, true, nX, nY));
					}
					else {
						//lMinion[getListID(32 * nX)].push_back(new Flower(32 * nX, CFG::GameHeight - 16 - 32 * nY, nX, nY));
					}
				}
				else {
					//lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, false, nX, nY));
				}
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMAPPER);
			}
			else {
				lCoin.push_back(new Coin(nX * 32 + 7, CFG::GameHeight - nY * 32 - 48));
				player->setScore(player->getScore() + 200);
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);
				player->setCoins(player->getCoins() + 1);
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
				//lMinion[getListID(32 * nX)].push_back(new Star(32 * nX, CFG::GameHeight - 16 - 32 * nY, nX, nY));
				lMap[nX][nY]->startBlockAnimation();
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMAPPER);
			}
			else if (lMap[nX][nY]->getSpawnMushroom()) {
				lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 9 : iLevelType == 1 ? 56 : 80);
				if (lMap[nX][nY]->getPowerUP()) {
					if (player->getPowerLVL() == 0) {
						//lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, true, nX, nY));
					}
					else {
						//lMinion[getListID(32 * nX)].push_back(new Flower(32 * nX, CFG::GameHeight - 16 - 32 * nY, nX, nY));
					}
				}
				else {
					//lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, false, nX, nY));
				}
				lMap[nX][nY]->startBlockAnimation();
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMAPPER);
			}
			else if (lMap[nX][nY]->getNumOfUse() > 0) {
				lCoin.push_back(new Coin(nX * 32 + 7, CFG::GameHeight - nY * 32 - 48));
				player->setScore(player->getScore() + 200);
				player->setCoins(player->getCoins() + 1);

				lMap[nX][nY]->setNumOfUse(lMap[nX][nY]->getNumOfUse() - 1);
				if (lMap[nX][nY]->getNumOfUse() == 0) {
					lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 9 : iLevelType == 1 ? 56 : 80);
				}

				CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);

				lMap[nX][nY]->startBlockAnimation();
			}
			else {
				if (player->getPowerLVL() > 0) {
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
					if (player->getPowerLVL() == 0) {
						//[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, true, nX, nY));
					}
					else {
						//lMinion[getListID(32 * nX)].push_back(new Flower(32 * nX, CFG::GameHeight - 16 - 32 * nY, nX, nY));
					}
				}
				else {
					//lMinion[getListID(32 * nX)].push_back(new Mushroom(32 * nX, CFG::GameHeight - 16 - 32 * nY, false, nX, nY));
				}
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMAPPER);
			}
			else {
				lCoin.push_back(new Coin(nX * 32 + 7, CFG::GameHeight - nY * 32 - 48));
				player->setCoins(player->getCoins() + 1);
				player->setScore(player->getScore() + 200);
				CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);

				lMap[nX][nY]->startBlockAnimation();
			}

			lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 9 : iLevelType == 1 ? 56 : 80);
			checkCollisionOnTopOfTheBlock(nX, nY);
			break;
		case 128: case 129:
			//spawnVine(nX, nY, iBlockID);
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
		player->addCoin();
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
			return CFG::GameHeight - 48 - player->getHitBoxY();;
		}
	case 3: case 7: case 11: case 15: case 19: case 23: case 27: case 31:
		return 150;
	}

	return CFG::GameHeight - 48 - player->getHitBoxY();
}

void Map::setSpawnPoint() {
	float X = (float)getSpawnPointXPos(iSpawnPointID);

	if (X > 6 * 32) {
		fXPos = -(X - 6 * 32);
		player->setXPos(6 * 32);
		player->setYPos((float)getSpawnPointYPos(iSpawnPointID));
	}
	else {
		fXPos = 0;
		player->setXPos(X);
		player->setYPos((float)getSpawnPointYPos(iSpawnPointID));
	}

	player->setMoveDirection(true);
}

void Map::resetGameData() {
	this->currentLevelID = 0;
	this->iSpawnPointID = 0;

	player->setCoins(0);
	player->setScore(0);
	player->resetPowerLVL();

	player->setNumOfLives(3);

	setSpawnPoint();

	loadLVL();
}


void Map::startLevelAnimation() {
	pEvent->newUnderWater = false;

	switch (currentLevelID) {
	case 0:

		break;
	case 1:
		pEvent->resetData();
		player->resetJump();
		player->stopMove();

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
		player->resetJump();
		player->stopMove();

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
		player->resetJump();
		player->stopMove();

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
		player->resetJump();
		player->stopMove();

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
		player->resetJump();
		player->stopMove();

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

void Map::loadLVL()
{
	switch (currentLevelID) {
	case 0:
		loadLVL_1_1();
		break;
	}
}

void Map::moveMap(int nX, int nY)
{
	if (fXPos + nX > 0) {
		player->updateXPos((int)(nX - fXPos));
		fXPos = 0;
	}
	else {
		this->fXPos += nX;
	}
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

	pFlag = new Flag(X * 32 - 16, Y + iHeight + 72*2 + 15);

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

	//addBulletBillSpawner(X, Y + iHieght + 1, 0);
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
		player->setCoins(player->getCoins() + 1);
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

float Map::getXPos()
{
	return fXPos;
}

void Map::setXPos(float val)
{
	fXPos = val;
}

float Map::getYPos()
{
	return fYPos;
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
		//pEvent->resetRedraw();
		//loadLVL();
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
    return player;
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



void Map::loadGameData(sf::RenderWindow& mainWindow)
{
	std::vector<std::string> tSprite;
	std::vector<unsigned int> iDelay;

	// ----- 0 Transparent -----
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(0, new AniSprite(mainWindow, tSprite, iDelay, false), false, true, false, false));
	tSprite.clear();
	iDelay.clear();
	// ----- 1 -----
	tSprite.push_back("gnd_red_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(1, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 2 -----
	tSprite.push_back("coin_0");
	iDelay.push_back(300);
	tSprite.push_back("coin_2");
	iDelay.push_back(30);
	tSprite.push_back("coin_1");
	iDelay.push_back(130);
	tSprite.push_back("coin_2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(2, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 3 -----
	tSprite.push_back("bush_center_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(3, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 4 -----
	tSprite.push_back("bush_center_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(4, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 5 -----
	tSprite.push_back("bush_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(5, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 6 -----
	tSprite.push_back("bush_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(6, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 7 -----
	tSprite.push_back("bush_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(7, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 8 -----
	tSprite.push_back("blockq_0");
	iDelay.push_back(300);
	tSprite.push_back("blockq_2");
	iDelay.push_back(30);
	tSprite.push_back("blockq_1");
	iDelay.push_back(130);
	tSprite.push_back("blockq_2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(8, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 9 -----
	tSprite.push_back("blockq_used");
	iDelay.push_back(0);
	vBlock.push_back(new Block(9, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 10 -----
	tSprite.push_back("grass_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(10, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 11 -----
	tSprite.push_back("grass_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(11, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 12 -----
	tSprite.push_back("grass_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(12, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 13 -----
	tSprite.push_back("brickred");
	iDelay.push_back(0);
	vBlock.push_back(new Block(13, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 14 -----
	tSprite.push_back("cloud_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(14, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 15 -----
	tSprite.push_back("cloud_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(15, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 16 -----
	tSprite.push_back("cloud_center_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(16, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 17 -----
	tSprite.push_back("cloud_center_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(17, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 18 -----
	tSprite.push_back("cloud_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(18, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 19 -----
	tSprite.push_back("cloud_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(19, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 20 -----
	tSprite.push_back("pipe_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(20, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 21 -----
	tSprite.push_back("pipe_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(21, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 22 -----
	tSprite.push_back("pipe_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(22, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 23 -----
	tSprite.push_back("pipe_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(23, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 24 BlockQ2 -----
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(24, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, false));
	tSprite.clear();
	iDelay.clear();
	// ----- 25 -----
	tSprite.push_back("gnd_red2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(25, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 26 -----
	tSprite.push_back("gnd1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(26, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 27 -----
	tSprite.push_back("gnd1_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(27, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 28 -----
	tSprite.push_back("brick1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(28, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 29 -----
	tSprite.push_back("coin_use0");
	iDelay.push_back(300);
	tSprite.push_back("coin_use2");
	iDelay.push_back(30);
	tSprite.push_back("coin_use1");
	iDelay.push_back(130);
	tSprite.push_back("coin_use2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(29, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 30 -----
	tSprite.push_back("pipe1_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(30, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 31 -----
	tSprite.push_back("pipe1_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(31, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 32 -----
	tSprite.push_back("pipe1_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(32, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 33 -----
	tSprite.push_back("pipe1_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(33, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 34 -----
	tSprite.push_back("pipe1_hor_bot_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(34, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 35 -----
	tSprite.push_back("pipe1_hor_top_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(35, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 36 -----
	tSprite.push_back("pipe1_hor_top_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(36, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 37 -----
	tSprite.push_back("pipe1_hor_bot_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(37, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 38 -----
	tSprite.push_back("pipe1_hor_bot_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(38, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 39 -----
	tSprite.push_back("pipe1_hor_top_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(39, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 40 -----
	tSprite.push_back("end0_l");
	iDelay.push_back(0);
	vBlock.push_back(new Block(40, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 41 -----
	tSprite.push_back("end0_dot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(41, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 42 -----
	tSprite.push_back("end0_flag");
	iDelay.push_back(0);
	vBlock.push_back(new Block(42, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 43 -----
	tSprite.push_back("castle0_brick");
	iDelay.push_back(0);
	vBlock.push_back(new Block(43, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 44 -----
	tSprite.push_back("castle0_top0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(44, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 45 -----
	tSprite.push_back("castle0_top1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(45, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 46 -----
	tSprite.push_back("castle0_center_center_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(46, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 47 -----
	tSprite.push_back("castle0_center_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(47, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 48 -----
	tSprite.push_back("castle0_center_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(48, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 49 -----
	tSprite.push_back("castle0_center_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(49, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 50 -----
	tSprite.push_back("coin_an0");
	iDelay.push_back(0);
	tSprite.push_back("coin_an1");
	iDelay.push_back(0);
	tSprite.push_back("coin_an2");
	iDelay.push_back(0);
	tSprite.push_back("coin_an3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(50, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 51 -----
	tSprite.push_back("castle_flag");
	iDelay.push_back(0);
	vBlock.push_back(new Block(51, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 52 -----
	tSprite.push_back("firework0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(52, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 53 -----
	tSprite.push_back("firework1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(53, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 54 -----
	tSprite.push_back("firework2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(54, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 55 -----
	tSprite.push_back("blockq1_0");
	iDelay.push_back(300);
	tSprite.push_back("blockq1_2");
	iDelay.push_back(30);
	tSprite.push_back("blockq1_1");
	iDelay.push_back(130);
	tSprite.push_back("blockq1_2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(55, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 56 -----
	tSprite.push_back("blockq1_used");
	iDelay.push_back(0);
	vBlock.push_back(new Block(56, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 57 -----
	tSprite.push_back("coin1_0");
	iDelay.push_back(300);
	tSprite.push_back("coin1_2");
	iDelay.push_back(30);
	tSprite.push_back("coin1_1");
	iDelay.push_back(130);
	tSprite.push_back("coin1_2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(57, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 58 -----
	tSprite.push_back("pipe_hor_bot_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(58, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 59 -----
	tSprite.push_back("pipe_hor_top_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(59, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 60 -----
	tSprite.push_back("pipe_hor_top_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(60, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 61 -----
	tSprite.push_back("pipe_hor_bot_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(61, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 62 -----
	tSprite.push_back("pipe_hor_bot_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(62, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 63 -----
	tSprite.push_back("pipe_hor_top_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(63, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 64 -----
	tSprite.push_back("block_debris0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(64, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 65 -----
	tSprite.push_back("block_debris1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(65, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 66 -----
	tSprite.push_back("block_debris2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(66, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 67 -----
	tSprite.push_back("t_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(67, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 68 -----
	tSprite.push_back("t_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(68, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 69 -----
	tSprite.push_back("t_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(69, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 70 -----
	tSprite.push_back("t_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(70, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 71 -----
	tSprite.push_back("coin_use00");
	iDelay.push_back(300);
	tSprite.push_back("coin_use02");
	iDelay.push_back(30);
	tSprite.push_back("coin_use01");
	iDelay.push_back(130);
	tSprite.push_back("coin_use02");
	iDelay.push_back(140);
	vBlock.push_back(new Block(71, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 72 -----
	tSprite.push_back("coin_use20");
	iDelay.push_back(300);
	tSprite.push_back("coin_use22");
	iDelay.push_back(30);
	tSprite.push_back("coin_use21");
	iDelay.push_back(130);
	tSprite.push_back("coin_use22");
	iDelay.push_back(140);
	vBlock.push_back(new Block(72, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 73 -----
	tSprite.push_back("coin_use30");
	iDelay.push_back(300);
	tSprite.push_back("coin_use32");
	iDelay.push_back(30);
	tSprite.push_back("coin_use31");
	iDelay.push_back(130);
	tSprite.push_back("coin_use32");
	iDelay.push_back(140);
	vBlock.push_back(new Block(73, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 74 -----
	tSprite.push_back("platform");
	iDelay.push_back(0);
	vBlock.push_back(new Block(74, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 75 -----
	tSprite.push_back("gnd_4");
	iDelay.push_back(0);
	vBlock.push_back(new Block(75, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 76 -----
	tSprite.push_back("bridge_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(76, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 77 -----
	tSprite.push_back("lava_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(77, new AniSprite(mainWindow, tSprite, iDelay, false), false, true, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 78 -----
	tSprite.push_back("lava_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(78, new AniSprite(mainWindow, tSprite, iDelay, false), false, true, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 79 -----
	tSprite.push_back("bridge_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(78, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 80 -----
	tSprite.push_back("blockq2_used");
	iDelay.push_back(0);
	vBlock.push_back(new Block(80, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 81 -----
	tSprite.push_back("brick2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(81, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 82 -----
	tSprite.push_back("axe_0");
	iDelay.push_back(225);
	tSprite.push_back("axe_1");
	iDelay.push_back(225);
	tSprite.push_back("axe_2");
	iDelay.push_back(200);
	vBlock.push_back(new Block(82, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 83 ----- END BRIDGE
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(83, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 84 -----
	tSprite.push_back("tree_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(84, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 85 -----
	tSprite.push_back("tree_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(85, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 86 -----
	tSprite.push_back("tree_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(86, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 87 -----
	tSprite.push_back("tree1_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(87, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 88 -----
	tSprite.push_back("tree1_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(88, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 89 -----
	tSprite.push_back("tree1_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(89, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 90 -----
	tSprite.push_back("fence");
	iDelay.push_back(0);
	vBlock.push_back(new Block(90, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 91 -----
	tSprite.push_back("tree_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(91, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 92 -----
	tSprite.push_back("uw_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(92, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 93 -----
	tSprite.push_back("uw_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(93, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 94 -----
	tSprite.push_back("water_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(94, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 95 -----
	tSprite.push_back("water_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(95, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 96 -----
	tSprite.push_back("bubble");
	iDelay.push_back(0);
	vBlock.push_back(new Block(96, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 97 -----
	tSprite.push_back("pipe2_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(97, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 98 -----
	tSprite.push_back("pipe2_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(98, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 99 -----
	tSprite.push_back("pipe2_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(99, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 100 -----
	tSprite.push_back("pipe2_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(100, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 101 -----
	tSprite.push_back("pipe2_hor_bot_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(101, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 102 -----
	tSprite.push_back("pipe2_hor_top_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(102, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 103 -----
	tSprite.push_back("pipe2_hor_top_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(103, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 104 -----
	tSprite.push_back("pipe2_hor_bot_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(104, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 105 -----
	tSprite.push_back("pipe2_hor_bot_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(105, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 106 -----
	tSprite.push_back("pipe2_hor_top_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(106, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 107 -----
	tSprite.push_back("bridge2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(107, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 108 -----
	tSprite.push_back("bridge3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(108, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 109 -----
	tSprite.push_back("platform1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(109, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 110 -----
	tSprite.push_back("water_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(110, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 111 -----
	tSprite.push_back("water_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(111, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 112 -----
	tSprite.push_back("pipe3_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(112, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 113 -----
	tSprite.push_back("pipe3_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(113, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 114 -----
	tSprite.push_back("pipe3_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(114, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 115 -----
	tSprite.push_back("pipe3_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(115, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 116 -----
	tSprite.push_back("pipe3_hor_bot_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(116, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 117 -----
	tSprite.push_back("pipe3_hor_top_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(117, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 118 -----
	tSprite.push_back("pipe3_hor_top_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(118, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 119 -----
	tSprite.push_back("pipe3_hor_bot_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(119, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 120 -----
	tSprite.push_back("pipe3_hor_bot_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(120, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 121 -----
	tSprite.push_back("pipe3_hor_top_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(121, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 122 -----
	tSprite.push_back("bridge4");
	iDelay.push_back(0);
	vBlock.push_back(new Block(122, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 123 -----
	tSprite.push_back("end1_l");
	iDelay.push_back(0);
	vBlock.push_back(new Block(123, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 124 -----
	tSprite.push_back("end1_dot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(124, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 125 -----
	tSprite.push_back("bonus");
	iDelay.push_back(0);
	vBlock.push_back(new Block(125, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 126 -----
	tSprite.push_back("platformbonus");
	iDelay.push_back(0);
	vBlock.push_back(new Block(126, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 127 ----- // -- BONUS END
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(127, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 128 ----- // -- SPAWN VINE
	tSprite.push_back("brickred");
	iDelay.push_back(0);
	vBlock.push_back(new Block(128, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 129 ----- // -- SPAWN VINE
	tSprite.push_back("brick1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(129, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 130 -----
	tSprite.push_back("vine");
	iDelay.push_back(0);
	vBlock.push_back(new Block(130, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 131 -----
	tSprite.push_back("vine1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(131, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 132 -----
	tSprite.push_back("seesaw_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(132, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 133 -----
	tSprite.push_back("seesaw_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(133, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 134 -----
	tSprite.push_back("seesaw_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(134, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 135 -----
	tSprite.push_back("seesaw_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(135, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 136 -----
	tSprite.push_back("pipe4_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(136, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 137 -----
	tSprite.push_back("pipe4_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(137, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 138 -----
	tSprite.push_back("pipe4_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(138, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 139 -----
	tSprite.push_back("pipe4_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(139, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 140 -----
	tSprite.push_back("t_left1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(140, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 141 -----
	tSprite.push_back("t_center1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(141, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 142 -----
	tSprite.push_back("t_right1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(142, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 143 -----
	tSprite.push_back("t_bot0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(143, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 144 -----
	tSprite.push_back("t_bot1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(144, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 145 -----
	tSprite.push_back("b_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(145, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 146 -----
	tSprite.push_back("b_top1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(146, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 147 -----
	tSprite.push_back("b_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(147, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 148 -----
	tSprite.push_back("cloud_left_bot1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(148, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 149 -----
	tSprite.push_back("cloud_center_bot1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(149, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 150 -----
	tSprite.push_back("cloud_center_top1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(150, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 151 -----
	tSprite.push_back("t_left2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(151, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 152 -----
	tSprite.push_back("t_center2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(152, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 153 -----
	tSprite.push_back("t_right2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(153, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 154 -----
	tSprite.push_back("t_bot2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(154, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 155 -----
	tSprite.push_back("castle1_brick");
	iDelay.push_back(0);
	vBlock.push_back(new Block(155, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 156 -----
	tSprite.push_back("castle1_top0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(156, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 157 -----
	tSprite.push_back("castle1_top1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(157, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 158 -----
	tSprite.push_back("castle1_center_center_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(158, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 159 -----
	tSprite.push_back("castle1_center_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(159, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 160 -----
	tSprite.push_back("castle1_center_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(160, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 161 -----
	tSprite.push_back("castle1_center_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(161, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 162 -----
	tSprite.push_back("seesaw1_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(162, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 163 -----
	tSprite.push_back("seesaw1_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(163, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 164 -----
	tSprite.push_back("seesaw1_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(164, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 165 -----
	tSprite.push_back("seesaw1_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(165, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 166 -----
	tSprite.push_back("gnd2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(166, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 167 -----
	tSprite.push_back("gnd2_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(167, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 168 -----
	tSprite.push_back("end1_flag");
	iDelay.push_back(0);
	vBlock.push_back(new Block(168, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 169 ----- TP
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(169, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 170 ----- TP2
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(170, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 171 ----- TP3 - bTP
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(171, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 172 -----
	tSprite.push_back("pipe5_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(172, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 173 -----
	tSprite.push_back("pipe5_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(173, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 174 -----
	tSprite.push_back("pipe5_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(174, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 175 -----
	tSprite.push_back("pipe5_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(175, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 176 -----
	tSprite.push_back("pipe6_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(176, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 177 -----
	tSprite.push_back("pipe6_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(177, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 178 -----
	tSprite.push_back("pipe6_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(178, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 179 -----
	tSprite.push_back("pipe6_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(179, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 180 -----
	tSprite.push_back("crown");
	iDelay.push_back(0);
	vBlock.push_back(new Block(180, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 181 -----
	tSprite.push_back("gnd_5");
	iDelay.push_back(0);
	vBlock.push_back(new Block(166, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 182 ----- ENDUSE
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(182, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();


	// --------------- MINION ---------------

	// ----- 0 -----
	tSprite.push_back("goombas_0");
	iDelay.push_back(200);
	tSprite.push_back("goombas_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(0, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 1 -----
	tSprite.push_back("goombas_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(1, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 2 -----
	tSprite.push_back("mushroom");
	iDelay.push_back(0);
	vMinion.push_back(new Block(2, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 3 -----
	tSprite.push_back("mushroom_1up");
	iDelay.push_back(0);
	vMinion.push_back(new Block(3, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 4 -----
	tSprite.push_back("koopa_0");
	iDelay.push_back(200);
	tSprite.push_back("koopa_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(4, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 5 -----
	tSprite.push_back("koopa_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(5, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 6 -----
	tSprite.push_back("flower0");
	iDelay.push_back(50);
	tSprite.push_back("flower1");
	iDelay.push_back(50);
	tSprite.push_back("flower2");
	iDelay.push_back(50);
	tSprite.push_back("flower3");
	iDelay.push_back(50);
	vMinion.push_back(new Block(6, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 7 -----
	tSprite.push_back("koopa_2");
	iDelay.push_back(200);
	tSprite.push_back("koopa_3");
	iDelay.push_back(200);
	vMinion.push_back(new Block(7, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 8 -----
	tSprite.push_back("goombas1_0");
	iDelay.push_back(200);
	tSprite.push_back("goombas1_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(8, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 9 -----
	tSprite.push_back("goombas1_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(9, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 10 -----
	tSprite.push_back("goombas2_0");
	iDelay.push_back(200);
	tSprite.push_back("goombas2_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(10, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 11 -----
	tSprite.push_back("goombas2_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(11, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 12 -----
	tSprite.push_back("koopa1_0");
	iDelay.push_back(200);
	tSprite.push_back("koopa1_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(12, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 13 -----
	tSprite.push_back("koopa1_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(13, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 14 -----
	tSprite.push_back("koopa1_2");
	iDelay.push_back(200);
	tSprite.push_back("koopa1_3");
	iDelay.push_back(200);
	vMinion.push_back(new Block(14, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 15 -----
	tSprite.push_back("koopa2_0");
	iDelay.push_back(200);
	tSprite.push_back("koopa2_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(15, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 16 -----
	tSprite.push_back("koopa2_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(16, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 17 -----
	tSprite.push_back("koopa2_2");
	iDelay.push_back(200);
	tSprite.push_back("koopa2_3");
	iDelay.push_back(200);
	vMinion.push_back(new Block(17, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 18 -----
	tSprite.push_back("plant_0");
	iDelay.push_back(125);
	tSprite.push_back("plant_1");
	iDelay.push_back(125);
	vMinion.push_back(new Block(18, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 19 -----
	tSprite.push_back("plant1_0");
	iDelay.push_back(125);
	tSprite.push_back("plant1_1");
	iDelay.push_back(125);
	vMinion.push_back(new Block(19, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 20 -----
	tSprite.push_back("bowser0");
	iDelay.push_back(285);
	tSprite.push_back("bowser1");
	iDelay.push_back(285);
	vMinion.push_back(new Block(20, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 21 -----
	tSprite.push_back("bowser2");
	iDelay.push_back(285);
	tSprite.push_back("bowser3");
	iDelay.push_back(285);
	vMinion.push_back(new Block(21, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 22 -----
	tSprite.push_back("fire_0");
	iDelay.push_back(35);
	tSprite.push_back("fire_1");
	iDelay.push_back(35);
	vMinion.push_back(new Block(22, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 23 -----
	tSprite.push_back("fireball_0");
	iDelay.push_back(75);
	tSprite.push_back("fireball_1");
	iDelay.push_back(75);
	tSprite.push_back("fireball_2");
	iDelay.push_back(75);
	tSprite.push_back("fireball_3");
	iDelay.push_back(75);
	vMinion.push_back(new Block(23, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 24 -----
	tSprite.push_back("star_0");
	iDelay.push_back(75);
	tSprite.push_back("star_1");
	iDelay.push_back(75);
	tSprite.push_back("star_2");
	iDelay.push_back(75);
	tSprite.push_back("star_3");
	iDelay.push_back(75);
	vMinion.push_back(new Block(24, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 25 -----
	tSprite.push_back("mushroom1_1up");
	iDelay.push_back(0);
	vMinion.push_back(new Block(25, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 26 -----
	tSprite.push_back("toad");
	iDelay.push_back(0);
	vMinion.push_back(new Block(26, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 27 -----
	tSprite.push_back("peach");
	iDelay.push_back(0);
	vMinion.push_back(new Block(27, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 28 -----
	tSprite.push_back("squid0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(28, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 29 -----
	tSprite.push_back("squid1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(29, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 30 -----
	tSprite.push_back("cheep0");
	iDelay.push_back(120);
	tSprite.push_back("cheep1");
	iDelay.push_back(120);
	vMinion.push_back(new Block(30, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 31 -----
	tSprite.push_back("cheep2");
	iDelay.push_back(110);
	tSprite.push_back("cheep3");
	iDelay.push_back(110);
	vMinion.push_back(new Block(31, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 32 -----
	tSprite.push_back("upfire");
	iDelay.push_back(0);
	vMinion.push_back(new Block(32, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 33 -----
	tSprite.push_back("vine_top");
	iDelay.push_back(0);
	vMinion.push_back(new Block(33, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 34 -----
	tSprite.push_back("vine");
	iDelay.push_back(0);
	vMinion.push_back(new Block(34, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 35 -----
	tSprite.push_back("vine1_top");
	iDelay.push_back(0);
	vMinion.push_back(new Block(35, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 36 -----
	tSprite.push_back("vine1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(36, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 37 -----
	tSprite.push_back("spring_0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(37, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 38 -----
	tSprite.push_back("spring_1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(38, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 39 -----
	tSprite.push_back("spring_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(39, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 40 -----
	tSprite.push_back("spring1_0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(40, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 41 -----
	tSprite.push_back("spring1_1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(41, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 42 -----
	tSprite.push_back("spring1_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(42, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 43 -----
	tSprite.push_back("hammerbro_0");
	iDelay.push_back(175);
	tSprite.push_back("hammerbro_1");
	iDelay.push_back(175);
	vMinion.push_back(new Block(43, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 44 -----
	tSprite.push_back("hammerbro_2");
	iDelay.push_back(155);
	tSprite.push_back("hammerbro_3");
	iDelay.push_back(155);
	vMinion.push_back(new Block(44, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 45 -----
	tSprite.push_back("hammerbro1_0");
	iDelay.push_back(175);
	tSprite.push_back("hammerbro1_1");
	iDelay.push_back(175);
	vMinion.push_back(new Block(45, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 46 -----
	tSprite.push_back("hammerbro1_2");
	iDelay.push_back(155);
	tSprite.push_back("hammerbro1_3");
	iDelay.push_back(155);
	vMinion.push_back(new Block(46, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 47 -----
	tSprite.push_back("hammer_0");
	iDelay.push_back(95);
	tSprite.push_back("hammer_1");
	iDelay.push_back(95);
	tSprite.push_back("hammer_2");
	iDelay.push_back(95);
	tSprite.push_back("hammer_3");
	iDelay.push_back(95);
	vMinion.push_back(new Block(47, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 48 -----
	tSprite.push_back("hammer1_0");
	iDelay.push_back(95);
	tSprite.push_back("hammer1_1");
	iDelay.push_back(95);
	tSprite.push_back("hammer1_2");
	iDelay.push_back(95);
	tSprite.push_back("hammer1_3");
	iDelay.push_back(95);
	vMinion.push_back(new Block(48, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 49 -----
	tSprite.push_back("lakito_0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(49, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 50 -----
	tSprite.push_back("lakito_1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(50, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 51 -----
	tSprite.push_back("spikey0_0");
	iDelay.push_back(135);
	tSprite.push_back("spikey0_1");
	iDelay.push_back(135);
	vMinion.push_back(new Block(51, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 52 -----
	tSprite.push_back("spikey1_0");
	iDelay.push_back(75);
	tSprite.push_back("spikey1_1");
	iDelay.push_back(75);
	vMinion.push_back(new Block(52, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 53 -----
	tSprite.push_back("beetle_0");
	iDelay.push_back(155);
	tSprite.push_back("beetle_1");
	iDelay.push_back(155);
	vMinion.push_back(new Block(53, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 54 -----
	tSprite.push_back("beetle_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(54, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 55 -----
	tSprite.push_back("beetle1_0");
	iDelay.push_back(155);
	tSprite.push_back("beetle1_1");
	iDelay.push_back(155);
	vMinion.push_back(new Block(55, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 56 -----
	tSprite.push_back("beetle1_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(56, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 57 -----
	tSprite.push_back("beetle2_0");
	iDelay.push_back(155);
	tSprite.push_back("beetle2_1");
	iDelay.push_back(155);
	vMinion.push_back(new Block(57, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 58 -----
	tSprite.push_back("beetle2_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(58, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 59 -----
	tSprite.push_back("bulletbill");
	iDelay.push_back(0);
	vMinion.push_back(new Block(59, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 60 -----
	tSprite.push_back("bulletbill1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(60, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 61 -----
	tSprite.push_back("hammer1_0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(61, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 62 -----
	tSprite.push_back("fireball_0");
	iDelay.push_back(155);
	tSprite.push_back("fireball_1");
	iDelay.push_back(155);
	tSprite.push_back("fireball_2");
	iDelay.push_back(155);
	tSprite.push_back("fireball_3");
	iDelay.push_back(155);
	vMinion.push_back(new Block(62, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 63 -----
	tSprite.push_back("firework0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(63, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 64 -----
	tSprite.push_back("firework1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(64, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 65 -----
	tSprite.push_back("firework1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(65, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();

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

	structBush(0, 2, 2);
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

	//*addKoppa(107 * 32, 480, 1, true);

	addGoombas(3648, 480, true);
	addGoombas(3696, 480, true);

	addGoombas(3968, 480, true);
	addGoombas(4016, 480, true);

	addGoombas(4096, 480, true);	addGoombas(4144, 480, true);

	addGoombas(5568, 480, true);
	addGoombas(5612, 480, true);
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
	lPipe.push_back(new Pipe(0, 38, 5, 39, 5, 242 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 253, 3, 253, 2, 67 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 4, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_4_1() {
	lPipe.push_back(new Pipe(0, 132, 5, 133, 5, 252 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 263, 3, 263, 2, 163 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_4_2() {
	lPipe.push_back(new Pipe(0, 84, 4, 85, 4, 272 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 283, 3, 283, 2, 131 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 1, true, 350, 1, false));

	lPipe.push_back(new Pipe(1, 187, 6, 187, 5, 394 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));

	// -- WARP ZONE 5
	lPipe.push_back(new Pipe(0, 214, 4, 215, 4, 84, CFG::GameHeight - 80, 16, 0, true, 350, 1, false));
	// -- WARP ZONE 6
	lPipe.push_back(new Pipe(0, 368, 4, 369, 4, 84, CFG::GameHeight - 80, 20, 4, true, 350, 1, false));
	// -- WARP ZONE 7
	lPipe.push_back(new Pipe(0, 364, 4, 365, 4, 84, CFG::GameHeight - 80, 24, 0, true, 350, 1, false));
	// -- WARP ZONE 8
	lPipe.push_back(new Pipe(0, 360, 4, 361, 4, 84, CFG::GameHeight - 80, 28, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_5_1() {
	lPipe.push_back(new Pipe(0, 156, 7, 157, 7, 230 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 241, 3, 241, 2, 163 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_5_2() {
	lPipe.push_back(new Pipe(0, 135, 4, 136, 4, 0, 32, currentLevelID, 2, true, 350, 1, true));

	lPipe.push_back(new Pipe(1, 62, 7, 62, 6, 195 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_6_2() {
	lPipe.push_back(new Pipe(0, 141, 6, 142, 6, 0, 32, currentLevelID, 2, true, 350, 1, true));

	lPipe.push_back(new Pipe(1, 62, 7, 62, 6, 200 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 4, true, 350, 1, false));

	lPipe.push_back(new Pipe(0, 104, 5, 105, 5, 332 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 343, 3, 343, 2, 120 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 4, true, 350, 1, false));

	lPipe.push_back(new Pipe(0, 238, 4, 239, 4, 362 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 373, 3, 373, 2, 264 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 4, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_7_1() {
	lPipe.push_back(new Pipe(0, 93, 4, 94, 4, 212 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 223, 3, 223, 2, 115 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_7_2() {
	lPipe.push_back(new Pipe(1, 189, 7, 189, 6, 274 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_8_1() {
	lPipe.push_back(new Pipe(0, 104, 5, 105, 5, 412 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 423, 3, 423, 2, 115 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_8_2() {
	lPipe.push_back(new Pipe(0, 156, 5, 157, 5, 242 * 32, 32, currentLevelID, 1, false, 350, 1, false));

	lPipe.push_back(new Pipe(1, 253, 3, 253, 2, 163 * 32, CFG::GameHeight - 16 - 3 * 32, currentLevelID, 0, true, 350, 1, false));
}

void Map::loadPipeEventsLVL_8_4() {
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

	//if (pFlag != NULL) {
	//	delete pFlag;
	//	pFlag = NULL;
	//}

	//pEvent->eventTypeID = pEvent->eNormal;

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
	player->resetJump();
	player->stopMove();

	pEvent->newUnderWater = false;

	CFG::getMusic()->StopMusic();
	CFG::getMusic()->PlayChunk(CFG::getMusic()->cLEVELEND);

	pEvent->eventTypeID = pEvent->eEnd;

	if (player->getYPos() < CFG::GameHeight - 16 - 10 * 32) {
		pFlag->iPoints = 5000;
	}
	else if (player->getYPos() < CFG::GameHeight - 16 - 8 * 32) {
		pFlag->iPoints = 2000;
	}
	else if (player->getYPos() < CFG::GameHeight - 16 - 6 * 32) {
		pFlag->iPoints = 500;
	}
	else if (player->getYPos() < CFG::GameHeight - 16 - 4 * 32) {
		pFlag->iPoints = 200;
	}
	else {
		pFlag->iPoints = 100;
	}

	pEvent->vOLDDir.push_back(pEvent->eRIGHTEND);
	pEvent->vOLDLength.push_back(player->getHitBoxX());

	pEvent->vOLDDir.push_back(pEvent->eENDBOT1);
	pEvent->vOLDLength.push_back((CFG::GameHeight - 16 - 32 * 2) - player->getYPos() - player->getHitBoxY() - 2);

	pEvent->vOLDDir.push_back(pEvent->eENDBOT2);
	pEvent->vOLDLength.push_back((CFG::GameHeight - 16 - 32 * 2) - player->getYPos() - player->getHitBoxY() - 2);

	pEvent->vOLDDir.push_back(pEvent->eRIGHTEND);
	pEvent->vOLDLength.push_back(player->getHitBoxX());

	pEvent->vOLDDir.push_back(pEvent->eBOTRIGHTEND);
	pEvent->vOLDLength.push_back(32);

	pEvent->vOLDDir.push_back(pEvent->eRIGHTEND);
	pEvent->vOLDLength.push_back(132);

	pEvent->iSpeed = 3;

	switch (currentLevelID) {
	case 0: {
		pEvent->newLevelType = 100;

		pEvent->iDelay = 1500;
		pEvent->newCurrentLevel = 1;
		pEvent->inEvent = true;

		pEvent->newMapXPos = -210 * 32;
		pEvent->newPlayerXPos = 64;
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = false;

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = false;

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = false;

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(96);

		pFlag->castleFlagExtraXPos = 96;

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32);

		pFlag->castleFlagExtraXPos = 32;

		player->setMoveDirection(true);

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

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = false;

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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
		pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
		pEvent->newMoveMap = true;

		player->setMoveDirection(true);

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

		player->setMoveDirection(true);

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
	player->resetJump();
	player->stopMove();

	pEvent->endGame = false;

	switch (currentLevelID) {
	case 31:
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
	pEvent->newPlayerYPos = CFG::GameHeight - 48 - player->getHitBoxY();
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

	if (lMap[getBlockIDX((int)(player->getXPos() + player->getHitBoxX() / 2 - fXPos))][6]->getBlockID() == 82) {
		pEvent->vOLDDir.push_back(pEvent->eBOT);
		pEvent->vOLDLength.push_back(CFG::GameHeight - 16 - 5 * 32 - (player->getYPos() + player->getHitBoxY()));
	}
	else {
		pEvent->vOLDDir.push_back(pEvent->eBOTRIGHTEND);
		pEvent->vOLDLength.push_back(CFG::GameHeight - 16 - 5 * 32 - (player->getYPos() + player->getHitBoxY()));

		pEvent->vOLDDir.push_back(pEvent->eRIGHT);
		pEvent->vOLDLength.push_back(32 - CFG::GameHeight - 16 - 5 * 32 - (player->getYPos() + player->getHitBoxY()));
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

	if (currentLevelID == 31) {
		CFG::getMusic()->StopMusic();
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cPRINCESSMUSIC);
	}

	pEvent->vOLDDir.push_back(pEvent->eBOTRIGHTBOSS);
	pEvent->vOLDLength.push_back(8 * 32);

	switch (currentLevelID) {
	case 31:
		pEvent->vOLDDir.push_back(pEvent->eENDGAMEBOSSTEXT1);
		pEvent->vOLDLength.push_back(getBlockIDX((int)(player->getXPos() + player->getHitBoxX() / 2 - fXPos)) * 32 + 7 * 32 + (addOne ? 32 : 0));
		break;
	default:
		pEvent->vOLDDir.push_back(pEvent->eBOSSTEXT1);
		pEvent->vOLDLength.push_back(getBlockIDX((int)(player->getXPos() + player->getHitBoxX() / 2 - fXPos)) * 32 + 7 * 32 + (addOne ? 32 : 0));
		break;
	}

	pEvent->vOLDDir.push_back(pEvent->eRIGHT);
	pEvent->vOLDLength.push_back(2 * 32 + 16);

	pEvent->vOLDDir.push_back(pEvent->eMARIOSPRITE1);
	pEvent->vOLDLength.push_back(1);

	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(90);

	switch (currentLevelID) {
	case 31:
		pEvent->vOLDDir.push_back(pEvent->eENDGAMEBOSSTEXT2);
		pEvent->vOLDLength.push_back(getBlockIDX((int)(player->getXPos() + player->getHitBoxX() / 2 - fXPos)) * 32 + 5 * 32 + (addOne ? 32 : 0) + 28);
		break;
	default:
		pEvent->vOLDDir.push_back(pEvent->eBOSSTEXT2);
		pEvent->vOLDLength.push_back(getBlockIDX((int)(player->getXPos() + player->getHitBoxX() / 2 - fXPos)) * 32 + 5 * 32 + (addOne ? 32 : 0));
		break;
	}

	pEvent->vOLDDir.push_back(pEvent->eNOTHING);
	pEvent->vOLDLength.push_back(300 + (currentLevelID == 31 ? 100 : 0));

	switch (currentLevelID) {
	case 31:
		pEvent->vOLDDir.push_back(pEvent->eNOTHING);
		pEvent->vOLDLength.push_back(90);
		break;
	}
}

void Map::EndBonus() {
	inEvent = true;

	pEvent->resetData();
	player->resetJump();
	player->stopMove();

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
		pEvent->newPlayerYPos = -player->getHitBoxY();

		break;
	}
	case 8: {
		pEvent->newMapXPos = -158 * 32 + 16;
		pEvent->newPlayerXPos = 128;
		pEvent->newPlayerYPos = -player->getHitBoxY();
		break;
	}
	case 17: {
		pEvent->newMapXPos = -207 * 32 + 16;
		pEvent->newPlayerXPos = 128;
		pEvent->newPlayerYPos = -player->getHitBoxY();
		break;
	}
	case 21: {
		pEvent->newMapXPos = -243 * 32 + 16;
		pEvent->newPlayerXPos = 128;
		pEvent->newPlayerYPos = -player->getHitBoxY();
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

void Map::checkSpawnPoint() {
	if (getNumOfSpawnPoints() > 1) {
		for (int i = iSpawnPointID + 1; i < getNumOfSpawnPoints(); i++) {
			if (getSpawnPointXPos(i) > player->getXPos() - fXPos && getSpawnPointXPos(i) < player->getXPos() - fXPos + 128) {
				iSpawnPointID = i;
				//player->getCoins() - fcloseall() = fXPos + player;
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

void Map::lockMinions() {
	for (unsigned int i = 0; i < lMinion.size(); i++) {
		for (unsigned int j = 0; j < lMinion[i].size(); j++) {
			lMinion[i][j]->lockMinion();
		}
	}
}
