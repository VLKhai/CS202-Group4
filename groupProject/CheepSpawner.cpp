#include "CheepSpawner.h"
#include "Core.h"
#include "stdlib.h"
#include "time.h"

/* ******************************************** */

CheepSpawner::CheepSpawner(int iXPos, int iXEnd) {
	this->fXPos = (float)iXPos;
	this->fYPos = 0;
	this->iXEnd = iXEnd;

	this->collisionOnlyWithPlayer = true;
	this->iHitBoxX = this->iHitBoxY = 1;

	this->minionSpawned = true;

	nextCheep();

	this->iBlockID = 0;

	srand((unsigned)time(NULL));
}

CheepSpawner::~CheepSpawner(void) {

}

/* ******************************************** */

void CheepSpawner::Update() {
	if(iSpawnTime < Core::coreClock.getElapsedTime().asMilliseconds()) {
		spawnCheep();
		nextCheep();
	}
}

void CheepSpawner::minionPhysics() { }

/* ******************************************** */

void CheepSpawner::spawnCheep() {
	if(Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() > fXPos && Core::getMap()->getPlayer()->getXPos() - Core::getMap()->getXPos() < iXEnd) {
		//Core::getMap()->addCheep((int)(-Core::getMap()->getXPos() + 64 + rand()%(CFG::GameWidth - 128)), CFG::GameHeight - 4, 2, 2 + rand() % 32, !(rand()%4 == 0));
	} else {
		nextCheep();
	}
}

void CheepSpawner::nextCheep() {
	iSpawnTime = Core::coreClock.getElapsedTime().asMilliseconds() + 675 + rand()%1025;
}