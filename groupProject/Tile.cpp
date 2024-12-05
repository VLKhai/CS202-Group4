#include "Tile.h"

Tile::Tile(int iBlockID)
{
	this->iBlockID = iBlockID;

	this->iNumOfUse = 0;

	this->blockAnimation = false;
	this->iYPos = 0;
	this->bYDirection = true;

	this->spawnMushroom = false;
	this->spawnStar = false;

	this->powerUP = true;
}

void Tile::startBlockAnimation()
{
	this->blockAnimation = true;
	this->iYPos = 0;
	this->bYDirection = true;
}

int Tile::updateYPos()
{
	if (blockAnimation) {
		if (bYDirection)
		{
			if (iYPos < 10)
			{
				if (iYPos < 5) {
					iYPos += 2;
				}
				else {
					iYPos += 1;
				}
			}
			else {
				bYDirection = false;
			}
		}
		else {
			if (iYPos > 0) {
				if (iYPos > 5) {
					iYPos -= 2;
				}
				else {
					iYPos -= 1;
				}
			}
			else {
				blockAnimation = false;
			}
		}
	}

	return iYPos;
}

// ----- get & set -----

int Tile::getBlockID() {
	return iBlockID;
}

void Tile::setBlockID(int iBlockID) {
	this->iBlockID = iBlockID;
}

int Tile::getYPos() {
	return iYPos;
}

int Tile::getNumOfUse() {
	return iNumOfUse;
}

void Tile::setNumOfUse(int iNumOfUse) {
	this->iNumOfUse = iNumOfUse;
}

bool Tile::getSpawnMushroom() {
	return spawnMushroom;
}

void Tile::setSpawnMushroom(bool spawnMushroom) {
	this->spawnMushroom = spawnMushroom;
}

bool Tile::getPowerUP() {
	return powerUP;
}

void Tile::setPowerUP(bool powerUP) {
	this->powerUP = powerUP;
}

bool Tile::getSpawnStar() {
	return spawnStar;
}

void Tile::setSpawnStar(bool spawnStar) {
	this->spawnStar = spawnStar;
}
