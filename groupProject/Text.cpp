#include "Text.h"
#include "CFG.h"

/* ******************************************** */

Text::Text() {

	rCrop.left = 0;
	rCrop.top = 0;
	rCrop.width = 8;
	rCrop.height = 8;

	rRect.left = 0;
	rRect.top = 0;
	rRect.width = 16;
	rRect.height = 16;

	this->fontSize = 16;
	this->extraLeft = 0;
	this->nextExtraLeft = 0;
}

Text::~Text() {
	delete FONT;
}

/* ******************************************** */

void Text::Draw(sf::RenderWindow& mainWindow, std::string sText, int X, int Y, int fontSize) {
	if (FONT == nullptr || FONT->getIMG() == nullptr) {
		std::cerr << "Error: FONT or its texture is not initialized." << std::endl;
		return;
	}
	this->fontSize = fontSize;
	this->extraLeft = 0;
	this->nextExtraLeft = 0;

	for (unsigned int i = 0; i < sText.size(); i++) {
		rCrop.left = getPos(sText.at(i));

		rRect.left = X + fontSize * i - extraLeft;
		rRect.top = Y;
		rRect.width = fontSize;
		rRect.height = fontSize;
		FONT->draw(mainWindow, rCrop, rRect);
		extraLeft += nextExtraLeft;
		nextExtraLeft = 0;
	}
}

void Text::Draw(sf::RenderWindow& mainWindow, std::string sText, int X, int Y, int fontSize, int iR, int iG, int iB) {
	this->fontSize = fontSize;
	this->extraLeft = 0;
	this->nextExtraLeft = 0;

	for (unsigned int i = 0; i < sText.size(); i++) {
		FONT->setColor(iR, iG, iB);
		rCrop.left = getPos(sText.at(i));

		rRect.left = X + fontSize * i - extraLeft;
		rRect.top = Y;
		rRect.width = fontSize;
		rRect.height = fontSize;
		FONT->draw(mainWindow, rCrop, rRect);
		extraLeft += nextExtraLeft;
		nextExtraLeft = 0;
		FONT->setColor(255, 255, 255);
	}
}

void Text::DrawCenterX(sf::RenderWindow& mainWindow, std::string sText, int Y, int fontSize, int iR, int iG, int iB) {
	int X = CFG::GameWidth / 2 - getTextWidth(sText, fontSize) / 2;

	Draw(mainWindow, sText, X, Y, fontSize, iR, iG, iB);
}

void Text::Draw(sf::RenderWindow& mainWindow, std::string sText, int X, int Y, int iWidth, int iHeight) {
	for (unsigned int i = 0; i < sText.size(); i++) {
		rCrop.left = getPos(sText.at(i));

		rRect.left = X + iWidth * i - extraLeft;
		rRect.top = Y;
		rRect.width = iWidth;
		rRect.height = iHeight;
		FONT->draw(mainWindow, rCrop, rRect);
	}
}


void Text::DrawWS(sf::RenderWindow& mainWindow, std::string sText, int X, int Y, int iR, int iG, int iB, int fontSize) {
	this->fontSize = fontSize;
	this->extraLeft = 0;
	this->nextExtraLeft = 0;

	for (unsigned int i = 0; i < sText.size(); i++) {
		FONT->setColor(0, 0, 0);
		rCrop.left = getPos(sText.at(i));

		rRect.left = X + fontSize * i - extraLeft - 1;
		rRect.top = Y + 1;
		rRect.width = fontSize;
		rRect.height = fontSize;
		FONT->draw(mainWindow, rCrop, rRect);
		FONT->setColor(255, 255, 255);
		rRect.left = X + fontSize * i - extraLeft + 1;
		rRect.top = Y - 1;
		FONT->draw(mainWindow, rCrop, rRect);
		extraLeft += nextExtraLeft;
		nextExtraLeft = 0;
	}
}

/* ******************************************** */

int Text::getTextWidth(std::string sText, int fontSize) {
	int iOutput = sText.size() * fontSize;
	nextExtraLeft = 0;

	for (unsigned int i = 0; i < sText.size(); i++) {
		checkExtra(sText.at(i));
	}

	iOutput -= nextExtraLeft;

	return iOutput;
}

int Text::getPos(int iChar) {
	if (iChar >= 43 && iChar < 91) {
		checkExtra(iChar);
		return (iChar - 43) * rCrop.width + rCrop.width;
	}

	if (iChar >= 118 && iChar < 123) { // v w x y z
		return (iChar - 70) * rCrop.width + rCrop.width;
	}

	return 0;
}

/* ******************************************** */

void Text::checkExtra(int iChar) {
	switch (iChar) {
	case 44: case 46: case 58: case 59:
		nextExtraLeft += 4 * fontSize / rRect.width;
		break;
	}
}

/* ******************************************** */

void Text::setFont(sf::RenderWindow& mainWindow, std::string fileName) {
	FONT = new IMG(fileName, mainWindow);
	if (FONT == nullptr || FONT->getIMG() == nullptr) {
		std::cerr << "Error: Failed to initialize FONT or load its texture." << std::endl;
	}
}
