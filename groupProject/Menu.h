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

	std::vector<MenuOption*> lMO;
	// ----- ID 
	int activeMenuOption;
	int numOfMenuOptions;
	virtual void updateActiveButton(int iDir);
};