#include "Player.h"
#include "Core.h"

Player::Player(sf::RenderWindow &window, float fXPos, float fYPos)
{
	this->fXPos = fXPos;
	this->fYPos = fYPos;
	this->iSpriteID = 1;

	// ---<MOVE>---
	this->moveDirection = true;
	this->currentMaxMove = 4;
	this->moveSpeed = 0;
	this->bMove = false;
	this->changeMoveDirection = false;
	this->bSquat = false;
	
	this->onPlatformID = -1;

	// ---<JUMP>---
	this->jumpState = 0;
	this->startJumpSpeed = 7.95f;
	this->currentFallingSpeed = 2.81f;

	// ---<STUFF>---
	this->powerLVL = 0;

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

bool Player::checkCollisionBot(int nX, int nY)
{
	if (nY >= 350) return true;
	return false;
}

bool Player::checkCollisionCenter(int nX, int nY)
{
	return true;
}

void Player::draw(sf::RenderWindow& window)
{
	sMario[getMarioSpriteID()]->getFrame()->draw(window, (float)fXPos, (float)fYPos + 2, !moveDirection);
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
	if (jumpState == 1) {
		updateYPos(-int(currentJumpSpeed));
		currentJumpDistance += (int)currentJumpSpeed;

		currentJumpSpeed *= (currentJumpDistance / jumpDistance > 0.75f ? 0.972f : 0.986f); //986

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
		if (onPlatformID >= 0) {}
		else { //onPlatformID = -1
			if (nextFallFrameID > 0) {
				--nextFallFrameID;
			}
			else if (!checkCollisionBot((int)fXPos, (int)fYPos)){
				currentFallingSpeed *= 1.05f;
				std::cout << currentFallingSpeed << " " << startJumpSpeed << std::endl;
				if (currentFallingSpeed > startJumpSpeed) {
					currentFallingSpeed = startJumpSpeed ;
				}

				updateYPos((int)currentFallingSpeed);
				jumpState = 2;
				setMarioSpriteID(5);
			}
			else if (jumpState == 2) {
				resetJump();
				onPlatformID = -1;
			}
			else {}

		}
	}
}

void Player::updateXPos(int iD)
{
	checkCollisionBot(iD, 0);
	checkCollisionCenter(iD, 0);
	fXPos += iD;

	if (iD > 0) {
		if (fXPos >= 416 && Core::getMap()->getMoveMap()) {
			Core::getMap()->moveMap(-iD*3, 0);
		}
		else {
			fXPos += iD;
		}
		updateXPos(iD - 1);
		if (moveSpeed > 1 && jumpState == 0) --moveSpeed;
	}
	else {}
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
	/*if (CCore::getMap()->getUnderWater()) {
		setMarioSpriteID(8);
	}*/
}

void Player::resetMove() {
	--moveSpeed;
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


