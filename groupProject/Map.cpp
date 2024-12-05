#include "Map.h"

Map::Map(sf::RenderWindow& mainWindow)
{
	this->currentLevelID = 0;

	this->iMapWidth = 0;
	this->iMapHeight = 0;
	this->iLevelType = 0;

	//this->drawLines = false;
	this->fXPos = 0;
	this->fYPos = 0;

	this->inEvent = false;

	this->iSpawnPointID = 0;

	this->bMoveMap = true;

	this->iFrameID = 0;

	//this->bTP = false;
	player = new Player(mainWindow, 84, 350);
	pEvent = nullptr;

	srand((unsigned)time(NULL));

	loadGameData(mainWindow);
	loadLVL();
}

void Map::update()
{
	updateGifBlocks();
	updatePlayer();
	// Loop: platform->update();
}

void Map::updateGifBlocks()
{
	vBlock[2]->getAniSprite()->update(); // gnd_red_1
	vBlock[8]->getAniSprite()->update(); // blockq_0, 2, 1, 2
	vBlock[29]->getAniSprite()->update(); // coin_use0, 2, 1, 2
	vBlock[55]->getAniSprite()->update(); // blockq1_0, 2, 1, 2
	vBlock[57]->getAniSprite()->update(); // coin1_0, 2, 1, 2
	vBlock[70]->getAniSprite()->update(); // t_bot
	vBlock[71]->getAniSprite()->update(); // coin_use00, 02, 01, 02
	vBlock[72]->getAniSprite()->update(); // coin_use20, 22, 21, 22
	vBlock[73]->getAniSprite()->update(); // coin_use30, 32, 31, 32
	vBlock[82]->getAniSprite()->update(); // axe_0, 1, 2
}

void Map::updatePlayer()
{

}

void Map::draw(sf::RenderWindow& mainWindow)
{
}

void Map::loadLVL()
{
	switch (currentLevelID) {
	case 0:
		loadLVL_1_1();
		break;
	}
}

void Map::structBush(int X, int Y, int iSize)
{
	// ----- LEFT & RIGHT
	for (int i = 0; i < iSize; i++) {
		lMap[X + i][Y + i]->setBlockID(5);
		lMap[X + iSize + 1 + i][Y + iSize - 1 - i]->setBlockID(6);
	}

	// ----- CENTER LEFT & RIGHT
	for (int i = 0, k = 1; i < iSize - 1; i++) {
		for (int j = 0; j < k; j++) {
			lMap[X + 1 + i][Y + j]->setBlockID((i % 2 == 0 ? 3 : 4));
			lMap[X + iSize * 2 - 1 - i][Y + j]->setBlockID((i % 2 == 0 ? 3 : 4));
		}
		++k;
	}

	// ----- CENTER
	for (int i = 0; i < iSize; i++) {
		lMap[X + iSize][Y + i]->setBlockID((i % 2 == 0 && iSize != 1 ? 4 : 3));
	}

	// ----- TOP
	lMap[X + iSize][Y + iSize]->setBlockID(7);
}

void Map::structGrass(int X, int Y, int iSize)
{
}

void Map::structCloud(int X, int Y, int iSize)
{
}

void Map::structGND(int X, int Y, int iWidth, int iHeight)
{
}

void Map::structGND2(int X, int Y, int iSize, bool bDir)
{
}

void Map::structGND2(int X, int Y, int iWidth, int iHeight)
{
}

float Map::getXPos()
{
	return fXPos;
}

void Map::setXPos(float val)
{
	fXPos = val;
}

float Map::getYPos()
{
	return fYPos;
}

void Map::setYPos(float val)
{
	fYPos = val;
}

int Map::getLevelType()
{
	return iLevelType;
}

void Map::setLevelType(int iLevelType)
{
	this->iLevelType = iLevelType;
}

int Map::getCurrentLevelID()
{
	return currentLevelID;
}

void Map::setCurrentLevelID(int currentLevelID)
{
	if (this->currentLevelID != currentLevelID) {
		this->currentLevelID = currentLevelID;
		//oEvent->resetRedraw();
		//loadLVL();
		iSpawnPointID = 0;
	}

	this->currentLevelID = currentLevelID;
}

Player* Map::getPlayer()
{
    return player;
}

Platform* Map::getPlatform(int iID)
{
	return vPlatform[iID];
}

Block* Map::getBlock(int iID)
{
	return vBlock[iID];
}

