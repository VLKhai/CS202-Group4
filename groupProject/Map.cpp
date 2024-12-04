#include "Map.h"

void Map::loadGameData(sf::RenderWindow& mainWindow)
{

}

Map::Map() {}

Map::Map(sf::RenderWindow& mainWindow)
{
	player = new Player(mainWindow, 84, 350);
}

Player* Map::getPlayer()
{
    return player;
}

void Map::draw(sf::RenderWindow& mainWindow)
{
}
