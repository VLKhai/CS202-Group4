#include "OptionsMenu.h"
#include "CFG.h"
#include "Core.h"

/* ******************************************** */

OptionsMenu::OptionsMenu() {
	rRect.left = 58;
	rRect.top = 48;
	rRect.width = 403;
	rRect.height = 324;

	this->lMO.push_back(new MenuOption("VOLUME", 73, 65));
	this->lMO.push_back(new MenuOption("LEFT", 73, 89));
	this->lMO.push_back(new MenuOption("DOWN", 73, 113));
	this->lMO.push_back(new MenuOption("RIGHT", 73, 137));
	this->lMO.push_back(new MenuOption("JUMP", 73, 161));
	this->lMO.push_back(new MenuOption("RUN", 73, 185));
	this->lMO.push_back(new MenuOption("CAN MOVE BACKWARD", 73, 209));
	this->lMO.push_back(new MenuOption("MAIN MENU", 73, 257));

	this->numOfMenuOptions = lMO.size();

	this->inSetKey = this->resetSetKey = false;

	rSetKeyRect.left = 75;
	rSetKeyRect.top = 284;
	rSetKeyRect.width = 369;
	rSetKeyRect.height = 71;

	rVolumeBG.left = 185;
	rVolumeBG.top = 65;
	rVolumeBG.height = 16;
	rVolumeBG.width = 200;

	rVolume.left = 185;
	rVolume.top = 65;
	rVolume.height = 16;
	rVolume.width = 100;

	this->escapeToMainMenu = true;
}

OptionsMenu::~OptionsMenu() {

}

void OptionsMenu::Draw(sf::RenderWindow& mainWindow) {
	sf::RectangleShape rectangle;
	//filled color 
	rectangle.setFillColor(sf::Color::Black);
	rectangle.setOutlineThickness(1); // Optional: Add a border
	rectangle.setOutlineColor(sf::Color::White); // Optional: Border color
	// Update the rectangle shape to reflect the changes
	rectangle.setPosition((rRect.left), (rRect.top));
	rectangle.setSize(sf::Vector2f((rRect.width),(rRect.height)));
	mainWindow.draw(rectangle);
    // Render menu options
    for (unsigned int i = 0; i < lMO.size(); i++) {
        if (i == activeMenuOption) {
            CFG::getText()->Draw(mainWindow, lMO[i]->getText(), lMO[i]->getXPos(), lMO[i]->getYPos(), 16, 255, 255, 255);
        }
        else {
            CFG::getText()->Draw(mainWindow, lMO[i]->getText(), lMO[i]->getXPos(), lMO[i]->getYPos(), 16, 90, 90, 90);
        }
    }
	CFG::getText()->Draw(mainWindow, CFG::getKeyString(CFG::keyIDA), 185, 89, 16, activeMenuOption == 1 ? 255 : 90, activeMenuOption == 1 ? 255 : 90, activeMenuOption == 1 ? 255 : 90);
	CFG::getText()->Draw(mainWindow, CFG::getKeyString(CFG::keyIDS), 185, 113, 16, activeMenuOption == 2 ? 255 : 90, activeMenuOption == 2 ? 255 : 90, activeMenuOption == 2 ? 255 : 90);
	CFG::getText()->Draw(mainWindow, CFG::getKeyString(CFG::keyIDD), 185, 137, 16, activeMenuOption == 3 ? 255 : 90, activeMenuOption == 3 ? 255 : 90, activeMenuOption == 3 ? 255 : 90);
	CFG::getText()->Draw(mainWindow, CFG::getKeyString(CFG::keyIDSpace), 185, 161, 16, activeMenuOption == 4 ? 255 : 90, activeMenuOption == 4 ? 255 : 90, activeMenuOption == 4 ? 255 : 90);
	CFG::getText()->Draw(mainWindow, CFG::getKeyString(CFG::keyIDShift), 185, 185, 16, activeMenuOption == 5 ? 255 : 90, activeMenuOption == 5 ? 255 : 90, activeMenuOption == 5 ? 255 : 90);

	CFG::getText()->Draw(mainWindow, CFG::canMoveBackward ? "TRUE" : "FALSE", 357, 209, 16, activeMenuOption == 6 ? 255 : 90, activeMenuOption == 6 ? 255 : 90, activeMenuOption == 6 ? 255 : 90);
   
	if (inSetKey) {
		sf::RectangleShape rec;
		//filled color 
		rec.setFillColor(sf::Color::Black);
		rec.setOutlineThickness(1); // Optional: Add a border
		rec.setOutlineColor(sf::Color::White); // Optional: Border color
		// Update the rectangle shape to reflect the changes
		rec.setPosition((rSetKeyRect.left), (rSetKeyRect.top));
		rec.setSize(sf::Vector2f((rSetKeyRect.width), (rSetKeyRect.height)));
		mainWindow.draw(rec);
		CFG::getText()->Draw(mainWindow, "PRESS KEY FOR " + lMO[activeMenuOption]->getText(), 92, rSetKeyRect.top + 16, 16, 255, 255, 255);
		CFG::getText()->Draw(mainWindow, "PRESS ESC TO CANCEL", 92, rSetKeyRect.top + 40, 16, 255, 255, 255);
	}
}