void Map::loadGameData(sf::RenderWindow& mainWindow)
{
	std::vector<std::string> tSprite;
	std::vector<unsigned int> iDelay;

	// ----- 0 Transparent -----
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(0, new AniSprite(mainWindow, tSprite, iDelay, false), false, true, false, false));
	tSprite.clear();
	iDelay.clear();
	// ----- 1 -----
	tSprite.push_back("gnd_red_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(1, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 2 -----
	tSprite.push_back("coin_0");
	iDelay.push_back(300);
	tSprite.push_back("coin_2");
	iDelay.push_back(30);
	tSprite.push_back("coin_1");
	iDelay.push_back(130);
	tSprite.push_back("coin_2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(2, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 3 -----
	tSprite.push_back("bush_center_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(3, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 4 -----
	tSprite.push_back("bush_center_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(4, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 5 -----
	tSprite.push_back("bush_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(5, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 6 -----
	tSprite.push_back("bush_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(6, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 7 -----
	tSprite.push_back("bush_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(7, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 8 -----
	tSprite.push_back("blockq_0");
	iDelay.push_back(300);
	tSprite.push_back("blockq_2");
	iDelay.push_back(30);
	tSprite.push_back("blockq_1");
	iDelay.push_back(130);
	tSprite.push_back("blockq_2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(8, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 9 -----
	tSprite.push_back("blockq_used");
	iDelay.push_back(0);
	vBlock.push_back(new Block(9, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 10 -----
	tSprite.push_back("grass_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(10, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 11 -----
	tSprite.push_back("grass_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(11, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 12 -----
	tSprite.push_back("grass_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(12, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 13 -----
	tSprite.push_back("brickred");
	iDelay.push_back(0);
	vBlock.push_back(new Block(13, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 14 -----
	tSprite.push_back("cloud_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(14, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 15 -----
	tSprite.push_back("cloud_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(15, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 16 -----
	tSprite.push_back("cloud_center_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(16, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 17 -----
	tSprite.push_back("cloud_center_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(17, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 18 -----
	tSprite.push_back("cloud_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(18, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 19 -----
	tSprite.push_back("cloud_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(19, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 20 -----
	tSprite.push_back("pipe_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(20, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 21 -----
	tSprite.push_back("pipe_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(21, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 22 -----
	tSprite.push_back("pipe_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(22, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 23 -----
	tSprite.push_back("pipe_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(23, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 24 BlockQ2 -----
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(24, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, false));
	tSprite.clear();
	iDelay.clear();
	// ----- 25 -----
	tSprite.push_back("gnd_red2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(25, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 26 -----
	tSprite.push_back("gnd1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(26, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 27 -----
	tSprite.push_back("gnd1_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(27, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 28 -----
	tSprite.push_back("brick1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(28, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 29 -----
	tSprite.push_back("coin_use0");
	iDelay.push_back(300);
	tSprite.push_back("coin_use2");
	iDelay.push_back(30);
	tSprite.push_back("coin_use1");
	iDelay.push_back(130);
	tSprite.push_back("coin_use2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(29, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 30 -----
	tSprite.push_back("pipe1_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(30, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 31 -----
	tSprite.push_back("pipe1_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(31, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 32 -----
	tSprite.push_back("pipe1_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(32, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 33 -----
	tSprite.push_back("pipe1_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(33, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 34 -----
	tSprite.push_back("pipe1_hor_bot_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(34, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 35 -----
	tSprite.push_back("pipe1_hor_top_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(35, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 36 -----
	tSprite.push_back("pipe1_hor_top_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(36, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 37 -----
	tSprite.push_back("pipe1_hor_bot_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(37, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 38 -----
	tSprite.push_back("pipe1_hor_bot_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(38, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 39 -----
	tSprite.push_back("pipe1_hor_top_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(39, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 40 -----
	tSprite.push_back("end0_l");
	iDelay.push_back(0);
	vBlock.push_back(new Block(40, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 41 -----
	tSprite.push_back("end0_dot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(41, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 42 -----
	tSprite.push_back("end0_flag");
	iDelay.push_back(0);
	vBlock.push_back(new Block(42, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 43 -----
	tSprite.push_back("castle0_brick");
	iDelay.push_back(0);
	vBlock.push_back(new Block(43, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 44 -----
	tSprite.push_back("castle0_top0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(44, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 45 -----
	tSprite.push_back("castle0_top1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(45, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 46 -----
	tSprite.push_back("castle0_center_center_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(46, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 47 -----
	tSprite.push_back("castle0_center_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(47, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 48 -----
	tSprite.push_back("castle0_center_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(48, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 49 -----
	tSprite.push_back("castle0_center_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(49, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 50 -----
	tSprite.push_back("coin_an0");
	iDelay.push_back(0);
	tSprite.push_back("coin_an1");
	iDelay.push_back(0);
	tSprite.push_back("coin_an2");
	iDelay.push_back(0);
	tSprite.push_back("coin_an3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(50, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 51 -----
	tSprite.push_back("castle_flag");
	iDelay.push_back(0);
	vBlock.push_back(new Block(51, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 52 -----
	tSprite.push_back("firework0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(52, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 53 -----
	tSprite.push_back("firework1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(53, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 54 -----
	tSprite.push_back("firework2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(54, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 55 -----
	tSprite.push_back("blockq1_0");
	iDelay.push_back(300);
	tSprite.push_back("blockq1_2");
	iDelay.push_back(30);
	tSprite.push_back("blockq1_1");
	iDelay.push_back(130);
	tSprite.push_back("blockq1_2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(55, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 56 -----
	tSprite.push_back("blockq1_used");
	iDelay.push_back(0);
	vBlock.push_back(new Block(56, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 57 -----
	tSprite.push_back("coin1_0");
	iDelay.push_back(300);
	tSprite.push_back("coin1_2");
	iDelay.push_back(30);
	tSprite.push_back("coin1_1");
	iDelay.push_back(130);
	tSprite.push_back("coin1_2");
	iDelay.push_back(140);
	vBlock.push_back(new Block(57, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 58 -----
	tSprite.push_back("pipe_hor_bot_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(58, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 59 -----
	tSprite.push_back("pipe_hor_top_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(59, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 60 -----
	tSprite.push_back("pipe_hor_top_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(60, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 61 -----
	tSprite.push_back("pipe_hor_bot_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(61, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 62 -----
	tSprite.push_back("pipe_hor_bot_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(62, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 63 -----
	tSprite.push_back("pipe_hor_top_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(63, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 64 -----
	tSprite.push_back("block_debris0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(64, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 65 -----
	tSprite.push_back("block_debris1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(65, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 66 -----
	tSprite.push_back("block_debris2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(66, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 67 -----
	tSprite.push_back("t_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(67, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 68 -----
	tSprite.push_back("t_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(68, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 69 -----
	tSprite.push_back("t_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(69, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 70 -----
	tSprite.push_back("t_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(70, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 71 -----
	tSprite.push_back("coin_use00");
	iDelay.push_back(300);
	tSprite.push_back("coin_use02");
	iDelay.push_back(30);
	tSprite.push_back("coin_use01");
	iDelay.push_back(130);
	tSprite.push_back("coin_use02");
	iDelay.push_back(140);
	vBlock.push_back(new Block(71, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 72 -----
	tSprite.push_back("coin_use20");
	iDelay.push_back(300);
	tSprite.push_back("coin_use22");
	iDelay.push_back(30);
	tSprite.push_back("coin_use21");
	iDelay.push_back(130);
	tSprite.push_back("coin_use22");
	iDelay.push_back(140);
	vBlock.push_back(new Block(72, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 73 -----
	tSprite.push_back("coin_use30");
	iDelay.push_back(300);
	tSprite.push_back("coin_use32");
	iDelay.push_back(30);
	tSprite.push_back("coin_use31");
	iDelay.push_back(130);
	tSprite.push_back("coin_use32");
	iDelay.push_back(140);
	vBlock.push_back(new Block(73, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 74 -----
	tSprite.push_back("platform");
	iDelay.push_back(0);
	vBlock.push_back(new Block(74, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 75 -----
	tSprite.push_back("gnd_4");
	iDelay.push_back(0);
	vBlock.push_back(new Block(75, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 76 -----
	tSprite.push_back("bridge_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(76, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 77 -----
	tSprite.push_back("lava_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(77, new AniSprite(mainWindow, tSprite, iDelay, false), false, true, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 78 -----
	tSprite.push_back("lava_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(78, new AniSprite(mainWindow, tSprite, iDelay, false), false, true, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 79 -----
	tSprite.push_back("bridge_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(78, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 80 -----
	tSprite.push_back("blockq2_used");
	iDelay.push_back(0);
	vBlock.push_back(new Block(80, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 81 -----
	tSprite.push_back("brick2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(81, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 82 -----
	tSprite.push_back("axe_0");
	iDelay.push_back(225);
	tSprite.push_back("axe_1");
	iDelay.push_back(225);
	tSprite.push_back("axe_2");
	iDelay.push_back(200);
	vBlock.push_back(new Block(82, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 83 ----- END BRIDGE
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(83, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 84 -----
	tSprite.push_back("tree_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(84, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 85 -----
	tSprite.push_back("tree_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(85, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 86 -----
	tSprite.push_back("tree_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(86, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 87 -----
	tSprite.push_back("tree1_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(87, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 88 -----
	tSprite.push_back("tree1_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(88, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 89 -----
	tSprite.push_back("tree1_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(89, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 90 -----
	tSprite.push_back("fence");
	iDelay.push_back(0);
	vBlock.push_back(new Block(90, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 91 -----
	tSprite.push_back("tree_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(91, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 92 -----
	tSprite.push_back("uw_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(92, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 93 -----
	tSprite.push_back("uw_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(93, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 94 -----
	tSprite.push_back("water_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(94, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 95 -----
	tSprite.push_back("water_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(95, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 96 -----
	tSprite.push_back("bubble");
	iDelay.push_back(0);
	vBlock.push_back(new Block(96, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 97 -----
	tSprite.push_back("pipe2_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(97, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 98 -----
	tSprite.push_back("pipe2_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(98, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 99 -----
	tSprite.push_back("pipe2_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(99, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 100 -----
	tSprite.push_back("pipe2_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(100, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 101 -----
	tSprite.push_back("pipe2_hor_bot_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(101, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 102 -----
	tSprite.push_back("pipe2_hor_top_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(102, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 103 -----
	tSprite.push_back("pipe2_hor_top_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(103, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 104 -----
	tSprite.push_back("pipe2_hor_bot_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(104, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 105 -----
	tSprite.push_back("pipe2_hor_bot_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(105, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 106 -----
	tSprite.push_back("pipe2_hor_top_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(106, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 107 -----
	tSprite.push_back("bridge2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(107, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 108 -----
	tSprite.push_back("bridge3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(108, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 109 -----
	tSprite.push_back("platform1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(109, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 110 -----
	tSprite.push_back("water_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(110, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 111 -----
	tSprite.push_back("water_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(111, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 112 -----
	tSprite.push_back("pipe3_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(112, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 113 -----
	tSprite.push_back("pipe3_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(113, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 114 -----
	tSprite.push_back("pipe3_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(114, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 115 -----
	tSprite.push_back("pipe3_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(115, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 116 -----
	tSprite.push_back("pipe3_hor_bot_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(116, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 117 -----
	tSprite.push_back("pipe3_hor_top_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(117, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 118 -----
	tSprite.push_back("pipe3_hor_top_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(118, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 119 -----
	tSprite.push_back("pipe3_hor_bot_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(119, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 120 -----
	tSprite.push_back("pipe3_hor_bot_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(120, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 121 -----
	tSprite.push_back("pipe3_hor_top_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(121, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 122 -----
	tSprite.push_back("bridge4");
	iDelay.push_back(0);
	vBlock.push_back(new Block(122, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 123 -----
	tSprite.push_back("end1_l");
	iDelay.push_back(0);
	vBlock.push_back(new Block(123, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 124 -----
	tSprite.push_back("end1_dot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(124, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 125 -----
	tSprite.push_back("bonus");
	iDelay.push_back(0);
	vBlock.push_back(new Block(125, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 126 -----
	tSprite.push_back("platformbonus");
	iDelay.push_back(0);
	vBlock.push_back(new Block(126, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 127 ----- // -- BONUS END
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(127, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 128 ----- // -- SPAWN VINE
	tSprite.push_back("brickred");
	iDelay.push_back(0);
	vBlock.push_back(new Block(128, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 129 ----- // -- SPAWN VINE
	tSprite.push_back("brick1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(129, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 130 -----
	tSprite.push_back("vine");
	iDelay.push_back(0);
	vBlock.push_back(new Block(130, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 131 -----
	tSprite.push_back("vine1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(131, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 132 -----
	tSprite.push_back("seesaw_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(132, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 133 -----
	tSprite.push_back("seesaw_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(133, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 134 -----
	tSprite.push_back("seesaw_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(134, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 135 -----
	tSprite.push_back("seesaw_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(135, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 136 -----
	tSprite.push_back("pipe4_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(136, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 137 -----
	tSprite.push_back("pipe4_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(137, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 138 -----
	tSprite.push_back("pipe4_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(138, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 139 -----
	tSprite.push_back("pipe4_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(139, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 140 -----
	tSprite.push_back("t_left1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(140, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 141 -----
	tSprite.push_back("t_center1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(141, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 142 -----
	tSprite.push_back("t_right1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(142, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 143 -----
	tSprite.push_back("t_bot0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(143, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 144 -----
	tSprite.push_back("t_bot1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(144, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 145 -----
	tSprite.push_back("b_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(145, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 146 -----
	tSprite.push_back("b_top1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(146, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 147 -----
	tSprite.push_back("b_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(147, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 148 -----
	tSprite.push_back("cloud_left_bot1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(148, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 149 -----
	tSprite.push_back("cloud_center_bot1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(149, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 150 -----
	tSprite.push_back("cloud_center_top1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(150, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 151 -----
	tSprite.push_back("t_left2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(151, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 152 -----
	tSprite.push_back("t_center2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(152, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 153 -----
	tSprite.push_back("t_right2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(153, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 154 -----
	tSprite.push_back("t_bot2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(154, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 155 -----
	tSprite.push_back("castle1_brick");
	iDelay.push_back(0);
	vBlock.push_back(new Block(155, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 156 -----
	tSprite.push_back("castle1_top0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(156, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 157 -----
	tSprite.push_back("castle1_top1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(157, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 158 -----
	tSprite.push_back("castle1_center_center_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(158, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 159 -----
	tSprite.push_back("castle1_center_center");
	iDelay.push_back(0);
	vBlock.push_back(new Block(159, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 160 -----
	tSprite.push_back("castle1_center_left");
	iDelay.push_back(0);
	vBlock.push_back(new Block(160, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 161 -----
	tSprite.push_back("castle1_center_right");
	iDelay.push_back(0);
	vBlock.push_back(new Block(161, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 162 -----
	tSprite.push_back("seesaw1_0");
	iDelay.push_back(0);
	vBlock.push_back(new Block(162, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 163 -----
	tSprite.push_back("seesaw1_1");
	iDelay.push_back(0);
	vBlock.push_back(new Block(163, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 164 -----
	tSprite.push_back("seesaw1_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(164, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 165 -----
	tSprite.push_back("seesaw1_3");
	iDelay.push_back(0);
	vBlock.push_back(new Block(165, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 166 -----
	tSprite.push_back("gnd2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(166, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 167 -----
	tSprite.push_back("gnd2_2");
	iDelay.push_back(0);
	vBlock.push_back(new Block(167, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 168 -----
	tSprite.push_back("end1_flag");
	iDelay.push_back(0);
	vBlock.push_back(new Block(168, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 169 ----- TP
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(169, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 170 ----- TP2
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(170, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 171 ----- TP3 - bTP
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(171, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 172 -----
	tSprite.push_back("pipe5_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(172, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 173 -----
	tSprite.push_back("pipe5_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(173, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 174 -----
	tSprite.push_back("pipe5_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(174, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 175 -----
	tSprite.push_back("pipe5_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(175, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 176 -----
	tSprite.push_back("pipe6_left_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(176, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 177 -----
	tSprite.push_back("pipe6_left_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(177, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 178 -----
	tSprite.push_back("pipe6_right_bot");
	iDelay.push_back(0);
	vBlock.push_back(new Block(178, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 179 -----
	tSprite.push_back("pipe6_right_top");
	iDelay.push_back(0);
	vBlock.push_back(new Block(179, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 180 -----
	tSprite.push_back("crown");
	iDelay.push_back(0);
	vBlock.push_back(new Block(180, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 181 -----
	tSprite.push_back("gnd_5");
	iDelay.push_back(0);
	vBlock.push_back(new Block(166, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 182 ----- ENDUSE
	tSprite.push_back("transp");
	iDelay.push_back(0);
	vBlock.push_back(new Block(182, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, true, true));
	tSprite.clear();
	iDelay.clear();


	// --------------- MINION ---------------

	// ----- 0 -----
	tSprite.push_back("goombas_0");
	iDelay.push_back(200);
	tSprite.push_back("goombas_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(0, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 1 -----
	tSprite.push_back("goombas_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(1, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 2 -----
	tSprite.push_back("mushroom");
	iDelay.push_back(0);
	vMinion.push_back(new Block(2, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 3 -----
	tSprite.push_back("mushroom_1up");
	iDelay.push_back(0);
	vMinion.push_back(new Block(3, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 4 -----
	tSprite.push_back("koopa_0");
	iDelay.push_back(200);
	tSprite.push_back("koopa_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(4, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 5 -----
	tSprite.push_back("koopa_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(5, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 6 -----
	tSprite.push_back("flower0");
	iDelay.push_back(50);
	tSprite.push_back("flower1");
	iDelay.push_back(50);
	tSprite.push_back("flower2");
	iDelay.push_back(50);
	tSprite.push_back("flower3");
	iDelay.push_back(50);
	vMinion.push_back(new Block(6, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 7 -----
	tSprite.push_back("koopa_2");
	iDelay.push_back(200);
	tSprite.push_back("koopa_3");
	iDelay.push_back(200);
	vMinion.push_back(new Block(7, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 8 -----
	tSprite.push_back("goombas1_0");
	iDelay.push_back(200);
	tSprite.push_back("goombas1_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(8, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 9 -----
	tSprite.push_back("goombas1_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(9, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 10 -----
	tSprite.push_back("goombas2_0");
	iDelay.push_back(200);
	tSprite.push_back("goombas2_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(10, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 11 -----
	tSprite.push_back("goombas2_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(11, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 12 -----
	tSprite.push_back("koopa1_0");
	iDelay.push_back(200);
	tSprite.push_back("koopa1_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(12, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 13 -----
	tSprite.push_back("koopa1_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(13, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 14 -----
	tSprite.push_back("koopa1_2");
	iDelay.push_back(200);
	tSprite.push_back("koopa1_3");
	iDelay.push_back(200);
	vMinion.push_back(new Block(14, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 15 -----
	tSprite.push_back("koopa2_0");
	iDelay.push_back(200);
	tSprite.push_back("koopa2_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block(15, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 16 -----
	tSprite.push_back("koopa2_ded");
	iDelay.push_back(0);
	vMinion.push_back(new Block(16, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 17 -----
	tSprite.push_back("koopa2_2");
	iDelay.push_back(200);
	tSprite.push_back("koopa2_3");
	iDelay.push_back(200);
	vMinion.push_back(new Block(17, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 18 -----
	tSprite.push_back("plant_0");
	iDelay.push_back(125);
	tSprite.push_back("plant_1");
	iDelay.push_back(125);
	vMinion.push_back(new Block(18, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 19 -----
	tSprite.push_back("plant1_0");
	iDelay.push_back(125);
	tSprite.push_back("plant1_1");
	iDelay.push_back(125);
	vMinion.push_back(new Block(19, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 20 -----
	tSprite.push_back("bowser0");
	iDelay.push_back(285);
	tSprite.push_back("bowser1");
	iDelay.push_back(285);
	vMinion.push_back(new Block(20, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 21 -----
	tSprite.push_back("bowser2");
	iDelay.push_back(285);
	tSprite.push_back("bowser3");
	iDelay.push_back(285);
	vMinion.push_back(new Block(21, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 22 -----
	tSprite.push_back("fire_0");
	iDelay.push_back(35);
	tSprite.push_back("fire_1");
	iDelay.push_back(35);
	vMinion.push_back(new Block(22, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 23 -----
	tSprite.push_back("fireball_0");
	iDelay.push_back(75);
	tSprite.push_back("fireball_1");
	iDelay.push_back(75);
	tSprite.push_back("fireball_2");
	iDelay.push_back(75);
	tSprite.push_back("fireball_3");
	iDelay.push_back(75);
	vMinion.push_back(new Block(23, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 24 -----
	tSprite.push_back("star_0");
	iDelay.push_back(75);
	tSprite.push_back("star_1");
	iDelay.push_back(75);
	tSprite.push_back("star_2");
	iDelay.push_back(75);
	tSprite.push_back("star_3");
	iDelay.push_back(75);
	vMinion.push_back(new Block(24, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 25 -----
	tSprite.push_back("mushroom1_1up");
	iDelay.push_back(0);
	vMinion.push_back(new Block(25, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 26 -----
	tSprite.push_back("toad");
	iDelay.push_back(0);
	vMinion.push_back(new Block(26, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 27 -----
	tSprite.push_back("peach");
	iDelay.push_back(0);
	vMinion.push_back(new Block(27, new AniSprite(mainWindow, tSprite, iDelay, false), false, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 28 -----
	tSprite.push_back("squid0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(28, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 29 -----
	tSprite.push_back("squid1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(29, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 30 -----
	tSprite.push_back("cheep0");
	iDelay.push_back(120);
	tSprite.push_back("cheep1");
	iDelay.push_back(120);
	vMinion.push_back(new Block(30, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 31 -----
	tSprite.push_back("cheep2");
	iDelay.push_back(110);
	tSprite.push_back("cheep3");
	iDelay.push_back(110);
	vMinion.push_back(new Block(31, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 32 -----
	tSprite.push_back("upfire");
	iDelay.push_back(0);
	vMinion.push_back(new Block(32, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 33 -----
	tSprite.push_back("vine_top");
	iDelay.push_back(0);
	vMinion.push_back(new Block(33, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 34 -----
	tSprite.push_back("vine");
	iDelay.push_back(0);
	vMinion.push_back(new Block(34, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 35 -----
	tSprite.push_back("vine1_top");
	iDelay.push_back(0);
	vMinion.push_back(new Block(35, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 36 -----
	tSprite.push_back("vine1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(36, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 37 -----
	tSprite.push_back("spring_0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(37, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 38 -----
	tSprite.push_back("spring_1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(38, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 39 -----
	tSprite.push_back("spring_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(39, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 40 -----
	tSprite.push_back("spring1_0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(40, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 41 -----
	tSprite.push_back("spring1_1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(41, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 42 -----
	tSprite.push_back("spring1_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(42, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 43 -----
	tSprite.push_back("hammerbro_0");
	iDelay.push_back(175);
	tSprite.push_back("hammerbro_1");
	iDelay.push_back(175);
	vMinion.push_back(new Block(43, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 44 -----
	tSprite.push_back("hammerbro_2");
	iDelay.push_back(155);
	tSprite.push_back("hammerbro_3");
	iDelay.push_back(155);
	vMinion.push_back(new Block(44, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 45 -----
	tSprite.push_back("hammerbro1_0");
	iDelay.push_back(175);
	tSprite.push_back("hammerbro1_1");
	iDelay.push_back(175);
	vMinion.push_back(new Block(45, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 46 -----
	tSprite.push_back("hammerbro1_2");
	iDelay.push_back(155);
	tSprite.push_back("hammerbro1_3");
	iDelay.push_back(155);
	vMinion.push_back(new Block(46, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 47 -----
	tSprite.push_back("hammer_0");
	iDelay.push_back(95);
	tSprite.push_back("hammer_1");
	iDelay.push_back(95);
	tSprite.push_back("hammer_2");
	iDelay.push_back(95);
	tSprite.push_back("hammer_3");
	iDelay.push_back(95);
	vMinion.push_back(new Block(47, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 48 -----
	tSprite.push_back("hammer1_0");
	iDelay.push_back(95);
	tSprite.push_back("hammer1_1");
	iDelay.push_back(95);
	tSprite.push_back("hammer1_2");
	iDelay.push_back(95);
	tSprite.push_back("hammer1_3");
	iDelay.push_back(95);
	vMinion.push_back(new Block(48, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 49 -----
	tSprite.push_back("lakito_0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(49, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 50 -----
	tSprite.push_back("lakito_1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(50, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 51 -----
	tSprite.push_back("spikey0_0");
	iDelay.push_back(135);
	tSprite.push_back("spikey0_1");
	iDelay.push_back(135);
	vMinion.push_back(new Block(51, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 52 -----
	tSprite.push_back("spikey1_0");
	iDelay.push_back(75);
	tSprite.push_back("spikey1_1");
	iDelay.push_back(75);
	vMinion.push_back(new Block(52, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 53 -----
	tSprite.push_back("beetle_0");
	iDelay.push_back(155);
	tSprite.push_back("beetle_1");
	iDelay.push_back(155);
	vMinion.push_back(new Block(53, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 54 -----
	tSprite.push_back("beetle_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(54, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 55 -----
	tSprite.push_back("beetle1_0");
	iDelay.push_back(155);
	tSprite.push_back("beetle1_1");
	iDelay.push_back(155);
	vMinion.push_back(new Block(55, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 56 -----
	tSprite.push_back("beetle1_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(56, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 57 -----
	tSprite.push_back("beetle2_0");
	iDelay.push_back(155);
	tSprite.push_back("beetle2_1");
	iDelay.push_back(155);
	vMinion.push_back(new Block(57, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 58 -----
	tSprite.push_back("beetle2_2");
	iDelay.push_back(0);
	vMinion.push_back(new Block(58, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 59 -----
	tSprite.push_back("bulletbill");
	iDelay.push_back(0);
	vMinion.push_back(new Block(59, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 60 -----
	tSprite.push_back("bulletbill1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(60, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 61 -----
	tSprite.push_back("hammer1_0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(61, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, true, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 62 -----
	tSprite.push_back("fireball_0");
	iDelay.push_back(155);
	tSprite.push_back("fireball_1");
	iDelay.push_back(155);
	tSprite.push_back("fireball_2");
	iDelay.push_back(155);
	tSprite.push_back("fireball_3");
	iDelay.push_back(155);
	vMinion.push_back(new Block(62, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 63 -----
	tSprite.push_back("firework0");
	iDelay.push_back(0);
	vMinion.push_back(new Block(63, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 64 -----
	tSprite.push_back("firework1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(64, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();
	// ----- 65 -----
	tSprite.push_back("firework1");
	iDelay.push_back(0);
	vMinion.push_back(new Block(65, new AniSprite(mainWindow, tSprite, iDelay, false), true, false, false, true));
	tSprite.clear();
	iDelay.clear();

	iBlockSize = vBlock.size();
	iMinionSize = vMinion.size();
}

void Map::loadLVL_1_1()
{
	clearMap();

	this->iMapWidth = 260;
	this->iMapHeight = 25;
	this->iLevelType = 0;
	this->iMapTime = 400;

	createMap();


	// ----- Bush -----
	//structBush(0, 2, 2);
	//structBush(16, 2, 1);
	//structBush(48, 2, 2);
	//structBush(64, 2, 1);
	//structBush(96, 2, 2);
	//structBush(112, 2, 1);
	//structBush(144, 2, 2);
	//structBush(160, 2, 1);
	//structBush(192, 2, 2);
	//structBush(208, 2, 1);

	// ----- Clouds -----
	//structCloud(8, 10, 1);
	//structCloud(19, 11, 1);
	//structCloud(27, 10, 3);
	//structCloud(36, 11, 2);
	//structCloud(56, 10, 1);
	//structCloud(67, 11, 1);
	//structCloud(75, 10, 3);
	//structCloud(84, 11, 2);
	//structCloud(104, 10, 1);
	//structCloud(115, 11, 1);
	//structCloud(123, 10, 3);
	//structCloud(132, 11, 2);
	//structCloud(152, 10, 1);
	//structCloud(163, 11, 1);
	//structCloud(171, 10, 3);
	//structCloud(180, 11, 2);
	//structCloud(200, 10, 1);
	//structCloud(211, 11, 1);
	//structCloud(219, 10, 3);

	// ----- Grass -----
	//structGrass(11, 2, 3);
	//structGrass(23, 2, 1);
	//structGrass(41, 2, 2);
	//structGrass(59, 2, 3);
	//structGrass(71, 2, 1);
	//structGrass(89, 2, 2);
	//structGrass(107, 2, 3);
	//structGrass(119, 2, 1);
	//structGrass(137, 2, 2);
	//structGrass(157, 2, 1);
	//structGrass(167, 2, 1);
	//structGrass(205, 2, 1);
	//structGrass(215, 2, 1);

	// ----- GND -----
	//structGND(0, 0, 69, 2);
	//structGND(71, 0, 15, 2);
	//structGND(89, 0, 64, 2);
	//structGND(155, 0, 85, 2);

	// ----- GND 2 -----
	//structGND2(134, 2, 4, true);
	//structGND2(140, 2, 4, false);
	//structGND2(148, 2, 4, true);
	//structGND2(152, 2, 1, 4);
	//structGND2(155, 2, 4, false);
	//structGND2(181, 2, 8, true);
	//structGND2(189, 2, 1, 8);
	//structGND2(198, 2, 1, 1);


	// ----- BRICK -----
	//struckBlockQ(16, 5, 1);
	//structBrick(20, 5, 1, 1);
	//struckBlockQ(21, 5, 1);
	//lMap[21][5]->setSpawnMushroom(true);
	//structBrick(22, 5, 1, 1);
	//struckBlockQ(22, 9, 1);
	//struckBlockQ(23, 5, 1);
	//structBrick(24, 5, 1, 1);

	//struckBlockQ2(64, 6, 1);
	//lMap[64][6]->setSpawnMushroom(true);
	//lMap[64][6]->setPowerUP(false);

	//structBrick(77, 5, 1, 1);
	//struckBlockQ(78, 5, 1);
	//lMap[78][5]->setSpawnMushroom(true);
	//structBrick(79, 5, 1, 1);

	//structBrick(80, 9, 8, 1);
	//structBrick(91, 9, 3, 1);
	//struckBlockQ(94, 9, 1);
	//structBrick(94, 5, 1, 1);
	//lMap[94][5]->setNumOfUse(4);

	//structBrick(100, 5, 2, 1);

	//struckBlockQ(106, 5, 1);
	//struckBlockQ(109, 5, 1);
	//struckBlockQ(109, 9, 1);
	//lMap[109][9]->setSpawnMushroom(true);
	//struckBlockQ(112, 5, 1);

	//structBrick(118, 5, 1, 1);

	//structBrick(121, 9, 3, 1);

	//structBrick(128, 9, 1, 1);
	//struckBlockQ(129, 9, 2);
	//structBrick(131, 9, 1, 1);

	//structBrick(129, 5, 2, 1);

	//structBrick(168, 5, 2, 1);
	//struckBlockQ(170, 5, 1);
	//structBrick(171, 5, 1, 1);

	//lMap[101][5]->setSpawnStar(true);

	// ----- MAP 1_1_2 -----

	this->iLevelType = 1;

	//structGND(240, 0, 17, 2);

	/*structBrick(240, 2, 1, 11);
	structBrick(244, 2, 7, 3);
	structBrick(244, 12, 7, 1);*/

	// ----- END LEVEL

	this->iLevelType = 0;
}

void Map::createMap()
{
	// ----- MIONION LIST -----
	//for (int i = 0; i < iMapWidth; i += 5) {
	//	std::vector<Minion*> temp;
	//	lMinion.push_back(temp);
	//}

	//iMinionListSize = lMinion.size();

	// ----- CREATE MAP -----
	for (int i = 0; i < iMapWidth; i++) {
		std::vector<Tile*> temp;
		for (int i = 0; i < iMapHeight; i++) {
			temp.push_back(new Tile(0));
		}

		lMap.push_back(temp);
	}

	this->underWater = false;
	//this->bTP = false;
}

void Map::clearMap()
{
	for (int i = 0; i < iMapWidth; i++) {
		for (int j = 0; j < iMapHeight; j++) {
			delete lMap[i][j];
		}
		lMap[i].clear();
	}
	lMap.clear();

	this->iMapWidth = this->iMapHeight = 0;

	//if (oFlag != NULL) {
	//	delete oFlag;
	//	oFlag = NULL;
	//}

	//oEvent->eventTypeID = oEvent->eNormal;

	//clearLevelText();
}

void Map::clearMinions()
{
	return;
}
