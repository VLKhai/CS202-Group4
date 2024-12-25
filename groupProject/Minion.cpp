#include "Minion.h"
#include "Core.h"

/* ******************************************** */

Minion::Minion(void) {
	this->minionState = 0;
	this->iHitBoxX = this->iHitBoxY = 32;

	this->killOtherUnits = false;
	this->minionSpawned = false;
	this->collisionOnlyWithPlayer = false;

	this->moveDirection = true;
	this->moveSpeed = 1;

	this->jumpState = 0;
	this->startJumpSpeed = 6.65f;
	this->currentFallingSpeed = 2.2f;

	this->currentJumpSpeed = 0;
	this->jumpDistance = 0;
	this->currentJumpDistance = 0;

	this->deadTime = -1;

	this->onAnotherMinion = false;
}

/* ******************************************** */

void Minion::Update() { }

void Minion::Draw(sf::RenderWindow& MainWindow, IMG* iIMG) {}

void Minion::drawBoundingBox(sf::RenderWindow& MainWindow)
{
	sf::RectangleShape rect(sf::Vector2f(iHitBoxX, iHitBoxY));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1);
	rect.setPosition((int)fXPos + (int)Core::getMap()->getXPos(), fYPos);
	MainWindow.draw(rect);
}

bool Minion::updateMinion() {
	if (!minionSpawned) {
		Spawn();
	}
	else {
		minionPhysics();
	}

	return minionSpawned;
}

void Minion::minionPhysics() {
	if (jumpState == 1) {
		physicsState1();
	}
	else {
		if (!Core::getMap()->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY, true) && !Core::getMap()->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY, true) && !onAnotherMinion) {
			physicsState2();
		}
		else {
			jumpState = 0;
			onAnotherMinion = false;
		}
	}
}

void Minion::physicsState1() {
	updateYPos(-(int)currentJumpSpeed);
	currentJumpDistance += (int)currentJumpSpeed;

	currentJumpSpeed *= (currentJumpDistance / jumpDistance > 0.75f ? 0.972f : 0.986f);

	if (currentJumpSpeed < 2.5f) {
		currentJumpSpeed = 2.5f;
	}

	if (jumpDistance <= currentJumpDistance) {
		jumpState = 2;
	}
}

void Minion::physicsState2() {
	currentFallingSpeed *= 1.06f;

	if (currentFallingSpeed > startJumpSpeed) {
		currentFallingSpeed = startJumpSpeed;
	}

	updateYPos((int)currentFallingSpeed);

	jumpState = 2;

	if (fYPos >= CFG::GameHeight) {
		minionState = -1;
	}
}

void Minion::updateXPos() {
	// ----- LEFT
	if (moveDirection) {
		if (Core::getMap()->checkCollisionLB((int)fXPos - moveSpeed, (int)fYPos - 2, iHitBoxY, true) || Core::getMap()->checkCollisionLT((int)fXPos - moveSpeed, (int)fYPos + 2, true)) {
			moveDirection = !moveDirection;
			if (killOtherUnits && fXPos > -Core::getMap()->getXPos() - 64 && fXPos < -Core::getMap()->getXPos() + CFG::GameWidth + 64 + iHitBoxX) CFG::getMusic()->PlayChunk(CFG::getMusic()->cBLOCKHIT);
		}
		else {
			fXPos -= (jumpState == 0 ? moveSpeed : moveSpeed / 2.0f);
		}
	}
	// ----- RIGHT
	else {
		if (Core::getMap()->checkCollisionRB((int)fXPos + moveSpeed, (int)fYPos - 2, iHitBoxX, iHitBoxY, true) || Core::getMap()->checkCollisionRT((int)fXPos + moveSpeed, (int)fYPos + 2, iHitBoxX, true)) {
			moveDirection = !moveDirection;
			if (killOtherUnits && fXPos > -Core::getMap()->getXPos() - 64 && fXPos < -Core::getMap()->getXPos() + CFG::GameWidth + 64 + iHitBoxX) CFG::getMusic()->PlayChunk(CFG::getMusic()->cBLOCKHIT);
		}
		else {
			fXPos += (jumpState == 0 ? moveSpeed : moveSpeed / 2.0f);
		}
	}

	if (fXPos < -iHitBoxX) {
		minionState = -1;
	}
}

