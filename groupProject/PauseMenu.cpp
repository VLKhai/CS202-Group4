#include "PauseMenu.h"
#include "CFG.h"
#include "Core.h"

/* ******************************************** */

PauseMenu::PauseMenu(void) {
	rPause.left = 220;
	rPause.top = 120;
	rPause.width = 560;
	rPause.height = 180;

	this->lMO.push_back(new MenuOption("RESUME", 0, 156));
	this->lMO.push_back(new MenuOption("OPTIONS", 0, 180));

	this->lMO.push_back(new MenuOption("QUIT TO MENU", 0, 228));
	this->lMO.push_back(new MenuOption("QUIT TO DESKTOP", 0, 252));

	this->numOfMenuOptions = lMO.size();
}

PauseMenu::~PauseMenu(void) {

}

/* ******************************************** */

void PauseMenu::update() {

}

void PauseMenu::draw(sf::RenderWindow& window) {
	Core::getMap()->drawMap(window);
	Core::getMap()->DrawMinions(window);
	Core::getMap()->getPlayer()->draw(window);
	Core::getMap()->DrawGameLayout(window);
	
	rPause.left += 1;
	rPause.top += 1;
	rPause.height -= 2;
	rPause.width -= 2;
	sf::RectangleShape rectangle;
	//filled color 
	rectangle.setFillColor(sf::Color::Black);
	rectangle.setOutlineThickness(1); // Optional: Add a border
	rectangle.setOutlineColor(sf::Color::White); // Optional: Border color
	// Update the rectangle shape to reflect the changes
	rectangle.setPosition((rPause.left), (rPause.top));
	rectangle.setSize(sf::Vector2f((rPause.width), (rPause.height)));
	window.draw(rectangle);
	rPause.left -= 1;
	rPause.top -= 1;
	rPause.height += 2;
	rPause.width += 2;
	for (unsigned int i = 0; i < lMO.size(); i++) {
		if (i == activeMenuOption) {
			CFG::getText()->DrawCenterX(window, lMO[i]->getText(), lMO[i]->getYPos(), 16, 255, 255, 255);
		}
		else {
			CFG::getText()->DrawCenterX(window, lMO[i]->getText(), lMO[i]->getYPos(), 16, 90, 90, 90);
		}
	}
}


/* ******************************************** */

void PauseMenu::enter() {
	switch (activeMenuOption) {
	case 0:
		CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGame);
		CFG::getMusic()->PlayMusic();
		break;
	case 1:
		CFG::getMenuManager()->getOptions()->setEscapeToMainMenu(false);
		CFG::getMenuManager()->resetActiveOptionID(CFG::getMenuManager()->eOptions);
		//CFG::getMenuManager()->getOptions()->updateVolumeRect();
		CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eOptions);
		break;
	case 2:
		Core::getMap()->Save("GAME_DATA.bin");
		Core::getMap()->resetGameData();
		CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eMainMenu);
		break;
	case 3:
		Core::quitGame = true;
		break;
	}
}

void PauseMenu::escape() {
	CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGame);
	CFG::getMusic()->PauseMusic();
}

void PauseMenu::resetActiveOptionID()
{
	activeMenuOption = 0;
}

void PauseMenu::updateActiveButton(int iDir) {
	Menu::updateActiveButton(iDir);
}