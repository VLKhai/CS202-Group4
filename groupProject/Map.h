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
#include "Tile.h"

class Map {
private:
	float fXPos, fYPos;

	std::vector<Block*> vBlock;
	int iBlockSize; // Size of vBlock

	std::vector<Block*> vMinion;
	int iMinionSize; // Size of vMinion

	std::vector<std::vector<Tile*>> lMap;
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

	void createMap();

	int getStartBlock();
	int getEndBlock();

	void clearMap();
	void clearMinions();
public:
	Map() = default;
    Map(sf::RenderWindow& mainWindow);

    void update();
	void updateGifBlocks();
	void updatePlayer();

	void draw(sf::RenderWindow& mainWindow);
	void drawMap(sf::RenderWindow& mainWindow);

	void moveMap(int iX, int iY);

	void loadLVL();

	// Structure object
	void structBush(int X, int Y, int iSze);
	void structGrass(int X, int Y, int iSize);
	void structCloud(int X, int Y, int iSize);
	void structGND(int X, int Y, int iWidth, int iHeight);
	void structGND2(int X, int Y, int iSize, bool bDir); // true = LEFT, false = RIGHT
	void structGND2(int X, int Y, int iWidth, int iHeight);

	void structBrick(int X, int Y, int iWidth, int iHeight);
	void struckBlockQ(int X, int Y, int iWidth);
	void struckBlockQ2(int X, int Y, int iWidth);

	void structEnd(int X, int Y, int iHeight);

	void structCastleSmall(int X, int Y);
	void structCastleBig(int X, int Y);
	void structCastleWall(int X, int Y, int iWidth, int iHeight);

	void structPipe(int X, int Y, int iHeight);

	void structT(int X, int Y, int iWidth, int iHeight);
	void structTMush(int X, int Y, int iWidth, int iHeight);

	void structWater(int X, int Y, int iWidth, int iHeight);
	void structLava(int X, int Y, int iWidth, int iHeight);
	void structBridge(int X, int Y, int iWidth);

	void structTree(int X, int Y, int iHeigth, bool BIG);
	void structFence(int X, int Y, int iWidth);
	void structBridge2(int X, int Y, int iWidth);

	void structSeeSaw(int X, int Y, int iWidth);
	void structPlatformLine(int X);


	// --GETTERS & SETTERS--
	float getXPos();
	void setXPos(float val);

	float getYPos();
	void setYPos(float val);

	int getLevelType();
	void setLevelType(int iLevelType);

	int getCurrentLevelID();
	void setCurrentLevelID(int iID);

	bool getMoveMap();
	void setMoveMap(bool bMoveMap);

	Player* getPlayer();
	Platform* getPlatform(int iID);
	Block* getBlock(int iID);

	// --Collision
	Vector2* getBlockID(int nX, int nY);
	bool checkCollision(Vector2* nV, bool checkVisible);

	bool checkCollisionLB(int nX, int nY, int nHitBoxY, bool checkVisible);
	bool checkCollisionRB(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible);

	int checkCollisionWithPlatform(int nX, int nY, int iHitBoxX, int iHitBoxY);

};