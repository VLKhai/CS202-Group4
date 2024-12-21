#pragma once
#include "AniSprite.h"
#include <vector>


class Skill {
protected:
	float fX, fY;
	float fXcen, fYcen;
	std::vector<AniSprite*> sSkill;
	int iSpriteID;
	int iMoveAnimationTime;

	int iHitBoxX, iHitBoxY;
	bool bTrigger;
public:
	Skill(sf::RenderWindow& window, float fX, float fY);
	virtual ~Skill();

	virtual void update(float fX, float fY) = 0;
	virtual void updateAnimation() = 0;

	virtual void draw(sf::RenderWindow& window) = 0;
	void drawHitBox(sf::RenderWindow& window);

	int getXPos();
	int getYPos();

	int getSpriteID();

	int getHitBoxX();
	int getHitBoxY();
	void setHitBox(int x, int y);

	bool getTrigger();
	void setTrigger(bool bTrigger);
};