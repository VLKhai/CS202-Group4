#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Block.h"
#include "AniSprite.h"
#include "Event.h"
#include "IMG.h"
#include "CFG.h"

class Map {
private:
    float fXPos, fYPos;

    std::vector<Block*> vBlock;
    std::vector<Block*> vMinion;

    Event* event;
    Player* player;

    // ----Load----
    void loadGameData(sf::RenderWindow& mainWindow);

public:
    Map();










    Map(sf::RenderWindow& mainWindow);
    Player* getPlayer();

	void draw(sf::RenderWindow& mainWindow);
};