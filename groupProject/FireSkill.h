#pragma once
#include "Skill.h"

class FireSkill : public Skill
{
private:
	float fXcen, fYcen;
public:
	FireSkill(sf::RenderWindow& window, float fX, float fY);
	~FireSkill();

	void update(float fX, float fY);
	void updateAnimation();

	void draw(sf::RenderWindow& window);
};