#include "MainMenu.h"
#include "CFG.h"
#include "Core.h"

/* ******************************************** */

MainMenu::MainMenu(void) {
	this->lMO.push_back(new MenuOption("1 PLAYER GAME", 178, 276));
	this->lMO.push_back(new MenuOption("OPTIONS", 222, 308));
	this->lMO.push_back(new MenuOption("ABOUT", 237, 340));

	this->numOfMenuOptions = lMO.size();

	this->selectWorld = false;

	rSelectWorld.left = 122;
	rSelectWorld.top = 280;
	rSelectWorld.width = 306;
	rSelectWorld.height = 72;

	this->activeWorldID = this->activeSecondWorldID = 0;
}

MainMenu::~MainMenu(void) {

}

/* ******************************************** */

void MainMenu::Update() {
	//CFG::getMusic()->StopMusic();

	Menu::update();
}

void MainMenu::Draw(sf::RenderWindow& mainWindow) {
	CFG::getSMBLOGO()->draw(mainWindow, 80, 48);
	Menu::draw(mainWindow);
	//CFG::getText()->Draw(mainWindow, "OK", 4, CFG::GameHeight -200, 8, 255, 255, 255);
	//CFG::getText()->Draw(mainWindow, "OK NHA", 5, CFG::GameHeight-100, 8, 255, 255, 255);
}

/* ******************************************** */



void MainMenu::escape() {
	selectWorld = false;
}

void MainMenu::updateActiveButton(int iDir) {
	switch (iDir) {
	case 0: case 2:
		if (!selectWorld) {
			Menu::updateActiveButton(iDir);
		}
		else {
			switch (iDir) {
			case 0:
				if (activeSecondWorldID < 1) {
					activeSecondWorldID = 3;
				}
				else {
					--activeSecondWorldID;
				}
				break;
			case 2:
				if (activeSecondWorldID > 2) {
					activeSecondWorldID = 0;
				}
				else {
					++activeSecondWorldID;
				}
				break;
			}
		}
		break;
	case 1:
		if (selectWorld) {
			if (activeWorldID < 7) {
				++activeWorldID;
			}
			else {
				activeWorldID = 0;
			}
		}
		break;
	case 3:
		if (selectWorld) {
			if (activeWorldID > 0) {
				--activeWorldID;
			}
			else {
				activeWorldID = 7;
			}
		}
		break;
	}
}