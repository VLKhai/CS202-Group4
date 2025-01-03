#include "Player.h"
#include "Core.h"


Player::Player(sf::RenderWindow& window, float fXPos, float fYPos)
{
	this->bUseSkill = false;

	this->fXPos = fXPos;
	this->fYPos = fYPos;
	this->iNumOfLives = 3;

	this->iSpriteID = 1;

	this->iScore = this->iCoins = 0;
	this->iFrameID = 0, this->iComboPoints = 1;

	this->nextBubbleTime = 0;
	this->nextFallFrameID = 0;

	this->powerLVL = 0;
	this->inLevelAnimation = false;
	this->inLevelAnimationType = false;

	this->unKillAble = false;
	this->starEffect = false;
	this->unKillAbleTimeFrameID = 0;

	this->inLevelDownAnimation = false;
	this->inLevelDownAnimationFrameID = 0;

	this->moveDirection = true;
	this->currentMaxMove = 4;
	this->moveSpeed = 0;
	this->bMove = false;
	this->changeMoveDirection = false;
	this->bSquat = false;

	this->onPlatformID = -1;

	this->springJump = false;

	this->iTimePassed = Core::coreClock.getElapsedTime().asMilliseconds();

	this->jumpState = 0;
	this->startJumpSpeed = 7.65f;
	this->currentFallingSpeed = 2.7f;

	this->iMoveAnimationTime = 0;

	this->nextFireBallFrameID = 8;

	srand((unsigned)time(NULL));
}

void Player::movePlayer() {
	if (bMove && !changeMoveDirection && (!bSquat || powerLVL == 0)) {
		if (moveSpeed > currentMaxMove) {
			--moveSpeed;
		}
		else if (Core::coreClock.getElapsedTime().asMilliseconds() - (100 + 35 * moveSpeed) >= iTimePassed && moveSpeed < currentMaxMove) {
			++moveSpeed;
			iTimePassed = Core::coreClock.getElapsedTime().asMilliseconds();
		}
		else if (moveSpeed == 0) {
			moveSpeed = 1;
		}
	}
	else {
		if (Core::coreClock.getElapsedTime().asMilliseconds() - (75 * (moveSpeed) * (bSquat && powerLVL > 0 ? 6 : 1)) > iTimePassed && moveSpeed != 0) {
			--moveSpeed;
			iTimePassed = Core::coreClock.getElapsedTime().asMilliseconds();
			if (jumpState == 0 && !Core::getMap()->getUnderWater()) setSpriteID(6);
		}

		if (changeMoveDirection && moveSpeed <= 1) {
			moveDirection = newMoveDirection;
			changeMoveDirection = false;
			bMove = true;
		}
	}

	if (moveSpeed > 0) {
		if (moveDirection) {
			updateXPos(moveSpeed*hackMove);
		}
		else {
			updateXPos(-moveSpeed);
		}

		// ----- SPRITE ANIMATION
		if (Core::getMap()->getUnderWater()) {
			swimingAnimation();
		}
		else if (!changeMoveDirection && jumpState == 0 && bMove) {
			moveAnimation();
		}
		// ----- SPRITE ANIMATION
	}
	else if (jumpState == 0) {
		setSpriteID(1);
		updateXPos(0);
	}
	else {
		updateXPos(0);
	}

	if (bSquat && !Core::getMap()->getUnderWater() && powerLVL > 0) {
		setSpriteID(7);
	}
}

bool Player::checkCollisionBot(int nX, int nY) {
	Vector2* vLT = getBlockLB(fXPos - Core::getMap()->getXPos(iIDPlayer) + nX, fYPos + nY);

	if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
		Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 1);
	}

	delete vLT;

	vLT = getBlockRB(fXPos - Core::getMap()->getXPos(iIDPlayer) + nX, fYPos + nY);

	if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
		Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 1);
	}

	delete vLT;
	return true;
}

