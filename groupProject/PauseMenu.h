#pragma once

#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "Menu.h"

class PauseMenu : public Menu
{
private:
	sf::IntRect rPause;
public:
	PauseMenu(void);
	~PauseMenu(void);

	void update();
	void draw(sf::RenderWindow& window);

	void enter();
	void escape();

	void resetActiveOptionID();
	void updateActiveButton(int iDir);
};

#endif