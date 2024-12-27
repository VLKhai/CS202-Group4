#include "MinionFactory.h"

Minion* MinionFactory::addGoombas(int x, int y, bool moveDirection, int iLevelType) {
	return new Goombas(x, y, iLevelType == 0 || iLevelType == 4 ? 0 : iLevelType == 1 ? 8 : 10, moveDirection);
}

Minion* MinionFactory::addKoppa(int x, int y, int minionState, bool moveDirection,int iLevelType) { 
	int tempBlock;

	switch (minionState) {
	case 0: case 3:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 7 : iLevelType == 1 ? 14 : 17;
		break;
	case 1:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 4 : iLevelType == 1 ? 12 : 15;
		break;
	case 2:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 5 : iLevelType == 1 ? 13 : 16;
		break;
	}

    return new Koppa(x, y, minionState, moveDirection, tempBlock);
}


Minion* MinionFactory::addBeetle(int x, int y, bool moveDirection) {
	return new Beetle(x, y, moveDirection);
}

Minion* MinionFactory::addPlant(int iX, int iY, int iLevelType) {
	return new Plant(iX,iY, iLevelType == 0 || iLevelType == 4 ? 18 : 19);
}

Minion* MinionFactory::addToad(int X, int Y, bool peach) {
	return new Toad(X, Y, peach);
}

Minion* MinionFactory::addSquid(int X, int Y) {
	return new Squid(X, Y);
}

Minion* MinionFactory::addCheep(int X, int Y, int minionType, int moveSpeed, bool moveDirection) {
	return new Cheep(X, Y, minionType, moveSpeed, moveDirection);
}
Minion* MinionFactory::addCheepSpawner(int X, int XEnd) {
	return new CheepSpawner(X, XEnd);
}
Minion* MinionFactory::addHammerBro(int X, int Y) {
	return new HammerBro(X, Y);
}
Minion* MinionFactory::addHammer(int X, int Y, bool moveDirection) {
	return new Hammer(X, Y, moveDirection);
}
Minion* MinionFactory::addLakito(int X, int Y, int iMaxXPos) {
	return new Lakito(X, Y, iMaxXPos);
}
Minion* MinionFactory::addSpikey(int X, int Y) {
	return new Spikey(X, Y);
}

Minion* MinionFactory::addUpFire(int X, int iYEnd) {
	return new UpFire(X, iYEnd);
}
Minion* MinionFactory::addSpring(int X, int Y) {
	return new Spring(X, Y);
}
Minion* MinionFactory::addBowser(int X, int Y, bool spawnHammer) {
	return new Bowser((float)X, (float)Y, spawnHammer);
}
Minion* MinionFactory::addFire(float fX, float fY, int toYPos) {
	return new Fire(fX, fY, toYPos);
}
Minion* MinionFactory::addVine(int X, int Y, int minionState, int iBlockID) {
	return new Vine(X, Y, minionState, iBlockID);
}

Minion* MinionFactory::addBulletBillSpawner(int X, int Y, int minionState) {
	return new BulletBillSpawner(X * 32, CFG::GameHeight - Y * 32, minionState);
}
Minion* MinionFactory::addBulletBill(int X, int Y, bool moveDirection, int minionState) {
	return new BulletBill(X, Y, moveDirection, minionState);
}

