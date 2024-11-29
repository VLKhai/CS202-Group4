# pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Menu {
public:
	Menu();
	~Menu();

	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window);
};