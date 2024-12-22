# pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "MenuOption.h"

class Menu {
public:
	Menu();
	~Menu();

	virtual void update();
	virtual void draw(sf::RenderWindow& window);

	virtual void enter() {};
	virtual void escape() {};
	virtual void setBackgroundColor(sf::RenderWindow& window) {};
	virtual void resetActiveOptionID() {};
	virtual void setKey(int keyID) {};


	std::vector<MenuOption*> lMO;
	// ----- ID 
	int activeMenuOption;
	int numOfMenuOptions;
	virtual void updateActiveButton(int iDir);
};