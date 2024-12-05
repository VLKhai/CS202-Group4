#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Block.h"
#include "AniSprite.h"
#include "Event.h"
#include "IMG.h"
#include "CFG.h"
#include "BlockDebris.h"
#include "Platform.h"

class Map {
private:
	float fXPos, fYPos;

	std::vector<Block*> vBlock;
	int iBlockSize; // Size of vBlock

	std::vector<Block*> vMinion;
	int iMinionSize; // Size of vMinion

	//std::vector<std::vector<MapLevel*>> lMap;
	int iMapWidth, iMapHeight;

	std::vector<BlockDebris*> lBlockDebris;

	std::vector<Platform*> vPlatform;

	//std::vector<LevelText*> vLevelText;

	//std::vector<Bubble*> lBubble;

	int currentLevelID;

	int iLevelType; // 0, 1, 2;

	bool underWater;

	int iSpawnPointID;

	bool bMoveMap;

	int iFrameID;

	int iMapTime;

	bool inEvent;
    Event* pEvent;

    Player* player;

    // ----Load----
    void loadGameData(sf::RenderWindow& mainWindow);
	void loadLVL_1_1();

public:
    Map() = default;
    Map(sf::RenderWindow& mainWindow);

    void update();
	void updateGifBlocks();
	void updatePlayer();

	void draw(sf::RenderWindow& mainWindow);

	void loadLVL();

	// --GETTERS & SETTERS--
	float getXPos();
	void setXPos(float val);

	float getYPos();
	void setYPos(float val);

	int getLevelType();
	void setLevelType(int iLevelType);

	int getCurrentLevelID();
	void setCurrentLevelID(int iID);

	Player* getPlayer();
	Platform* getPlatform(int iID);
	Block* getBlock(int iID);
};