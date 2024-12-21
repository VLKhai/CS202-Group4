#include "Skill.h"
#include "Core.h"

Skill::Skill(sf::RenderWindow& window, float fX, float fY)
{
	this->iSpriteID = 0;
	this->iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();

	this->fX = fX; this->fY = fY;
	this->iHitBoxX = 0; this->iHitBoxY = 0;

	this->bTrigger = false;
}

Skill::~Skill()
{
	for (AniSprite* sp : sSkill)
		delete sp;
}

void Skill::drawHitBox(sf::RenderWindow& window)
{
	sf::RectangleShape rect(sf::Vector2f(iHitBoxX, iHitBoxY));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1);
	rect.setOrigin(iHitBoxX / 2, iHitBoxY / 2);
	rect.setPosition(fXcen, fYcen);
	window.draw(rect);
	//rect.setOutlineColor(sf::Color::Red);
	//rect.setOrigin(0, 0);
	//rect.setPosition(fX, fY);
	//window.draw(rect);
}

int Skill::getXPos()
{
	return fX;
}

int Skill::getYPos()
{
	return fY;
}

int Skill::getSpriteID()
{
	return this->iSpriteID;
}

int Skill::getHitBoxX()
{
	return iHitBoxX;
}

int Skill::getHitBoxY()
{
	return iHitBoxY;
}

void Skill::setHitBox(int x, int y)
{
	this->iHitBoxX = x;
	this->iHitBoxY = y;
}

bool Skill::getTrigger()
{
	return bTrigger;
}

void Skill::setTrigger(bool bTrigger)
{
	this->bTrigger = bTrigger;
}
