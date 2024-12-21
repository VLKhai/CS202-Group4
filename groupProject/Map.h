#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Mario.h"
#include "Luigi.h"
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
#include "Koppa.h"
#include "Beetle.h"
#include "Bowser.h"
#include "BulletBill.h"
#include "BulletBillSpawner.h"
#include "Cheep.h"
#include "CheepSpawner.h"
#include "Fire.h"
#include "Flower.h"
#include "Hammer.h"
#include "HammerBro.h"
#include "Lakito.h"
#include "Mushroom.h"
#include "Plant.h"
#include "Spikey.h"
#include "Star.h"
#include "Toad.h"
#include "Squid.h"


class Map {
private:
	friend class Core;
	float fXPos, fYPos; // Coordinate of the view window in the virtual world

	std::vector<Block*> vBlock;
	int iBlockSize; // Size of vBlock

	std::vector<Block*> vMinion;
	int iMinionSize; // Size of vBlock

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

	// ----- PLAYER -----
	std::vector<Player*> vPlayer;
	int indexPlayer;
	Player* pPlayer;

	// ----- MINIONS -----
	std::vector<std::vector<Minion*>> lMinion;
	int iMinionListSize;

	int getListID(int nXPos);

	int getNumOfMinions();
	// ----- MINIONS -----

	// ----- POINTS & COIN -----

	std::vector<Coin*> lCoin;
	std::vector<Points*> lPoints;

	// ----- POINTS & COIN -----

	// ----- PIPEEVENTS -----

	std::vector<Pipe*> lPipe;

	// ----- ENDEVENTS

	Flag* pFlag;

	bool drawLines;

	// ---------- Methods ----------

	int getStartBlock();
	int getEndBlock();


	void checkSpawnPoint();
	int getNumOfSpawnPoints();
	int getSpawnPointXPos(int iID);
	int getSpawnPointYPos(int iID);

	// ----- Load -----
	void loadMap(sf::RenderWindow& mainWindow);
	void loadGameData(sf::RenderWindow& window);
	void createMap();


	void loadLVL_1_1();
	void loadLVL_1_2();
	void loadLVL_1_3();
	void loadLVL_1_4();
	void loadLVL_2_1();
	void loadLVL_2_2();
	void loadLVL_2_3();
	void loadLVL_2_4();
	void loadLVL_3_1();
	void loadLVL_3_2();
	void loadLVL_3_3();
	void loadLVL_3_4();
	void loadLVL_4_1();
	void loadLVL_4_2();
	void loadLVL_4_3();
	void loadLVL_4_4();
	void loadLVL_5_1();
	void loadLVL_5_2();
	void loadLVL_5_3();
	void loadLVL_5_4();
	void loadLVL_6_1();
	void loadLVL_6_2();
	void loadLVL_6_3();
	void loadLVL_6_4();
	void loadLVL_7_1();
	void loadLVL_7_2();
	void loadLVL_7_3();
	void loadLVL_7_4();
	void loadLVL_8_1();
	void loadLVL_8_2();
	void loadLVL_8_3();
	void loadLVL_8_4();

	void loadMinionsLVL_1_1();
	void loadMinionsLVL_1_2();
	void loadMinionsLVL_1_3();
	void loadMinionsLVL_1_4();
	void loadMinionsLVL_2_1();
	void loadMinionsLVL_2_2();
	void loadMinionsLVL_2_3();
	void loadMinionsLVL_2_4();
	void loadMinionsLVL_3_1();
	void loadMinionsLVL_3_2();
	void loadMinionsLVL_3_3();
	void loadMinionsLVL_3_4();
	void loadMinionsLVL_4_1();
	void loadMinionsLVL_4_2();
	void loadMinionsLVL_4_3();
	void loadMinionsLVL_4_4();
	void loadMinionsLVL_5_1();
	void loadMinionsLVL_5_2();
	void loadMinionsLVL_5_3();
	void loadMinionsLVL_5_4();
	void loadMinionsLVL_6_1();
	void loadMinionsLVL_6_2();
	void loadMinionsLVL_6_3();
	void loadMinionsLVL_6_4();
	void loadMinionsLVL_7_1();
	void loadMinionsLVL_7_2();
	void loadMinionsLVL_7_3();
	void loadMinionsLVL_7_4();
	void loadMinionsLVL_8_1();
	void loadMinionsLVL_8_2();
	void loadMinionsLVL_8_3();
	void loadMinionsLVL_8_4();

