#ifndef MINIONFACTORY_H
#define MINIONFACTORY_H

#include "Minion.h"
#include "Goombas.h"
#include "Koppa.h"
#include "Beetle.h"
#include "Bowser.h"
#include "BulletBill.h"
#include "BulletBillSpawner.h"
#include "Cheep.h"
#include "CheepSpawner.h"
#include "Fire.h"
#include "Hammer.h"
#include "HammerBro.h"
#include "Lakito.h"
#include "Plant.h"
#include "Spikey.h"
#include "Star.h"
#include "Toad.h"
#include "Squid.h"
#include "UpFire.h"
#include "Bubble.h"
#include "Spring.h"
#include "Vine.h"

class MinionFactory {
public:
    static Minion* addGoombas(int x, int y, bool moveDirection,int iLevelType);
    static Minion* addKoppa(int x, int y, int minionState, bool moveDirection, int iLevelType);
	static Minion*addBeetle(int X, int y, bool moveDirection);
	static Minion*addPlant(int iX, int iY,int iLevelType);
	static Minion*addToad(int X, int Y, bool peach);
	static Minion*addSquid(int X, int Y);
	static Minion*addCheep(int X, int Y, int minionType, int moveSpeed, bool moveDirection = false);
	static Minion*addCheepSpawner(int X, int XEnd);
	static Minion*addHammerBro(int X, int Y);
	static Minion*addHammer(int X, int Y, bool moveDirection);
	static Minion*addLakito(int X, int Y, int iMaxXPos);
	static Minion*addSpikey(int X, int Y);


	static Minion*addUpFire(int X, int iYEnd);
	static Minion*addSpring(int X, int Y);

	static Minion*addBowser(int X, int Y, bool spawnHammer = false);

	static Minion*addFire(float fX, float fY, int toYPos);

	static Minion*addVine(int X, int Y, int minionState, int iBlockID);
	static Minion* addBulletBillSpawner(int X, int Y, int minionState);
	static Minion* addBulletBill(int X, int Y, bool moveDirection, int minionState);
};

#endif // MINIONFACTORY_H
