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
#include "Coin.h"
#include "Points.h"
#include "Flag.h"
#include "LevelText.h"
#include "Pipe.h"

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

	std::vector<LevelText*> vLevelText;

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
	bool drawLines;

    Player* player;

	// ----- MINIONS -----
	std::vector<std::vector<Minion*>> lMinion;
	int iMinionListSize;

	// ----- POINTS & COIN -----
	std::vector<Coin*> lCoin;
	std::vector<Points*> lPoints;
	std::vector<Pipe*> lPipe;

	Flag* pFlag;

    void createMap();

	void checkSpawnPoint();
	int getNumOfSpawnPoints();
	int getSpawnPointXPos(int iID);
	int getSpawnPointYPos(int iID);

	int getStartBlock();
	int getEndBlock();

	void clearMap();
	void clearLevelText();
	void clearMinions();
	
	// ----Load----
    void loadGameData(sf::RenderWindow& mainWindow);
	void loadLVL_1_1();
	void loadMinionsLVL_1_1();

	void clearPipeEvents();
	void loadPipeEventsLVL_1_1();
	void loadPipeEventsLVL_1_2();
	void loadPipeEventsLVL_1_3();
	void loadPipeEventsLVL_2_1();
	void loadPipeEventsLVL_2_2();
	void loadPipeEventsLVL_3_1();
	void loadPipeEventsLVL_4_1();
	void loadPipeEventsLVL_4_2();
	void loadPipeEventsLVL_5_1();
	void loadPipeEventsLVL_5_2();
	void loadPipeEventsLVL_6_2();
	void loadPipeEventsLVL_7_1();
	void loadPipeEventsLVL_7_2();
	void loadPipeEventsLVL_8_1();
	void loadPipeEventsLVL_8_2();
	void loadPipeEventsLVL_8_4();


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
	void DrawGameLayout(sf::RenderWindow& mainWindow);

	//Add 
	void addText(int X, int Y, std::string sText);
	void addPoints(int X, int Y, std::string sText, int iW, int iH);
	void addGoombas(int iX, int iY, bool moveDirection);

	void lockMinions();
	void clearPlatforms();

	// Block Interaction
	bool blockUse(int nX, int nY, int iBlockID, int POS);

	// Load resources
	void loadLVL();
	void resetGameData();
	void startLevelAnimation();
	void moveMap(int nX, int nY);

	// Structure object
	void structBush(int X, int Y, int iSze);
	void structGrass(int X, int Y, int iSize);
	void structCloud(int X, int Y, int iSize);
	void structGND(int X, int Y, int iWidth, int iHeight);
	void structGND2(int X, int Y, int iSize, bool bDir); // true = LEFT, false = RIGHT
	void structGND2(int X, int Y, int iWidth, int iHeight);
	void structUW1(int X, int Y, int iWidth, int iHeight);

	void structBonus(int X, int Y, int iWidth);
	void structBonusEnd(int X);

	void structPipe(int X, int Y, int iHeight);
	void structPipeVertical(int X, int Y, int iHeight);
	void structPipeHorizontal(int X, int Y, int iWidth);

	void structCoins(int X, int Y, int iWidth, int iHeight);

	void structBrick(int X, int Y, int iWidth, int iHeight);
	void struckBlockQ(int X, int Y, int iWidth);
	void struckBlockQ2(int X, int Y, int iWidth);

	void structEnd(int X, int Y, int iHeight);

	void structCastleSmall(int X, int Y);
	void structCastleBig(int X, int Y);
	void structCastleWall(int X, int Y, int iWidth, int iHeight);

	void setBlockID(int X, int Y, int iBlockID); 

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

	void playerDeath(bool animation, bool instantDeath);

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

	bool getUnderWater();
	void setUnderWater(bool underWater);

	bool getMoveMap();
	void setMoveMap(bool bMoveMap);
	int getMapWidth();

	bool getDrawLines();
	void setDrawLines(bool drawLines);

	int getMapTime();
	void setMapTime(int iMapTime);

	int getListID(int nX);
	std::string getLevelName();
	void setBackgroundColor(sf::RenderWindow& mainWindow);

	bool getInEvent();
	void setInEvent(bool inEvent);
	void setSpawnPointID(int iSpawnPointID);

	void setSpawnPoint();

	Flag* getFlag();
	Player* getPlayer();
	Platform* getPlatform(int iID);
	Block* getBlock(int iID);
	Tile* getMapBlock(int iX, int iY);
	Event* getEvent();
};