bool Player::checkCollisionCenter(int nX, int nY)
{
	if (powerLVL == 0) {
		Vector2* vLT = getBlockLC(fXPos - Core::getMap()->getXPos(iIDPlayer) + nX, fYPos + nY);

		if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
			Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 2);
		}

		delete vLT;

		vLT = getBlockRC(fXPos - Core::getMap()->getXPos(iIDPlayer) + nX, fYPos + nY);

		if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
			Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 2);
		}

		delete vLT;
	}
	else {
		Vector2* vLT = getBlockLC(fXPos - Core::getMap()->getXPos(iIDPlayer) + nX, fYPos + nY + (powerLVL > 0 ? 16 : 0));

		if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
			Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 2);
		}

		delete vLT;

		vLT = getBlockRC(fXPos - Core::getMap()->getXPos(iIDPlayer) + nX, fYPos + nY + (powerLVL > 0 ? 16 : 0));

		if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
			Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 2);
		}

		delete vLT;

		vLT = getBlockLC(fXPos - Core::getMap()->getXPos(iIDPlayer) + nX, fYPos + nY - (powerLVL > 0 ? 16 : 0));

		if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
			Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 2);
		}

		delete vLT;

		vLT = getBlockRC(fXPos - Core::getMap()->getXPos(iIDPlayer) + nX, fYPos + nY - (powerLVL > 0 ? 16 : 0));

		if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
			Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 2);
		}

		delete vLT;
	}

	return true;
}

Vector2* Player::getBlockLB(float nX, float nY) {
	return Core::getMap()->getBlockID((int)nX + 1, (int)nY + getHitBoxY() + 2);
}

Vector2* Player::getBlockRB(float nX, float nY) {
	return Core::getMap()->getBlockID((int)nX + getHitBoxX() - 1, (int)nY + getHitBoxY() + 2);
}

Vector2* Player::getBlockLC(float nX, float nY) {
	return Core::getMap()->getBlockID((int)nX - 1, (int)nY + getHitBoxY() / 2);
}

Vector2* Player::getBlockRC(float nX, float nY) {
	return Core::getMap()->getBlockID((int)nX + getHitBoxX() + 1, (int)nY + getHitBoxY() / 2);
}

Vector2* Player::getBlockLT(float nX, float nY) {
	return Core::getMap()->getBlockID((int)nX + 1, (int)nY);
}

Vector2* Player::getBlockRT(float nX, float nY) {
	return Core::getMap()->getBlockID((int)nX + getHitBoxX() - 1, (int)nY);
}

void Player::update()
{
	//unKillAble = starEffect = true;
	playerPhysics();
	movePlayer();

	if (iFrameID > 0) {
		--iFrameID;
	}
	else if (iComboPoints > 1) {
		--iComboPoints;
	}

	if (powerLVL == 2) {
		if (nextFireBallFrameID > 0) {
			--nextFireBallFrameID;
		}
	}

	if (inLevelDownAnimation) {
		if (inLevelDownAnimationFrameID > 0) {
			--inLevelDownAnimationFrameID;
		}
		else {
			unKillAble = false;
		}
	}
}

void Player::copyStats(Player* pPlayer)
{
	this->fXPos = pPlayer->fXPos;
	this->fYPos = pPlayer->fYPos;
	this->moveDirection = pPlayer->moveDirection;
	this->powerLVL = pPlayer->powerLVL;
	this->iNumOfLives = pPlayer->iNumOfLives;
	this->iScore = pPlayer->iScore;
	this->iCoins = pPlayer->iCoins;
	this->iComboPoints = pPlayer->iComboPoints;
	this->iFrameID = pPlayer->iFrameID;
	this->nextBubbleTime = pPlayer->nextBubbleTime;
	this->nextFallFrameID = pPlayer->nextFallFrameID;
	this->currentFallingSpeed = pPlayer->currentFallingSpeed;
	this->currentJumpSpeed = pPlayer->currentJumpSpeed;
	this->currentJumpDistance = pPlayer->currentJumpDistance;
	this->jumpDistance = pPlayer->jumpDistance;
	this->jumpState = pPlayer->jumpState;
	this->startJumpSpeed = pPlayer->startJumpSpeed;
	this->iTimePassed = pPlayer->iTimePassed;
	this->moveSpeed = pPlayer->moveSpeed;
	this->currentMaxMove = pPlayer->currentMaxMove;
	this->bMove = pPlayer->bMove;
	this->changeMoveDirection = pPlayer->changeMoveDirection;
	this->newMoveDirection = pPlayer->newMoveDirection;
	this->bSquat = pPlayer->bSquat;
	this->onPlatformID = pPlayer->onPlatformID;
	this->springJump = pPlayer->springJump;
	this->iMoveAnimationTime = pPlayer->iMoveAnimationTime;
	this->bUseSkill = pPlayer->bUseSkill;
	this->inLevelAnimation = pPlayer->inLevelAnimation;
	this->inLevelAnimationType = pPlayer->inLevelAnimationType;
	this->inLevelDownAnimation = pPlayer->inLevelDownAnimation;
	this->inLevelDownAnimationFrameID = pPlayer->inLevelDownAnimationFrameID;
	this->unKillAble = pPlayer->unKillAble;
	this->unKillAbleTimeFrameID = pPlayer->unKillAbleTimeFrameID;
	this->starEffect = pPlayer->starEffect;
	this->nextFireBallFrameID = pPlayer->nextFireBallFrameID;
}

