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

	void update();
	void draw(sf::RenderWindow& mainWindow);

	void enter();
	void escape();
	void setKey(int keyID);

	void updateActiveButton(int iDir);

	//void updateVolumeRect();

	void setEscapeToMainMenu(bool escapeToMainMenu);
};

#endif