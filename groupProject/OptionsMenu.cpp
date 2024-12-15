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