void Player::playerPhysics()
{
	// 1: Jumping, 2: Falling, 0: On ground (jumpState)
	// -1: on platform (onPlatformID)
	if (!Core::getMap()->getUnderWater())
	{
		if (jumpState == 1) {
			updateYPos(-int(currentJumpSpeed));
			currentJumpDistance += (int)currentJumpSpeed;

			currentJumpSpeed *= (currentJumpDistance / jumpDistance > 0.75f ? 0.972f : 0.986f);

			if (currentJumpSpeed < 2.5f) {
				currentJumpSpeed = 2.5f;
			}

			if (!bJumpPressed && currentJumpDistance > 32 * limSmallJumpBlock && !springJump) {
				jumpDistance = 16;
				currentJumpDistance = 0;
				currentJumpSpeed = 2.5f;
			}

			if (jumpDistance <= currentJumpDistance) {
				jumpState = 2;
			}
		}
		else {
			if (onPlatformID == -1) {
				onPlatformID = Core::getMap()->checkCollisionWithPlatform((int)fXPos, (int)fYPos, getHitBoxX(), getHitBoxY());
				if (onPlatformID >= 0) {
					if (Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + 2), (int)fYPos + 2, getHitBoxY(), true)
						|| Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) - 2), (int)fYPos + 2, getHitBoxX(), getHitBoxY(), true)) {
						onPlatformID = -1;
						resetJump();
					}
					else {
						fYPos = (float)Core::getMap()->getPlatform(onPlatformID)->getYPos() - getHitBoxY();
						resetJump();
						Core::getMap()->getPlatform(onPlatformID)->turnON();
					}
				}
			}
			else {
				onPlatformID = Core::getMap()->checkCollisionWithPlatform((int)fXPos, (int)fYPos, getHitBoxX(), getHitBoxY());
			}

			if (onPlatformID >= 0) {
				if (Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + 2), (int)fYPos + 2, getHitBoxY(), true)
					|| Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) - 2), (int)fYPos + 2, getHitBoxX(), getHitBoxY(), true)) {
					onPlatformID = -1;
					resetJump();
				}
				else {
					fYPos += Core::getMap()->getPlatform(onPlatformID)->getMoveY();
					Core::getMap()->getPlatform(onPlatformID)->moveY();
					Core::getMap()->setXPos(Core::getMap()->getXPos(iIDPlayer) - Core::getMap()->getPlatform(onPlatformID)->getMoveX());

					jumpState = 0;
				}
			}
			else if (!Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + 2), (int)fYPos + 2, getHitBoxY(), true) &&
				!Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) - 2), (int)fYPos + 2, getHitBoxX(), getHitBoxY(), true)) {

				if (nextFallFrameID > 0) {
					--nextFallFrameID;
				}
				else {
					currentFallingSpeed *= 1.05f;

					if (currentFallingSpeed > startJumpSpeed) {
						currentFallingSpeed = startJumpSpeed;
					}

					updateYPos((int)currentFallingSpeed);
				}


				jumpState = 2;

				setSpriteID(5);
			}
			else if (jumpState == 2) { // Falling
				resetJump();
			}
			else {
				checkCollisionBot(0, 0);
			}
		}
	}
	else { // Underwater
		if (nextBubbleTime + 685 < Core::coreClock.getElapsedTime().asMilliseconds()) {
			Core::getMap()->addBubble((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + (moveDirection ? getHitBoxX() - rand() % 8 : rand() % 8)), (int)fYPos + 4);
			nextBubbleTime = Core::coreClock.getElapsedTime().asMilliseconds() + rand() % 715;
		}

		if (jumpState == 1) {
			if (fYPos > CFG::GameHeight - 12 * 32 - 16) {
				updateYPos(-2);
				currentJumpDistance += 2;

				swimingAnimation();

				if (jumpDistance <= currentJumpDistance) {
					jumpState = 2;
					currentJumpDistance = 0;
					nextFallFrameID = 4;
				}
			}
			else {
				jumpState = 2;
				nextFallFrameID = 14;
				currentJumpDistance = 0;
			}
		}
		else {
			if (!Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + 2), (int)fYPos + 2, getHitBoxY(), true) &&
				!Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) - 2), (int)fYPos + 2, getHitBoxX(), getHitBoxY(), true)) {

				if (nextFallFrameID > 0) {
					--nextFallFrameID;
				}
				else {
					if (currentJumpDistance < 32) {
						updateYPos(1);
						++currentJumpDistance;
					}
					else {
						updateYPos(2);
					}
				}

				jumpState = 2;

				swimingAnimation();
			}
			else if (jumpState == 2) {
				resetJump();
			}
		}
	}
}

