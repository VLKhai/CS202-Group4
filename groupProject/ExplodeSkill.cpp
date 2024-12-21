#include "ExplodeSkill.h"
#include "Core.h"

ExplodeSkill::ExplodeSkill(sf::RenderWindow& window, float fX, float fY) : Skill(window, fX, fY)
{

	// LOAD SPRITE
	std::vector<std::string> tempS;
	std::vector<unsigned int> tempI;
	// ----- 0
	tempS.push_back("explodeSkill/0");
	tempI.push_back(0);
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 1
	tempS.push_back("explodeSkill/1");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 2
	tempS.push_back("explodeSkill/2");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 3
	tempS.push_back("explodeSkill/3");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 4
	tempS.push_back("explodeSkill/4");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 5
	tempS.push_back("explodeSkill/5");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 6
	tempS.push_back("explodeSkill/6");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 7
	tempS.push_back("explodeSkill/7");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 8
	tempS.push_back("explodeSkill/8");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 9
	tempS.push_back("explodeSkill/9");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 10
	tempS.push_back("explodeSkill/10");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 11
	tempS.push_back("explodeSkill/11");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 12
	tempS.push_back("explodeSkill/12");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	// ----- 13
	tempS.push_back("explodeSkill/13");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
}

ExplodeSkill::~ExplodeSkill()
{
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
	sSkill[getSpriteID()]->getFrame()->drawFromCenter(window, fXcen, fYcen);
	//drawHitBox(window);
}
