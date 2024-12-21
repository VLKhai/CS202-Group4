#pragma once
#include "AniSprite.h"

class ExplodeSkill
{
private:
	float fX, fY;
	float fXcen, fYcen;
	std::vector<AniSprite*> sExplodeSkill;
	int iSpriteID;
	int iMoveAnimationTime;

	int iHitBoxX, iHitBoxY;
public:
	ExplodeSkill(sf::RenderWindow& window, float fX, float fY);
	~ExplodeSkill();

	void update(float fX, float fY);
	void updateAnimation();

	void draw(sf::RenderWindow& window);
	void drawHitBox(sf::RenderWindow& window);
	
	int getXPos();
	int getYPos();
	
	int getSpriteID();

	void setHitBox(int x, int y);
};