void Player::updateXPos(int iN)
{
	checkCollisionBot(iN, 0);
	checkCollisionCenter(iN, 0);
	if (iN > 0) {
		if (!Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + iN), (int)fYPos - 2, getHitBoxX(), getHitBoxY(), true)
			&& !Core::getMap()->checkCollisionRT((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + iN), (int)fYPos + 2, getHitBoxX(), true)
			&& (powerLVL == 0 ? true : (!Core::getMap()->checkCollisionRC((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + iN), (int)fYPos, getHitBoxX(), getHitBoxY() / 2, true))))
		{
			if (fXPos >= 416 && Core::getMap()->getMoveMap()) {
				Core::getMap()->moveMap(-iN, 0, iIDPlayer);
			}
			else {
				fXPos += iN;
			}
		}
		else {
			updateXPos(iN - 1);
			if (moveSpeed > 1 && jumpState == 0) --moveSpeed;
		}
	}
	else if (iN < 0) {
		if (!Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + iN), (int)fYPos - 2, getHitBoxY(), true)
			&& !Core::getMap()->checkCollisionLT((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + iN), (int)fYPos + 2, true)
			&& (powerLVL == 0 ? true : (!Core::getMap()->checkCollisionLC((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + iN), (int)fYPos, getHitBoxY() / 2, true))))
		{
			if (fXPos <= 192 && Core::getMap()->getXPos(iIDPlayer) && Core::getMap()->getMoveMap() && CFG::canMoveBackward) {

				Core::getMap()->moveMap(-iN, 0, iIDPlayer);
			}
			else if (fXPos - Core::getMap()->getXPos(iIDPlayer) + iN >= 0 && fXPos >= 0) {
				fXPos += iN;
			}
			else if (CFG::canMoveBackward && fXPos >= 0) {
				updateXPos(iN + 1);
			}
		}
		else {
			updateXPos(iN + 1);
			if (moveSpeed > 1 && jumpState == 0) --moveSpeed;
		}
	}
}

