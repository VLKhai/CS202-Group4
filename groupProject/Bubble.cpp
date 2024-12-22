#include "Bubble.h"
#include "Core.h"
#include "CFG.h"

/* ******************************************** */

Bubble::Bubble(int iXPos, int iYPos) {
	this->iXPos = iXPos;
	this->iYPos = iYPos;

	this->bDestoy = false;

	this->iBlockID = 96;
}

Bubble::~Bubble(void) {

}

/* ******************************************** */

void Bubble::Update() {
	if (iYPos < CFG::GameHeight - 12 * 32 - 8) {
		bDestoy = true;
	}
	else {
		iYPos -= 1;
	}
}

void Bubble::Draw(sf::RenderWindow& mainWindow, IMG* iIMG) {
	iIMG->draw(mainWindow, (int)(iXPos + Core::getMap()->getXPos()), (int)(iYPos));
}

/* ******************************************** */

int Bubble::getBlockID() {
	return iBlockID;
}

bool Bubble::getDestroy() {
	return bDestoy;
}