	void clearPipeEvents(); // Done
	void loadPipeEventsLVL_1_1(); // Done
	void loadPipeEventsLVL_1_2(); // Done
	//void loadPipeEventsLVL_1_3(); // Not used
	void loadPipeEventsLVL_2_1(); // Done
	void loadPipeEventsLVL_2_2(); // Done
	void loadPipeEventsLVL_3_1(); // Done
	void loadPipeEventsLVL_4_1(); // Done
	void loadPipeEventsLVL_4_2(); // Done
	void loadPipeEventsLVL_5_1(); // Done
	void loadPipeEventsLVL_5_2(); // Done
	void loadPipeEventsLVL_6_2(); // Done
	void loadPipeEventsLVL_7_1(); // Done
	void loadPipeEventsLVL_7_2(); // Done
	void loadPipeEventsLVL_8_1(); // Done
	void loadPipeEventsLVL_8_2(); // Done
	void loadPipeEventsLVL_8_4(); // Done

	void clearLevelText();

	void pipeUse();
	void EndUse();
	void EndBoss();
	void EndBonus();

	//void TPUse();
	//void TPUse2();
	//void TPUse3();

	bool bTP; // -- TP LOOP

	//void spawnVine(int nX, int nY, int iBlockID);

	void clearMap();
	void clearMinions();

	// SingleTon design pattern
	Map();
	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;
public:
	static Map& Instance();
	~Map();

    void update();
	void updateGifBlocks();
	void updatePlayer();
	void updateMinionBlocks();
	void updateMinions();
	void updateMinionsCollisions();
	void updateSkillCollisions(Minion* pMinion);

	void draw(sf::RenderWindow& mainWindow);
	void drawMap(sf::RenderWindow& mainWindow);
	void DrawMinions(sf::RenderWindow& mainWindow);
	void DrawGameLayout(sf::RenderWindow& mainWindow);

	void moveSelectPlayer(int iD);

	//Add 
	void addPoints(int X, int Y, std::string sText, int iW, int iH);
	void addGoombas(int iX, int iY, bool moveDirection);
	void addKoppa(int iX, int iY, int minionState, bool moveDirection);
	void addBeetle(int X, int Y, bool moveDirection);
	void addPlant(int iX, int iY);
	void addToad(int X, int Y, bool peach);
	void addSquid(int X, int Y);
	void addCheep(int X, int Y, int minionType, int moveSpeed, bool moveDirection = false);
	void addCheepSpawner(int X, int XEnd);
	void addHammerBro(int X, int Y);
	void addHammer(int X, int Y, bool moveDirection);
	void addLakito(int X, int Y, int iMaxXPos);
	void addSpikey(int X, int Y);

	//void addPlayerFireBall(int X, int Y, bool moveDirection);

	//void addUpFire(int X, int iYEnd);
	//void addSpring(int X, int Y);

	void addBowser(int X, int Y, bool spawnHammer = false);

	void addFire(float fX, float fY, int toYPos);
	//void addFireBall(int X, int Y, int iWidth, int iSliceID, bool LEFT);

	//void addVine(int X, int Y, int minionState, int iBlockID);

	void addText(int X, int Y, std::string sText);

	//void addBubble(int X, int Y);
	void addBulletBillSpawner(int X, int Y, int minionState);
	void addBulletBill(int X, int Y, bool moveDirection, int minionState);

	// --- Clear ---
	void lockMinions();
	void clearPlatforms();
	//void clearBubbles();

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

	void structBulletBill(int X, int Y, int iHieght);

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
	Block* getMinionBlock(int iID);
};