void Player::updateYPos(int iN) {
	bool bLEFT, bRIGHT;

	if (iN > 0) {
		bLEFT = Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + 2), (int)fYPos + iN, getHitBoxY(), true);
		bRIGHT = Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) - 2), (int)fYPos + iN, getHitBoxX(), getHitBoxY(), true);

		if (!bLEFT && !bRIGHT) {
			fYPos += iN;
		}
		else {
			if (jumpState == 2) {
				jumpState = 0;
			}
			updateYPos(iN - 1);
		}
	}
	else if (iN < 0) {
		bLEFT = Core::getMap()->checkCollisionLT((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + 2), (int)fYPos + iN, false);
		bRIGHT = Core::getMap()->checkCollisionRT((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) - 2), (int)fYPos + iN, getHitBoxX(), false);

		if (Core::getMap()->checkCollisionWithPlatform((int)fXPos, (int)fYPos, 0, 0) >= 0 || Core::getMap()->checkCollisionWithPlatform((int)fXPos, (int)fYPos, getHitBoxX(), 0) >= 0) {
			jumpState = 2;
		}
		else if (!bLEFT && !bRIGHT) {
			fYPos += iN;
		}
		else {
			if (jumpState == 1) {
				if (!bLEFT && bRIGHT) {
					Vector2* vRT = getBlockRT(fXPos - Core::getMap()->getXPos(iIDPlayer), fYPos + iN);

					if (!Core::getMap()->getBlock(Core::getMap()->getMapBlock(vRT->getX(), vRT->getY())->getBlockID())->getVisible()) {
						if (Core::getMap()->blockUse(vRT->getX(), vRT->getY(), Core::getMap()->getMapBlock(vRT->getX(), vRT->getY())->getBlockID(), 0)) {
							jumpState = 2;
						}
						else {
							fYPos += iN;
						}
					}
					else if ((int)(fXPos + getHitBoxX() - Core::getMap()->getXPos(iIDPlayer)) % 32 <= 8) {
						updateXPos((int)-((int)(fXPos + getHitBoxX() - Core::getMap()->getXPos(iIDPlayer)) % 32));
					}
					else if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vRT->getX(), vRT->getY())->getBlockID())->getUse()) {
						if (Core::getMap()->blockUse(vRT->getX(), vRT->getY(), Core::getMap()->getMapBlock(vRT->getX(), vRT->getY())->getBlockID(), 0)) {
							jumpState = 2;
						}
						else {
							fYPos += iN;
						}
					}
					else {
						jumpState = 2;
					}

					delete vRT;
				}
				else if (bLEFT && !bRIGHT) {
					Vector2* vLT = getBlockLT(fXPos - Core::getMap()->getXPos(iIDPlayer), fYPos + iN);
					if (!Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getVisible()) {
						if (Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 0)) {
							jumpState = 2;
						}
						else {
							fYPos += iN;
						}
					}
					else if ((int)(fXPos - Core::getMap()->getXPos(iIDPlayer)) % 32 >= 24) {
						updateXPos((int)(32 - (int)(fXPos - Core::getMap()->getXPos(iIDPlayer)) % 32));
					}
					else if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
						if (Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 0)) {
							jumpState = 2;
						}
						else {
							fYPos += iN;
						}
					}
					else {
						jumpState = 2;
					}

					delete vLT;
				}
				else {
					if ((int)(fXPos + getHitBoxX() - Core::getMap()->getXPos(iIDPlayer)) % 32 > 32 - (int)(fXPos - Core::getMap()->getXPos(iIDPlayer)) % 32) {
						Vector2* vRT = getBlockRT(fXPos - Core::getMap()->getXPos(iIDPlayer), fYPos + iN);

						if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vRT->getX(), vRT->getY())->getBlockID())->getUse()) {
							if (Core::getMap()->blockUse(vRT->getX(), vRT->getY(), Core::getMap()->getMapBlock(vRT->getX(), vRT->getY())->getBlockID(), 0)) {
								jumpState = 2;
							}
						}
						else {
							jumpState = 2;
						}

						delete vRT;
					}
					else {
						Vector2* vLT = getBlockLT(fXPos - Core::getMap()->getXPos(iIDPlayer), fYPos + iN);

						if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
							if (Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 0)) {
								jumpState = 2;
							}
						}
						else {
							jumpState = 2;
						}

						delete vLT;
					}
				}
			}

			updateYPos(iN + 1);
		}
	}

	if ((int)fYPos % 2 == 1) {
		fYPos += 1;
	}

	if (!Core::getMap()->getInEvent() && fYPos - getHitBoxY() > CFG::GameHeight) {
		Core::getMap()->playerDeath(false, true);
		fYPos = -80;
	}
}

void Player::moveAnimation()
{
	if (Core::coreClock.getElapsedTime().asMilliseconds() - (65 + moveSpeed * 4) > iMoveAnimationTime) {
		iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
		if (iSpriteID >= 4 + 11 * powerLVL) {
			setSpriteID(2);
		}
		else {
			++iSpriteID;
		}
	}
}

void Player::swimingAnimation()
{
	if (Core::coreClock.getElapsedTime().asMilliseconds() - 105 > iMoveAnimationTime) {
		iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
		if (iSpriteID % 11 == 8) {
			setSpriteID(9);
		}
		else {
			setSpriteID(8);
		}
	}
}

