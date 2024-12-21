#include "ExplodeSkill.h"
#include "Core.h"

ExplodeSkill::ExplodeSkill(sf::RenderWindow& window, float fX, float fY)
{
	this->iSpriteID = 0;
	this->iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
	
	this->fX = fX; this->fY = fY;
	this->iHitBoxX = 0; this->iHitBoxY = 0;

	// LOAD SPRITE
	std::vector<std::string> tempS;
	std::vector<unsigned int> tempI;
	// ----- 0
	tempS.push_back("explodeSkill/0");
	tempI.push_back(0);
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 1
	tempS.push_back("explodeSkill/1");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 2
	tempS.push_back("explodeSkill/2");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 3
	tempS.push_back("explodeSkill/3");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 4
	tempS.push_back("explodeSkill/4");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 5
	tempS.push_back("explodeSkill/5");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 6
	tempS.push_back("explodeSkill/6");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 7
	tempS.push_back("explodeSkill/7");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 8
	tempS.push_back("explodeSkill/8");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 9
	tempS.push_back("explodeSkill/9");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 10
	tempS.push_back("explodeSkill/10");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 11
	tempS.push_back("explodeSkill/11");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 12
	tempS.push_back("explodeSkill/12");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	// ----- 13
	tempS.push_back("explodeSkill/13");
	sExplodeSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
}

ExplodeSkill::~ExplodeSkill()
{
	for (AniSprite* sp : sExplodeSkill)
		delete sp;
}

void ExplodeSkill::update(float fXcen, float fYcen)
{
	this->fXcen = fXcen; this->fYcen = fYcen - 30;
	this->fX = fXcen - iHitBoxX/2; this->fY = this->fYcen - iHitBoxY/2;
	if (2 <= iSpriteID && iSpriteID <= 6) setHitBox(140, 150);
	updateAnimation();
}

void ExplodeSkill::updateAnimation()
{
	int deltaT = (2 <= iSpriteID && iSpriteID <= 6) ? 40 : 150;
	if (Core::coreClock.getElapsedTime().asMilliseconds() - iMoveAnimationTime > deltaT) {
		if (iSpriteID == 13) bTrigger = false;
		iSpriteID = (iSpriteID + 1) % 14;
		iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
	}
}

void ExplodeSkill::draw(sf::RenderWindow& window)
{
	sExplodeSkill[getSpriteID()]->getFrame()->drawFromCenter(window, fXcen, fYcen);
	//drawHitBox(window);
}

void ExplodeSkill::drawHitBox(sf::RenderWindow& window)
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

int ExplodeSkill::getXPos()
{
	return fX;
}

int ExplodeSkill::getYPos()
{
	return fY;
}

int ExplodeSkill::getSpriteID()
{
	return this->iSpriteID;
}

int ExplodeSkill::getHitBoxX()
{
	return iHitBoxX;
}

int ExplodeSkill::getHitBoxY()
{
	return iHitBoxY;
}

void ExplodeSkill::setHitBox(int x, int y)
{
	this->iHitBoxX = x;
	this->iHitBoxY = y;
}

bool ExplodeSkill::getTrigger()
{
	return bTrigger;
}
