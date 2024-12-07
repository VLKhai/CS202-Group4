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
#include "Minion.h"
#include "Goombas.h"

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

	// ----- MINIONS -----
	std::vector<std::vector<Minion*>> lMinion;
	int iMinionListSize;

    // ----Load----
    void loadGameData(sf::RenderWindow& mainWindow);
	void loadLVL_1_1();
	void loadMinionsLVL_1_1();

	void createMap();

	int getStartBlock();
	int getEndBlock();

	void clearMap();
	void clearMinions();
	void clearPlatforms();
public:
	Map() = default;
    Map(sf::RenderWindow& mainWindow);

    void update();
	void updateGifBlocks();
	void updatePlayer();
	void updateMinionBlocks();
	void updateMinions();
	void updateMinionsCollisions();

	void draw(sf::RenderWindow& mainWindow);
	void drawMap(sf::RenderWindow& mainWindow);
	void DrawMinions(sf::RenderWindow& mainWindow);

	//Add minions
	void addGoombas(int iX, int iY, bool moveDirection);

	// Block Interaction
	bool blockUse(int nX, int nY, int iBlockID, int POS);

	// Load resources
	void loadLVL();

	void moveMap(int nX, int nY);

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

	// --<COLLISION>--
	bool checkCollision(Vector2* nV, bool checkVisible);
	// LEFT
	bool checkCollisionLB(int nX, int nY, int nHitBoxY, bool checkVisible);
	bool checkCollisionLT(int nX, int nY, bool checkVisible);
	// CENTER
	bool checkCollisionLC(int nX, int nY, int nHitBoxY, bool checkVisible);
	bool checkCollisionRC(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible);
	// RIGHT
	bool checkCollisionRB(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible);
	bool checkCollisionRT(int nX, int nY, int nHitBoxX, bool checkVisible);

	int checkCollisionWithPlatform(int nX, int nY, int iHitBoxX, int iHitBoxY);

	void checkCollisionOnTopOfTheBlock(int nX, int nY);

	Vector2* getBlockID(int nX, int nY);
	int getBlockIDX(int nX);
	int getBlockIDY(int nY);

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

	int getListID(int nX);

	Player* getPlayer();
	Platform* getPlatform(int iID);
	Block* getBlock(int iID);
	Tile* getMapBlock(int iX, int iY);
};