void Player::powerUPAnimation()
{
	if (inLevelDownAnimation) {
		if (inLevelAnimationFrameID % 15 < 5) {
			iSpriteID = 12;
			if (inLevelAnimationFrameID != 0 && inLevelAnimationFrameID % 15 == 0) {
				fYPos += 16;
				fXPos -= 4;
			}
		}
		else if (inLevelAnimationFrameID % 15 < 10) {
			iSpriteID = 67;
			if (inLevelAnimationFrameID % 15 == 5) {
				fYPos += 16;
				fXPos += 1;
			}
		}
		else {
			iSpriteID = 1;
			if (inLevelAnimationFrameID % 15 == 10) {
				fYPos -= 32;
				fXPos += 3;
			}
		}

		++inLevelAnimationFrameID;
		if (inLevelAnimationFrameID > 59) {
			inLevelAnimation = false;
			fYPos += 32;
			if (jumpState != 0) {
				setSpriteID(5);
			}
		}
	}
	else if (powerLVL == 1) {
		if (inLevelAnimationFrameID % 15 < 5) {
			iSpriteID = 1;
			if (inLevelAnimationFrameID != 0 && inLevelAnimationFrameID % 15 == 0) {
				fYPos += 32;
				fXPos += 4;
			}
		}
		else if (inLevelAnimationFrameID % 15 < 10) {
			iSpriteID = 67;
			if (inLevelAnimationFrameID % 15 == 5) {
				fYPos -= 16;
				fXPos -= 3;
			}
		}
		else {
			iSpriteID = 12;
			if (inLevelAnimationFrameID % 15 == 10) {
				fYPos -= 16;
				fXPos -= 1;
			}
		}

		++inLevelAnimationFrameID;
		if (inLevelAnimationFrameID > 59) {
			inLevelAnimation = false;
			if (jumpState != 0) {
				setSpriteID(5);
			}
		}
	}
	else if (powerLVL == 2) {
		if (inLevelAnimationFrameID % 10 < 5) {
			iSpriteID = iSpriteID % 11 + 22;
		}
		else {
			iSpriteID = iSpriteID % 11 + 33;
		}

		++inLevelAnimationFrameID;
		if (inLevelAnimationFrameID > 59) {
			inLevelAnimation = false;
			if (jumpState != 0) {
				setSpriteID(5);
			}
			iSpriteID = iSpriteID % 11 + 22;
		}
	}
	else {
		inLevelAnimation = false;
	}
}

bool Player::getStarEffect() {
	return starEffect;
}

void Player::setStarEffect(bool starEffect) {
	if (starEffect && this->starEffect != starEffect) {
		CFG::getMusic()->PlayMusic(CFG::getMusic()->mSTAR);
	}
	this->starEffect = starEffect;
	this->unKillAble = starEffect;

	this->unKillAbleFrameID = 0;

	this->unKillAbleTimeFrameID = 550;
}

int Player::getJumpState()
{
	return jumpState;
}

int Player::getPowerLVL() {
	return powerLVL;
}

void Player::setPowerLVL(int powerLVL) {
	if (powerLVL <= 2) {
		if (this->powerLVL < powerLVL) {
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMMEAT);
			setScore(getScore() + 1000);
			Core::getMap()->addPoints((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + getHitBoxX() / 2), (int)fYPos + 16, "1000", 8, 16);
			inLevelAnimation = true;
			inLevelAnimationFrameID = 0;
			inLevelDownAnimationFrameID = 0;
			inLevelDownAnimation = false;
			this->powerLVL = powerLVL;
		}
		else if (this->powerLVL > 0) {
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cSHRINK);
			inLevelDownAnimation = true;
			inLevelDownAnimationFrameID = 180;
			inLevelAnimation = true;
			inLevelAnimationFrameID = 0;
			this->powerLVL = 0;
			unKillAble = true;
		}
	}
	else {
		++iNumOfLives;
		Core::getMap()->addPoints((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + getHitBoxX() / 2), (int)fYPos + 16, "1UP", 10, 14);
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cONEUP);
	}
}

void Player::resetPowerLVL() {
	this->powerLVL = 0;
	this->iSpriteID = 1;
}

bool Player::getSquat() {
	return bSquat;
}

void Player::setSquat(bool bSquat) {
	if (bSquat && this->bSquat != bSquat) {
		if (powerLVL > 0) {
			fYPos += 20;
		}
		this->bSquat = bSquat;
	}
	else if (this->bSquat != bSquat && !Core::getMap()->getUnderWater()) {
		if (powerLVL > 0) {
			fYPos -= 20;
		}
		this->bSquat = bSquat;
	}
}

