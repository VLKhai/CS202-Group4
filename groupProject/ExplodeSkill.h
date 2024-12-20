#pragma once
#include "AniSprite.h"

class ExplodeSkill
{
private:
	float fX, fY;

	std::vector<AniSprite*> sExplodeSkill;
	int iSpriteID;
	int iMoveAnimationTime;
public:
	ExplodeSkill(sf::RenderWindow& window, float fX, float fY);
	~ExplodeSkill();

	void update(float fX, float fY);
	void updateAnimation();
	void draw(sf::RenderWindow& window);

	int getSpriteID();
};