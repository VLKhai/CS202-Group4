#include "Player.h"
#include "Core.h"

Player::Player(sf::RenderWindow &window, float fXPos, float fYPos)
{
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

	// LOAD SPRITE
	std::vector<std::string> tempS;
	std::vector<unsigned int> tempI;

	// ----- 0
	tempS.push_back("mario/mario_death");
	tempI.push_back(0);
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();

	// ----- 1
	tempS.push_back("mario/mario");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 2
	tempS.push_back("mario/mario_move0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 3
	tempS.push_back("mario/mario_move1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 4
	tempS.push_back("mario/mario_move2");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 5
	tempS.push_back("mario/mario_jump");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 6
	tempS.push_back("mario/mario_st");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 7
	tempS.push_back("mario/mario"); // SQUAT
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 8
	tempS.push_back("mario/mario_underwater0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 9
	tempS.push_back("mario/mario_underwater1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 10
	tempS.push_back("mario/mario_end");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 11
	tempS.push_back("mario/mario_end1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();

	// ---------- BIG
	// ----- 12
	tempS.push_back("mario/mario1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 13
	tempS.push_back("mario/mario1_move0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 14
	tempS.push_back("mario/mario1_move1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 15
	tempS.push_back("mario/mario1_move2");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 16
	tempS.push_back("mario/mario1_jump");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 17
	tempS.push_back("mario/mario1_st");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 18
	tempS.push_back("mario/mario1_squat"); // SQUAT
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 19
	tempS.push_back("mario/mario1_underwater0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 20
	tempS.push_back("mario/mario1_underwater1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 21
	tempS.push_back("mario/mario1_end");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 22
	tempS.push_back("mario/mario1_end1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();

	// ----- 23
	tempS.push_back("mario/mario2");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 24
	tempS.push_back("mario/mario2_move0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 25
	tempS.push_back("mario/mario2_move1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 26
	tempS.push_back("mario/mario2_move2");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 27
	tempS.push_back("mario/mario2_jump");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 28
	tempS.push_back("mario/mario2_st");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 29
	tempS.push_back("mario/mario2_squat"); // SQUAT
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 30
	tempS.push_back("mario/mario2_underwater0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 31
	tempS.push_back("mario/mario2_underwater1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 32
	tempS.push_back("mario/mario2_end");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 33
	tempS.push_back("mario/mario2_end1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();

	// ----- 34
	tempS.push_back("mario/mario2s");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 35
	tempS.push_back("mario/mario2s_move0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 36
	tempS.push_back("mario/mario2s_move1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 37
	tempS.push_back("mario/mario2s_move2");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 38
	tempS.push_back("mario/mario2s_jump");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 39
	tempS.push_back("mario/mario2s_st");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 40
	tempS.push_back("mario/mario2s_squat"); // SQUAT
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 41
	tempS.push_back("mario/mario2s_underwater0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 42
	tempS.push_back("mario/mario2s_underwater1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 43
	tempS.push_back("mario/mario2s_end");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 44
	tempS.push_back("mario/mario2s_end1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();

	// ----- 45
	tempS.push_back("mario/mario_s0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 46
	tempS.push_back("mario/mario_s0_move0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 47
	tempS.push_back("mario/mario_s0_move1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 48
	tempS.push_back("mario/mario_s0_move2");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 49
	tempS.push_back("mario/mario_s0_jump");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 50
	tempS.push_back("mario/mario_s0_st");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 51
	tempS.push_back("mario/mario_s0"); // SQUAT
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 52
	tempS.push_back("mario/mario_s0_underwater0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 53
	tempS.push_back("mario/mario_s0_underwater1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 54
	tempS.push_back("mario/mario_s0_end");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 55
	tempS.push_back("mario/mario_s0_end1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();

	// ----- 56
	tempS.push_back("mario/mario_s1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 57
	tempS.push_back("mario/mario_s1_move0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 58
	tempS.push_back("mario/mario_s1_move1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 59
	tempS.push_back("mario/mario_s1_move2");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 60
	tempS.push_back("mario/mario_s1_jump");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 61
	tempS.push_back("mario/mario_s1_st");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 62
	tempS.push_back("mario/mario_s1"); // SQUAT
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 63
	tempS.push_back("mario/mario_s1_underwater0");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 64
	tempS.push_back("mario/mario_s1_underwater1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 65
	tempS.push_back("mario/mario_s1_end");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
	// ----- 66
	tempS.push_back("mario/mario_s1_end1");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();

	// ----- LOAD SPRITE

	// ----- 67
	tempS.push_back("mario/mario_lvlup");
	sMario.push_back(new AniSprite(window,  tempS, tempI, true));
	tempS.clear();
}

void Player::movePlayer() {
	// if player is moving
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
	// not moving, change direction, or squat
	else {
		if (Core::coreClock.getElapsedTime().asMilliseconds() - (50 + 15 * (moveSpeed*5)) > iTimePassed && moveSpeed != 0) {
			--moveSpeed;
			iTimePassed = Core::coreClock.getElapsedTime().asMilliseconds();
			if (jumpState == 0) setMarioSpriteID(6);
		}

		if (changeMoveDirection && moveSpeed <= 1) {
			moveDirection = newMoveDirection;
			changeMoveDirection = false;
			bMove = true;
		}
	}

	if (moveSpeed > 0) {
		if (moveDirection) {
			updateXPos(moveSpeed);
		}
		else {
			updateXPos(-moveSpeed);
		}
		if (!changeMoveDirection && jumpState == 0 && bMove) moveAnimation();
			
	}
	else if (jumpState == 0) {
		setMarioSpriteID(1);
		updateXPos(0);
	}
	else {
		updateXPos(0);
	}

	if (bSquat && powerLVL > 0) {
		setMarioSpriteID(7);
	}
}

// CLOSED
bool Player::checkCollisionBot(int nX, int nY) { 
	Vector2* vLT = getBlockLB(fXPos - Core::getMap()->getXPos() + nX, fYPos + nY);

	if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
		Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 1);
	}

	delete vLT;

	vLT = getBlockRB(fXPos - Core::getMap()->getXPos() + nX, fYPos + nY);

	if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->getUse()) {
		Core::getMap()->blockUse(vLT->getX(), vLT->getY(), Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 1);
	}

	delete vLT;
	return true;
}

bool Player::checkCollisionCenter(int nX, int nY)
{
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

void Player::draw(sf::RenderWindow& window)
{
	sMario[getMarioSpriteID()]->getFrame()->draw(window, (int)fXPos, (int)fYPos, !moveDirection);
}

void Player::update()
{
	playerPhysics();
	movePlayer();
	//std::cout << "Speed: " << moveSpeed << std::endl;
	if (iFrameID > 0) {
		--iFrameID;
	}
	else if (iComboPoints > 1) {
		--iComboPoints;
	}
}

void Player::playerPhysics()
{
	// 1: Jumping, 2: Falling, 0: On ground (jumpState)
	// -1: on platform (onPlatformID)
	if (jumpState == 1) { 
		updateYPos(-int(currentJumpSpeed));
		currentJumpDistance += (int)currentJumpSpeed;

		currentJumpSpeed *= (currentJumpDistance / jumpDistance > 0.75f ? 0.972f : 0.986f);

		if (currentJumpSpeed < 2.5f) {
			currentJumpSpeed = 2.5f;
		}	

		if (!CFG::keySpace && currentJumpDistance > 64 && !springJump) {
			jumpDistance = 16;
			currentJumpDistance = 0;
			currentJumpSpeed = 2.5f;
		}

		if (jumpDistance <= currentJumpDistance) {
			jumpState = 2;
		}
	}
	else { 
		
		if (onPlatformID == -1) { // On ground
			onPlatformID = Core::getMap()->checkCollisionWithPlatform((int)fXPos, (int)fYPos, getHitBoxX(), getHitBoxY());
			if (onPlatformID >= 0) {
				if (Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos() + 2), (int)fYPos + 2, getHitBoxY(), true)
					|| Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos() - 2), (int)fYPos + 2, getHitBoxX(), getHitBoxY(), true)) {
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
			if (Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos() + 2), (int)fYPos + 2, getHitBoxY(), true) 
				|| Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos() - 2), (int)fYPos + 2, getHitBoxX(), getHitBoxY(), true)) {
				onPlatformID = -1;
				resetJump();
			}
			else {
				fYPos += Core::getMap()->getPlatform(onPlatformID)->getMoveY();
				Core::getMap()->getPlatform(onPlatformID)->moveY();
				//if(moveSpeed == 0)
				Core::getMap()->setXPos(Core::getMap()->getXPos() - Core::getMap()->getPlatform(onPlatformID)->getMoveX());

				jumpState = 0;
			}
		}
		else if (!Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos() + 2), (int)fYPos + 2, getHitBoxY(), true) &&
			!Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos() - 2), (int)fYPos + 2, getHitBoxX(), getHitBoxY(), true)) {

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

			 setMarioSpriteID(5);
		}
		else if (jumpState == 2) { // Falling
			resetJump();
		}
		else {
			checkCollisionBot(0, 0);
		}
	}
}

void Player::updateXPos(int iD)
{
	checkCollisionBot(iD, 0);
	//checkCollisionCenter(iD, 0);
	fXPos += iD;
}

void Player::updateYPos(int iD)
{
	fYPos += iD;
}

void Player::moveAnimation()
{
	if (Core::coreClock.getElapsedTime().asMilliseconds() - (65 + moveSpeed * 4) > iMoveAnimationTime) {
		iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
		if (iSpriteID >= 4) {
			setMarioSpriteID(2);
		}
		else {
			++iSpriteID;
		}
	}
}

bool Player::getSquat()
{
	return bSquat;
}

void Player::setSquat(bool bSquat)
{
	return;
}

int Player::getPowerLVL() {
	return powerLVL;
}

void Player::setPowerLVL(int powerLVL) {
	if (powerLVL <= 2) {
		if (this->powerLVL < powerLVL) {
			//CFG::getMusic()->PlayChunk(CFG::getMusic()->cMUSHROOMMEAT);
			//setScore(getScore() + 1000);
			//Core::getMap()->addPoints((int)(fXPos - Core::getMap()->getXPos() + getHitBoxX() / 2), (int)fYPos + 16, "1000", 8, 16);
			inLevelAnimation = true;
			inLevelAnimationFrameID = 0;
			inLevelDownAnimationFrameID = 0;
			inLevelDownAnimation = false;
			this->powerLVL = powerLVL;
		}
		else if (this->powerLVL > 0) {
			//CFG::getMusic()->PlayChunk(CFG::getMusic()->cSHRINK);
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
		//Core::getMap()->addPoints((int)(fXPos - Core::getMap()->getXPos() + getHitBoxX() / 2), (int)fYPos + 16, "1UP", 10, 14);
		//CFG::getMusic()->PlayChunk(CFG::getMusic()->cONEUP);
	}
}

void Player::resetPowerLVL() {
	this->powerLVL = 0;
	this->iSpriteID = 1;
}

void Player::startRun()
{
	currentMaxMove = maxMove;
}

void Player::resetRun()
{
	return;
}

void Player::jump()
{
	if (jumpState == 0) {
		startJump(4);
	}
}

void Player::startJump(int iH)
{
	currentJumpSpeed = startJumpSpeed;
	jumpDistance = 32 * iH + 24.0f;
	currentJumpDistance = 0;
	
	setMarioSpriteID(5); // mario_jump
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


void Player::setMarioSpriteID(int iID)
{
	this->iSpriteID = iID + 11 * powerLVL;
}

int Player::getMarioSpriteID()
{
	return iSpriteID;
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

void Player::startMove()
{
	iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
	iTimePassed = Core::coreClock.getElapsedTime().asMilliseconds();
	moveSpeed = 1;
	bMove = true;
	/*if (Core::getMap()->getUnderWater()) {
		setMarioSpriteID(8);
	}*/
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
	setMarioSpriteID(1);
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