void Minion::updateYPos(int iN) {
	if (iN > 0) {
		if (!Core::getMap()->checkCollisionLB((int)fXPos + 2, (int)fYPos + iN, iHitBoxY, true) && !Core::getMap()->checkCollisionRB((int)fXPos - 2, (int)fYPos + iN, iHitBoxX, iHitBoxY, true)) {
			fYPos += iN;
		}
		else {
			if (jumpState == 1) {
				jumpState = 2;
			}
			updateYPos(iN - 1);
		}
	}
	else if (iN < 0) {
		if (!Core::getMap()->checkCollisionLT((int)fXPos + 2, (int)fYPos + iN, true) && !Core::getMap()->checkCollisionRT((int)fXPos - 2, (int)fYPos + iN, iHitBoxX, true)) {
			fYPos += iN;
		}
		else {
			if (jumpState == 1) {
				jumpState = 2;
			}
			updateYPos(iN + 1);
		}
	}

	if ((int)fYPos % 2 == 1) {
		fYPos += 1;
	}
}

void Minion::collisionEffect() {
	if (minionSpawned)
		moveDirection = !moveDirection;
}

bool Minion::checkVerticalOverlap(float leftX, float rightX)
{
	return ((fXPos <= leftX && leftX <= fXPos + iHitBoxX) || (fXPos <= rightX && rightX <= fXPos + iHitBoxX) 
		|| (leftX < fXPos && rightX > fXPos+iHitBoxX));
}

bool Minion::checkHorizontalOverlap(float topY, float botY)
{
	return ((fYPos <= topY && topY <= fYPos + iHitBoxY) || (fYPos <= botY && botY <= fYPos + iHitBoxY) 
		|| (topY < fYPos && botY > fYPos+iHitBoxY));
}

bool Minion::checkHorizontalTopOverlap(float botY)
{
	return (fYPos-2 <= botY && botY <= fYPos+16);
}

void Minion::collisionWithPlayer(bool TOP, Player* pPlayer) { }

void Minion::collisionWithAnotherUnit() { };

void Minion::lockMinion() { }

/* ******************************************** */

void Minion::Spawn() {
	if ((fXPos >= -Core::getMap()->getXPos() && fXPos <= -Core::getMap()->getXPos() + CFG::GameWidth) || (fXPos + iHitBoxX >= -Core::getMap()->getXPos() && fXPos + iHitBoxX <= -Core::getMap()->getXPos() + CFG::GameWidth)) {
		minionSpawned = true;
	}
}

void Minion::startJump(int iH) {
	jumpState = 1;
	currentJumpSpeed = startJumpSpeed;
	jumpDistance = 32 * iH + 16.0f;
	currentJumpDistance = 0;
}

void Minion::resetJump() {
	jumpState = 0;
	currentFallingSpeed = 2.7f;
}

void Minion::killMinion()
{
	setMinionState(-2);
}

void Minion::points(int iPoints) {
	Core::getMap()->addPoints((int)fXPos + 7, (int)fYPos, std::to_string(iPoints * Core::getMap()->getPlayer()->getComboPoints()), 8, 16);
	Core::getMap()->getPlayer()->setScore(Core::getMap()->getPlayer()->getScore() + iPoints * Core::getMap()->getPlayer()->getComboPoints());
	Core::getMap()->getPlayer()->addComboPoints();
}

void Minion::minionDeathAnimation() {
	fXPos += (moveDirection ? -1.5f : 1.5f);
	fYPos += 2 * (deadTime > 8 ? -1 : deadTime > 2 ? 1 : 4.25f);

	if (deadTime > 0) {
		--deadTime;
	}
	
	if (fYPos > CFG::GameHeight) {
		minionState = -1;
	}
}

/* ******************************************** */

int Minion::getBloockID() {
	return iBlockID;
}

void Minion::setBlockID(int iBlockID) {
	this->iBlockID = iBlockID;
}

int Minion::getXPos() {
	return (int)fXPos;
}

int Minion::getYPos() {
	return (int)fYPos;
}

void Minion::setYPos(int iYPos) {
	this->fYPos = (float)iYPos;
}

bool Minion::isAlive()
{
	return (deadTime<0);
}

int Minion::getMinionState() {
	return minionState;
}

void Minion::setMinionState(int minionState) {
	this->minionState = minionState;
	if (minionState == -2) {
		deadTime = 16;
		fYPos -= iHitBoxY / 4;
		points(200);
		collisionOnlyWithPlayer = true;
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cSHOT);
	}
}

bool Minion::getPowerUP() { return true; }