void OptionsMenu::updateActiveButton(int iDir) {
	if (activeMenuOption == 0 && (iDir == 1 || iDir == 3)) {
		switch (iDir) {
		case 1:
			if (CFG::getMusic()->getVolume() < 100) {
				CFG::getMusic()->setVolume(CFG::getMusic()->getVolume() + 5);
			}
			else {
				CFG::getMusic()->setVolume(100);
			}
			break;
		case 3:
			if (CFG::getMusic()->getVolume() > 0) {
				CFG::getMusic()->setVolume(CFG::getMusic()->getVolume() - 5);
			}
			else {
				CFG::getMusic()->setVolume(0);
			}
			break;
		}
		//updateVolumeRect();
		//CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);
	}
	if (!inSetKey) {
		Menu::updateActiveButton(iDir);
	}
}

void OptionsMenu::Update() {
	if (resetSetKey) {
		inSetKey = false;
		resetSetKey = false;
	}
}

void OptionsMenu::enter() {
	switch (activeMenuOption) {
	case 0:
		//CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);
		break;
	case 1: case 2: case 3: case 4: case 5:
		inSetKey = true;
		break;
	case 6:
		CFG::canMoveBackward = !CFG::canMoveBackward;
		break;
	case 7:
		Core::getMap()->resetGameData();
		CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eMainMenu);
		break;
	}
}

void OptionsMenu::setKey(int keyID) {
	if (inSetKey && keyID != sf::Keyboard::Enter && keyID != sf::Keyboard::Return && keyID != sf::Keyboard::Escape) {
		switch (activeMenuOption) {
		case 1:
			CFG::keyIDA = keyID;
			if (CFG::keyIDD == keyID) CFG::keyIDD = 0;
			if (CFG::keyIDS == keyID) CFG::keyIDS = 0;
			if (CFG::keyIDSpace == keyID) CFG::keyIDSpace = 0;
			if (CFG::keyIDShift == keyID) CFG::keyIDShift = 0;
			break;
		case 2:
			CFG::keyIDS = keyID;
			if (CFG::keyIDA == keyID) CFG::keyIDA = 0;
			if (CFG::keyIDD == keyID) CFG::keyIDD = 0;
			if (CFG::keyIDSpace == keyID) CFG::keyIDSpace = 0;
			if (CFG::keyIDShift == keyID) CFG::keyIDShift = 0;
			break;
		case 3:
			CFG::keyIDD = keyID;
			if (CFG::keyIDA == keyID) CFG::keyIDA = 0;
			if (CFG::keyIDS == keyID) CFG::keyIDS = 0;
			if (CFG::keyIDSpace == keyID) CFG::keyIDSpace = 0;
			if (CFG::keyIDShift == keyID) CFG::keyIDShift = 0;
			break;
		case 4:
			CFG::keyIDSpace = keyID;
			if (CFG::keyIDA == keyID) CFG::keyIDA = 0;
			if (CFG::keyIDS == keyID) CFG::keyIDS = 0;
			if (CFG::keyIDD == keyID) CFG::keyIDD = 0;
			if (CFG::keyIDShift == keyID) CFG::keyIDShift = 0;
			break;
		case 5:
			CFG::keyIDShift = keyID;
			if (CFG::keyIDA == keyID) CFG::keyIDA = 0;
			if (CFG::keyIDS == keyID) CFG::keyIDS = 0;
			if (CFG::keyIDD == keyID) CFG::keyIDD = 0;
			if (CFG::keyIDSpace == keyID) CFG::keyIDSpace = 0;
			break;
		}
		resetSetKey = true;
	}
	else if (keyID == sf::Keyboard::Escape) {
		resetSetKey = true;
	}
}
