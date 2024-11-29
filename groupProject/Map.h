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

public:
    Map();
    Player* getPlayer();
};