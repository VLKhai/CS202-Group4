#include "Flag.h"
#include "Core.h"

/* ******************************************** */

Flag::Flag(int iXPos, int iYPos) {
	this->iXPos = iXPos;
	this->iYPos = iYPos;

	this->iYTextPos = CFG::GameHeight - 3 * 32;
	this->iPoints = -1;

	this->iBlockID = Core::getMap()->getLevelType() == 4 || Core::getMap()->getLevelType() == 3 ? 168 : 42;

	this->castleFlagExtraXPos = this->castleFlagY = 0;
}

/* ******************************************** */

void Flag::Update() {
	iYPos += 4;
	iYTextPos -= 4;
}

void Flag::UpdateCastleFlag() {
	if (castleFlagY <= 56)
		castleFlagY += 2;
}

void Flag::draw(sf::RenderWindow& window, IMG* iIMG) {
	iIMG->draw(window, (int)(iXPos + Core::getMap()->getXPos()), iYPos);

	if (iPoints > 0) {
		CFG::getText()->Draw(window, std::to_string(iPoints), (int)(iXPos + Core::getMap()->getXPos() + 42), iYTextPos - 22, 8, 16);
	}
}

void Flag::drawCastleFlag(sf::RenderWindow& window, IMG* iIMG) {
	iIMG->draw(window, (int)(iXPos + Core::getMap()->getXPos() + castleFlagExtraXPos + 7 * 32 - 14), CFG::GameHeight + 14 - 6 * 32 - castleFlagY);
}

/* ******************************************** */