#include "ExplodeSkill.h"
#include "Core.h"

ExplodeSkill::ExplodeSkill(sf::RenderWindow& window, float fX, float fY)
{
	this->iSpriteID = 0;
	this->iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
	
	this->fX = fX; this->fY = fY;

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

void ExplodeSkill::update(float fX, float fY)
{
	this->fX = fX; this->fY = fY + 20;
	updateAnimation();
}

void ExplodeSkill::updateAnimation()
{
	if (Core::coreClock.getElapsedTime().asMilliseconds() - iMoveAnimationTime > 100) {
		iSpriteID = (iSpriteID + 1) % 14;
		iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
	}
}

void ExplodeSkill::draw(sf::RenderWindow& window)
{
	sExplodeSkill[getSpriteID()]->getFrame()->drawFromBotCen(window, fX, fY);
}

int ExplodeSkill::getSpriteID()
{
	return this->iSpriteID;
}


