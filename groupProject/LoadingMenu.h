#pragma once

#ifndef LOADINGMENU_H
#define LOADINGMENU_H

#include "Menu.h"

class LoadingMenu : public Menu
{
private:
	unsigned int iTime;
public:
	LoadingMenu(void);
	~LoadingMenu(void);

	void update();
	void draw(sf::RenderWindow& window);

	void updateTime();

	void setBackgroundColor(sf::RenderWindow& window);
	void updateActiveButton(int iDir) {};

	bool loadingType; // -- true = LOADING, false = GAME OVER
};

#endif