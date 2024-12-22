#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H
#include "Menu.h"

class MainMenu : public Menu
{
private:
	bool selectWorld;
	int activeWorldID, activeSecondWorldID;

	sf::IntRect rSelectWorld;
public:
	MainMenu(void);
	~MainMenu(void);

	void update();
	void draw(sf::RenderWindow& mainWindow);

	void enter();
	void escape();
	void setBackgroundColor(sf::RenderWindow& window);
	void resetActiveOptionID();

	void updateActiveButton(int iDir);
};

#endif