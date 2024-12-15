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

	if (selectWorld) {
		// Draw filled rectangle with transparency
		sf::RectangleShape filledRect(sf::Vector2f(rSelectWorld.width, rSelectWorld.height));
		filledRect.setPosition(static_cast<float>(rSelectWorld.left), static_cast<float>(rSelectWorld.top));
		filledRect.setFillColor(sf::Color(4, 4, 4, 235)); // Dark gray with transparency
		mainWindow.draw(filledRect);

		// Draw rectangle outline
		sf::RectangleShape outlineRect(sf::Vector2f(rSelectWorld.width - 2, rSelectWorld.height - 2));
		outlineRect.setPosition(static_cast<float>(rSelectWorld.left + 1), static_cast<float>(rSelectWorld.top + 1));
		outlineRect.setFillColor(sf::Color::Transparent);
		outlineRect.setOutlineColor(sf::Color(255, 255, 255, 255)); // White color
		outlineRect.setOutlineThickness(1.0f);
		mainWindow.draw(outlineRect);

		// Draw "SELECT WORLD" text
		int textX = rSelectWorld.left + rSelectWorld.width / 2 - CFG::getText()->getTextWidth("SELECT WORLD") / 2;
		int textY = rSelectWorld.top + 16;
		CFG::getText()->Draw(mainWindow, "SELECT WORLD", textX, textY, 16, 255, 255, 255);

		// Loop to draw worlds
		int extraX = 0;
		for (int i = 0; i < 8; i++) {
			if (i == activeWorldID) {
				std::string worldLabel = std::to_string(i + 1) + "-" + std::to_string(activeSecondWorldID + 1);
				CFG::getText()->Draw(mainWindow, worldLabel,
					rSelectWorld.left + 16 * (i + 1) + 16 * i + extraX,
					rSelectWorld.top + 16 + 24,
					16, 255, 255, 255);

				extraX = 32;

				// Uncomment to draw sub-worlds if needed
				/*
				for (int j = 0; j < 4; j++) {
					std::string subWorldLabel = std::to_string(j + 1);
					sf::Color textColor = (j == activeSecondWorldID) ? sf::Color(255, 255, 255, 255) : sf::Color(90, 90, 90, 255);
					CCFG::getText()->Draw(mainWindow, subWorldLabel,
										  rSelectWorld.left + 16 * (i + 1) + 16 * i,
										  rSelectWorld.top + 40 + 24 * j,
										  16, textColor.r, textColor.g, textColor.b);
				}
				*/
			}
			else {
				std::string worldLabel = std::to_string(i + 1);
				CFG::getText()->Draw(mainWindow, worldLabel,
					rSelectWorld.left + 16 * (i + 1) + 16 * i + extraX,
					rSelectWorld.top + 16 + 24,
					16, 90, 90, 90);
			}
		}

		// Set background color (using SFML-based function)
		Core::getMap()->setBackgroundColor(mainWindow);
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
			//Core::getMap()->resetGameData();
			Core::getMap()->setCurrentLevelID(activeWorldID * 4 + activeSecondWorldID);
			CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGameLoading);
			CFG::getMenuManager()->getLoadingMenu()->loadingType = true;
			//Core::getMap()->setSpawnPointID(0);
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