#include "MainMenu.h"
#include "CFG.h"
#include "Core.h"

/* ******************************************** */

MainMenu::MainMenu(void) {
	this->lMO.push_back(new MenuOption("1 PLAYER", 207, 276));
	this->lMO.push_back(new MenuOption("2 PLAYER", 207, 308));
	this->lMO.push_back(new MenuOption("CONTINUE", 207, 340));
	this->lMO.push_back(new MenuOption("OPTIONS", 207, 372));

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

void MainMenu::update() {
	CFG::getMusic()->StopMusic();

	Menu::update();

	Core::getMap()->updateGifBlocks();
}

void MainMenu::draw(sf::RenderWindow& mainWindow) {
	Core::getMap()->drawMap(mainWindow);
	Core::getMap()->getPlayer()->draw(mainWindow);
	Core::getMap()->DrawGameLayout(mainWindow);
	
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
		for (int i = 0, extraX = 0; i < 3; i++) {
			if (i == activeWorldID) {
				CFG::getText()->Draw(mainWindow, std::to_string(i + 1) + "-" + std::to_string(activeSecondWorldID + 1), rSelectWorld.left + 40 * (i + 1) + 40 * i + extraX, rSelectWorld.top + 16 + 24, 16, 255, 255, 255);

				extraX = 32;
			}
			else {
				CFG::getText()->Draw(mainWindow, std::to_string(i + 1), rSelectWorld.left + 40 * (i + 1) + 40 * i + extraX, rSelectWorld.top + 16 + 24, 16, 90, 90, 90);
			}
		}
	}

}

/* ******************************************** */


void MainMenu::enter() {
	switch (activeMenuOption) {
	case 0: case 1: 
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
			Core::getMap()->setNumOfPlayers(activeMenuOption+1);
			selectWorld = false;
			Core::resetKeys();
		}
		break;	
	case 3:
		CFG::getMenuManager()->getOptions()->setEscapeToMainMenu(true);
		CFG::getMenuManager()->resetActiveOptionID(CFG::getMenuManager()->eOptions);
		CFG::getMenuManager()->getOptions()->updateVolumeRect();
		CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eOptions);
		break;
	case 2:
		Map * tem = Core::getMap()->Load("GAME_DATA.bin");
		Core::getMap()->setCurrentLevelID(tem->getCurrentLevelID());
		Core::getMap()->loadLVL();
		Core::getMap()->change(tem);
		CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGame);
		CFG::getMusic()->changeMusic(true, true);
		break;
	}
}


void MainMenu::escape() {
	selectWorld = false;
}

void MainMenu::setBackgroundColor(sf::RenderWindow& window)
{
	Core::getMap()->setBackgroundColor(window);
}

void MainMenu::resetActiveOptionID()
{
	activeMenuOption = 0;
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
			if (activeWorldID < 2) {
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
				activeWorldID = 2;
			}
		}
		break;
	}
}