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

	if (selectWorld) {
		sf::RectangleShape rectangle;

		// Adjust the rectangle
		rSelectWorld.left += 1;
		rSelectWorld.top += 1;
		rSelectWorld.width -= 2;
		rSelectWorld.height -= 2;
		//filled color 
		rectangle.setFillColor(sf::Color::Black);
		rectangle.setOutlineThickness(1); // Optional: Add a border
		rectangle.setOutlineColor(sf::Color::White); // Optional: Border color
		// Update the rectangle shape to reflect the changes
		rectangle.setPosition(static_cast<float>(rSelectWorld.left), static_cast<float>(rSelectWorld.top));
		rectangle.setSize(sf::Vector2f(static_cast<float>(rSelectWorld.width), static_cast<float>(rSelectWorld.height)));
		mainWindow.draw(rectangle);

		CFG::getText()->Draw(mainWindow, "SELECT WORLD", rSelectWorld.left + rSelectWorld.width / 2 - CFG::getText()->getTextWidth("SELECT WORLD") / 2, rSelectWorld.top + 16, 16, 255, 255, 255);
		rSelectWorld.left -= 1;
		rSelectWorld.top -= 1;
		rSelectWorld.width += 2;
		rSelectWorld.height += 2;
		for (int i = 0, extraX = 0; i < 8; i++) {
			if (i == activeWorldID) {
				CFG::getText()->Draw(mainWindow, std::to_string(i + 1) + "-" + std::to_string(activeSecondWorldID + 1), rSelectWorld.left + 16 * (i + 1) + 16 * i + extraX, rSelectWorld.top + 16 + 24, 16, 255, 255, 255);

				extraX = 32;
			}
			else {
				CFG::getText()->Draw(mainWindow, std::to_string(i + 1), rSelectWorld.left + 16 * (i + 1) + 16 * i + extraX, rSelectWorld.top + 16 + 24, 16, 90, 90, 90);
			}
		}
	}

}

/* ******************************************** */


void MainMenu::enter() {
	switch (activeMenuOption) {
	case 0:
		if (!selectWorld) {
			selectWorld = true;
		}
		else {
			CFG::getMenuManager()->getLoadingMenu()->updateTime();
			Core::getMap()->resetGameData();
			Core::getMap()->setCurrentLevelID(activeWorldID * 4 + activeSecondWorldID);
			CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGameLoading);
			CFG::getMenuManager()->getLoadingMenu()->loadingType = true;
			Core::getMap()->setSpawnPointID(0);
			selectWorld = false;
		}
		break;	
	case 1:
		//CFG::getMenuManager()->getOptions()->setEscapeToMainMenu(true);
		CFG::getMenuManager()->resetActiveOptionID(CFG::getMenuManager()->eOptions);
		//CFG::getMenuManager()->getOptions()->updateVolumeRect();
		CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eOptions);
		break;
	case 2:
		//CFG::getMenuManager()->getAboutMenu()->updateTime();
		CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eAbout);
		//CFG::getMusic()->PlayMusic(CFG::getMusic()->mOVERWORLD);
		break;
	}
}


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