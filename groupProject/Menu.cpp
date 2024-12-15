#include "Menu.h"
#include "CFG.h"

Menu::Menu() {
	this->activeMenuOption = 0;
}


Menu::~Menu() {

}

void Menu::update() {

}

void Menu::draw(sf::RenderWindow& window) {
	for (unsigned int i = 0; i < lMO.size(); i++) {
		CFG::getText()->Draw(window, lMO[i]->getText(), lMO[i]->getXPos(), lMO[i]->getYPos());
	}

	CFG::getMenuManager()->getActiveOption()->draw(window, lMO[activeMenuOption]->getXPos() - 32, lMO[activeMenuOption]->getYPos());
}

void Menu::updateActiveButton(int iDir) {
	switch (iDir) {
	case 0:
		if (activeMenuOption - 1 < 0) {
			activeMenuOption = numOfMenuOptions - 1;
		}
		else {
			--activeMenuOption;
		}
		break;
	case 2:
		if (activeMenuOption + 1 >= numOfMenuOptions) {
			activeMenuOption = 0;
		}
		else {
			++activeMenuOption;
		}
		break;
	default:
		break;
	}
}