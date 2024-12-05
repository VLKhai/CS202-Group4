#pragma once

class Tile
{
private:
	int iBlockID;

	bool spawnMushroom;
	bool powerUP; // -- true = powerUP, false = 1UP
	bool spawnStar;
	int iNumOfUse;

	// ----- Animation -----

	bool blockAnimation;
	int iYPos;
	bool bYDirection;	// ----- true TOP, false BOTTOM

	// ----- Animation -----

public:
	Tile(int iBlockID);
	~Tile() = default;

	void startBlockAnimation();

	int updateYPos();

	/* ----- get & set ----- */

	int getBlockID();
	void setBlockID(int iBlockID);

	int getYPos();

	int getNumOfUse();
	void setNumOfUse(int iNumOfUse);

	bool getSpawnMushroom();
	void setSpawnMushroom(bool spawnMushroom);
	bool getPowerUP();
	void setPowerUP(bool powerUP);
	bool getSpawnStar();
	void setSpawnStar(bool spawnStar);
};