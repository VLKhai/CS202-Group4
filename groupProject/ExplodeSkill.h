#pragma once
#include "Skill.h"

class ExplodeSkill : public Skill
{
private:
	float fXcen, fYcen;
public:
	ExplodeSkill(sf::RenderWindow& window, float fX, float fY);
	~ExplodeSkill();

	void update(float fX, float fY);
	void updateAnimation();

	void draw(sf::RenderWindow& window);
};