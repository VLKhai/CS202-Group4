#include "FireSkill.h"
#include "Core.h"

FireSkill::FireSkill(sf::RenderWindow& window, float fX, float fY) : Skill(window, fX, fY)
{

	// LOAD SPRITE
	std::vector<std::string> tempS;
	std::vector<unsigned int> tempI;
	setHitBox(140, 140);
	// ----- 0
	tempS.push_back("FireSkill/0");
	tempI.push_back(0);
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 1
	tempS.push_back("FireSkill/1");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 2
	tempS.push_back("FireSkill/2");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 3
	tempS.push_back("FireSkill/3");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 4
	tempS.push_back("FireSkill/4");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 5
	tempS.push_back("FireSkill/5");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 6
	tempS.push_back("FireSkill/6");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 7
	tempS.push_back("FireSkill/7");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 8
	tempS.push_back("FireSkill/8");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 9
	tempS.push_back("FireSkill/9");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 10
	tempS.push_back("FireSkill/10");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 11
	tempS.push_back("FireSkill/11");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
	// ----- 12
	tempS.push_back("FireSkill/12");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	// ----- 13
	tempS.push_back("FireSkill/13");
	sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
	tempS.clear();
    // ----- 14
    tempS.push_back("FireSkill/14");
    sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
    tempS.clear();
    // ----- 15
    tempS.push_back("FireSkill/15");
    sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
    tempS.clear();
    // ----- 16
    tempS.push_back("FireSkill/16");
    sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
    tempS.clear();
    // ----- 17
    tempS.push_back("FireSkill/17");
    sSkill.push_back(new AniSprite(window, tempS, tempI, true, ".png"));
    tempS.clear();
}

FireSkill::~FireSkill()
{
}

void FireSkill::update(float fXcen, float fYcen)
{
	this->fXcen = fXcen; this->fYcen = fYcen - 30;
	this->fX = fXcen - iHitBoxX/2; this->fY = this->fYcen - iHitBoxY/2;
	//if (2 <= iSpriteID && iSpriteID <= 6) setHitBox(140, 150);
	updateAnimation();
}

void FireSkill::updateAnimation()
{
	int deltaT = (6 <= iSpriteID && iSpriteID <= 13) ? 30 : 50;
	if (Core::coreClock.getElapsedTime().asMilliseconds() - iMoveAnimationTime > deltaT) {
		if (iSpriteID == 17) bTrigger = false;
		iSpriteID = (iSpriteID + 1) % 18;
		iMoveAnimationTime = Core::coreClock.getElapsedTime().asMilliseconds();
	}
}

void FireSkill::draw(sf::RenderWindow& window)
{
	sSkill[getSpriteID()]->getFrame()->drawFromCenter(window, fXcen, fYcen);
	//drawHitBox(window);
}
