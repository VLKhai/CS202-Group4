#pragma once

#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "Menu.h"

class OptionsMenu : public Menu
{
private:
	sf::IntRect rRect;
	sf::IntRect rSetKeyRect;
	sf::IntRect rVolumeBG;
	sf::IntRect rVolume;

	bool inSetKey, resetSetKey;

	bool escapeToMainMenu;
public:
	OptionsMenu();
	~OptionsMenu();

};

#endif