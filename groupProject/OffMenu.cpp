#include "OffMenu.h"
#include "Core.h"

void OffMenu::update()
{
	Core::getMap()->update();
	Core::getMap()->updateMinionsCollisions();
}

void OffMenu::draw(sf::RenderWindow& window)
{	
	Core::getMap()->draw(window);
}

void OffMenu::enter()
{
	Core::getMap()->setDrawLines(!Core::getMap()->getDrawLines());
}

void OffMenu::setBackgroundColor(sf::RenderWindow& window)
{
	Core::getMap()->setBackgroundColor(window);
}