unsigned int Player::getScore() {
	return iScore;
}

void Player::setScore(unsigned int iScore) {
	this->iScore = iScore;
}

void Player::addComboPoints() {
	++iComboPoints;
	iFrameID = 40;
}

int Player::getComboPoints() {
	return iComboPoints;
}

void Player::addCoin()
{
	++iCoins;
	iScore += 100;
}

unsigned int Player::getCoins() {
	return iCoins;
}

void Player::setCoins(unsigned int iCoins) {
	this->iCoins = iCoins;
}

void Player::startRun()
{
	currentMaxMove = maxMove + (Core::getMap()->getUnderWater() ? 0 : 2);
	createFireBall();
}

void Player::resetRun()
{
	currentMaxMove = maxMove;
}

void Player::jump()
{
	if (Core::getMap()->getUnderWater()) {
		startJump(1);
		nextBubbleTime -= 65;
	}
	else if (jumpState == 0) {
		startJump(limBigJumpBlock*hackJump);
	}
}

void Player::startJump(int iH) {
	currentJumpSpeed = startJumpSpeed;
	jumpDistance = 32 * iH + 24.0f;
	currentJumpDistance = 0;

	if (!Core::getMap()->getUnderWater()) {
		setSpriteID(5);
	}
	else { // In water
		if (jumpState == 0) {
			iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
			setSpriteID(8);
			swimingAnimation();
		}
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cSWIM);
	}


	if (iH > 1) { // music
		if (powerLVL == 0) {
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cJUMP);
		}
		else {
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cJUMPBIG);
		}
	}
	jumpState = 1;
}

void Player::resetJump()
{
	jumpState = 0;
	jumpDistance = 0;
	currentJumpDistance = 0;
	currentFallingSpeed = 2.7f;
	nextFallFrameID = 0;
	springJump = false;
}

int Player::getHitBoxX() {
	return powerLVL == 0 ? iSmallX : iBigX;
}

int Player::getHitBoxY() {
	return powerLVL == 0 ? iSmallY : bSquat ? 44 : iBigY;
}

void Player::createFireBall()
{
	if (powerLVL == 2) {
		if (nextFireBallFrameID <= 0) {
			Core::getMap()->addPlayerFireBall((int)(fXPos - Core::getMap()->getXPos(iIDPlayer) + (moveDirection ? getHitBoxX() : -32)), (int)(fYPos + getHitBoxY() / 2), !moveDirection);
			nextFireBallFrameID = 16;
			CFG::getMusic()->PlayChunk(CFG::getMusic()->cFIREBALL);
		}
	}
}

void Player::setSpriteID(int iID)
{
	this->iSpriteID = iID + 11 * powerLVL;
}

int Player::getSpriteID()
{
	if (starEffect && !inLevelAnimation && CFG::getMenuManager()->getViewID() == CFG::getMenuManager()->eGame) {
		if (unKillAbleTimeFrameID <= 0) {
			starEffect = unKillAble = false;
		}

		if (unKillAbleTimeFrameID == 35) {
			CFG::getMusic()->changeMusic(true, true);
		}

		++unKillAbleFrameID;

		--unKillAbleTimeFrameID;

		if (unKillAbleTimeFrameID < 90) {
			if (unKillAbleFrameID < 5) {
				return powerLVL < 1 ? iSpriteID + 44 : powerLVL == 2 ? iSpriteID : iSpriteID + 11;
			}
			else if (unKillAbleFrameID < 10) {
				return powerLVL < 1 ? iSpriteID + 55 : powerLVL == 2 ? iSpriteID + 11 : iSpriteID + 22;
			}
			else {
				unKillAbleFrameID = 0;
			}
		}
		else {
			if (unKillAbleFrameID < 20) {
				return powerLVL < 1 ? iSpriteID + 44 : powerLVL == 2 ? iSpriteID : iSpriteID + 11;
			}
			else if (unKillAbleFrameID < 40) {
				return powerLVL < 1 ? iSpriteID + 55 : powerLVL == 2 ? iSpriteID + 11 : iSpriteID + 22;
			}
			else {
				unKillAbleFrameID = 0;
			}
		}
	}
	return iSpriteID;
}

