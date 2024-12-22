#pragma once

#include "Menu.h"

class OffMenu : public Menu
{
private:
public:

	void update();

	void draw(sf::RenderWindow& window);

	void enter();
	void setBackgroundColor(sf::RenderWindow& window);
	void updateActiveButton(int iDir) {};
};