#pragma once
#include "Vector2.h"

class BlockDebris
{
private:
	int debrisState;

	Vector2* vPositionL;
	Vector2* vPositionR;
	Vector2* vPositionL2;
	Vector2* vPositionR2;

	int iFrameID;

	float fSpeedX, fSpeedY;

	bool bRotate;
public:
};