bool Player::getInLevelAnimation() {
	return inLevelAnimation;
}

void Player::setInLevelAnimation(bool inLevelAnimation) {
	this->inLevelAnimation = inLevelAnimation;
}

int Player::getMoveSpeed()
{
	return moveSpeed;
}

bool Player::getMove()
{
	return bMove;
}

bool Player::getMoveDirection()
{
	return moveDirection;
}

void Player::setNextFallFrameID(int nextFallFrameID)
{
	this->nextFallFrameID = nextFallFrameID;
}

void Player::setCurrentJumpSpeed(float currentJumpSpeed) {
	this->currentJumpSpeed = currentJumpSpeed;
}

void Player::setMoveSpeed(int moveSpeed) {
	this->moveSpeed = moveSpeed;
}

bool Player::getUnkillAble()
{
	return unKillAble;
}

int Player::getNumOfLives() {
	return iNumOfLives;
}

void Player::setNumOfLives(int iNumOfLives) {
	this->iNumOfLives = iNumOfLives;
}

void Player::startMove()
{
	iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
	iTimePassed = Core::coreClock.getElapsedTime().asMilliseconds();
	moveSpeed = 1;
	bMove = true;
	if (Core::getMap()->getUnderWater()) {
		setSpriteID(8);
	}
}

void Player::resetMove() {
	bMove = false;
}

void Player::stopMove()
{
	moveSpeed = 0;
	bMove = false;
	changeMoveDirection = false;
	bSquat = false;
	setSpriteID(1);
}

void Player::setMoveDirection(bool moveDirection)
{
	this->moveDirection = moveDirection;
}

bool Player::getChangeMoveDirection()
{
	return changeMoveDirection;
}

void Player::setChangeMoveDirection()
{
	changeMoveDirection = true;
	this->newMoveDirection = !moveDirection;
}

int Player::getXPos() {
	return (int)fXPos;
}

void Player::setXPos(float fXPos) {
	this->fXPos = fXPos;
}

int Player::getYPos() {
	return (int)fYPos;
}

void Player::setYPos(float fYPos) {
	this->fYPos = fYPos;
}

void Player::setSpringJump(bool springJump)
{
	this->springJump = springJump;
}

int Player::getIDPlayer()
{
	return iIDPlayer;
}

void Player::setIDPlayer(int iIDPlayer)
{
	this->iIDPlayer = iIDPlayer;
}

bool Player::getUseSkill()
{
	return bUseSkill;
}

void Player::setUseSkill(bool bUseSkill)
{
	if (!bUseSkill) {
		this->bUseSkill = false;
		this->pSkill->setTrigger(false);
		return;
	}
	if (Core::coreClock.getElapsedTime().asMilliseconds() - iTimeUsedSkill > iSkillCooldown) {
		this->bUseSkill = bUseSkill;
		this->pSkill->setTrigger(true);
		iTimeUsedSkill = Core::coreClock.getElapsedTime().asMilliseconds();
	}
}

void Player::Save(std::ofstream& outFile) const {
	outFile.write(reinterpret_cast<const char*>(&fXPos), sizeof(fXPos));
	outFile.write(reinterpret_cast<const char*>(&fYPos), sizeof(fYPos));
	outFile.write(reinterpret_cast<const char*>(&powerLVL), sizeof(powerLVL));
	outFile.write(reinterpret_cast<const char*>(&iNumOfLives), sizeof(iNumOfLives));
	outFile.write(reinterpret_cast<const char*>(&iScore), sizeof(iScore));
	outFile.write(reinterpret_cast<const char*>(&iCoins), sizeof(iCoins));
}

void Player::Load(std::ifstream& inFile) {
	inFile.read(reinterpret_cast<char*>(&fXPos), sizeof(fXPos));
	inFile.read(reinterpret_cast<char*>(&fYPos), sizeof(fYPos));
	inFile.read(reinterpret_cast<char*>(&powerLVL), sizeof(powerLVL));
	inFile.read(reinterpret_cast<char*>(&iNumOfLives), sizeof(iNumOfLives));
	inFile.read(reinterpret_cast<char*>(&iScore), sizeof(iScore));
	inFile.read(reinterpret_cast<char*>(&iCoins), sizeof(iCoins));
}
