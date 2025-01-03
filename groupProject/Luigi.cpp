#include "Luigi.h"
#include "Core.h"
#include "ExplodeSkill.h"

Luigi::Luigi(sf::RenderWindow& window, float fXPos, float fYPos) : Player(window, fXPos, fYPos) {
	
	limSmallJumpBlock = 3;
	limBigJumpBlock = 5;
	iIDPlayer = 0;

	pSkill = new ExplodeSkill(window, fXPos, fYPos);

	// LOAD SPRITE
	std::vector<std::string> tempS;
	std::vector<unsigned int> tempI;
	// ----- 0
	tempS.push_back("luigi/luigi_death");
	tempI.push_back(0);
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 1
	tempS.push_back("luigi/luigi");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 2
	tempS.push_back("luigi/luigi_move0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 3
	tempS.push_back("luigi/luigi_move1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 4
	tempS.push_back("luigi/luigi_move2");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 5
	tempS.push_back("luigi/luigi_jump");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 6
	tempS.push_back("luigi/luigi_st");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 7
	tempS.push_back("luigi/luigi"); // SQUAT
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 8
	tempS.push_back("luigi/luigi_underwater0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 9
	tempS.push_back("luigi/luigi_underwater1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 10
	tempS.push_back("luigi/luigi_end");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 11
	tempS.push_back("luigi/luigi_end1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ---------- BIG
	// ----- 12
	tempS.push_back("luigi/luigi1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 13
	tempS.push_back("luigi/luigi1_move0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 14
	tempS.push_back("luigi/luigi1_move1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 15
	tempS.push_back("luigi/luigi1_move2");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 16
	tempS.push_back("luigi/luigi1_jump");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 17
	tempS.push_back("luigi/luigi1_st");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 18
	tempS.push_back("luigi/luigi1_squat"); // SQUAT
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 19
	tempS.push_back("luigi/luigi1_underwater0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 20
	tempS.push_back("luigi/luigi1_underwater1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 21
	tempS.push_back("luigi/luigi1_end");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 22
	tempS.push_back("luigi/luigi1_end1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- 23
	tempS.push_back("luigi/luigi2");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 24
	tempS.push_back("luigi/luigi2_move0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 25
	tempS.push_back("luigi/luigi2_move1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 26
	tempS.push_back("luigi/luigi2_move2");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 27
	tempS.push_back("luigi/luigi2_jump");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 28
	tempS.push_back("luigi/luigi2_st");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 29
	tempS.push_back("luigi/luigi2_squat"); // SQUAT
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 30
	tempS.push_back("luigi/luigi2_underwater0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 31
	tempS.push_back("luigi/luigi2_underwater1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 32
	tempS.push_back("luigi/luigi2_end");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 33
	tempS.push_back("luigi/luigi2_end1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- 34
	tempS.push_back("luigi/luigi2s");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 35
	tempS.push_back("luigi/luigi2s_move0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 36
	tempS.push_back("luigi/luigi2s_move1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 37
	tempS.push_back("luigi/luigi2s_move2");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 38
	tempS.push_back("luigi/luigi2s_jump");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 39
	tempS.push_back("luigi/luigi2s_st");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 40
	tempS.push_back("luigi/luigi2s_squat"); // SQUAT
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 41
	tempS.push_back("luigi/luigi2s_underwater0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 42
	tempS.push_back("luigi/luigi2s_underwater1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 43
	tempS.push_back("luigi/luigi2s_end");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 44
	tempS.push_back("luigi/luigi2s_end1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- 45
	tempS.push_back("luigi/luigi_s0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 46
	tempS.push_back("luigi/luigi_s0_move0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 47
	tempS.push_back("luigi/luigi_s0_move1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 48
	tempS.push_back("luigi/luigi_s0_move2");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 49
	tempS.push_back("luigi/luigi_s0_jump");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 50
	tempS.push_back("luigi/luigi_s0_st");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 51
	tempS.push_back("luigi/luigi_s0"); // SQUAT
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 52
	tempS.push_back("luigi/luigi_s0_underwater0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 53
	tempS.push_back("luigi/luigi_s0_underwater1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 54
	tempS.push_back("luigi/luigi_s0_end");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 55
	tempS.push_back("luigi/luigi_s0_end1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- 56
	tempS.push_back("luigi/luigi_s1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 57
	tempS.push_back("luigi/luigi_s1_move0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 58
	tempS.push_back("luigi/luigi_s1_move1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 59
	tempS.push_back("luigi/luigi_s1_move2");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 60
	tempS.push_back("luigi/luigi_s1_jump");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 61
	tempS.push_back("luigi/luigi_s1_st");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 62
	tempS.push_back("luigi/luigi_s1"); // SQUAT
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 63
	tempS.push_back("luigi/luigi_s1_underwater0");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 64
	tempS.push_back("luigi/luigi_s1_underwater1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 65
	tempS.push_back("luigi/luigi_s1_end");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 66
	tempS.push_back("luigi/luigi_s1_end1");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 67
	tempS.push_back("luigi/luigi_lvlup");
	sLuigi.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
}

Luigi::~Luigi()
{
	for (std::vector<AniSprite*>::iterator i = sLuigi.begin(); i != sLuigi.end(); i++) {
		delete (*i);
	}
	delete pSkill;
}

void Luigi::update()
{
	bJumpPressed = (iIDPlayer == 1) ? CFG::keyUp : CFG::keySpace;
	Player::update();
	if (bUseSkill) {
		pSkill->update(fXPos + Player::getHitBoxX() / 2, fYPos + Player::getHitBoxY() / 2);
	}
	if (!pSkill->getTrigger()) setUseSkill(false);
}

void Luigi::draw(sf::RenderWindow& window)
{
	int fXPosCam = fXPos; 
	int fYPosCam = fYPos;
	
	if (iIDPlayer == 1) {
		fXPosCam += (Core::getMap()->getXPos() - Core::getMap()->getXPos(1));
		fYPosCam += (Core::getMap()->getYPos() - Core::getMap()->getYPos(1));
	}

	if (bUseSkill) pSkill->draw(window);

	if (!inLevelDownAnimation || Core::getMap()->getInEvent()) {
		// Super Luigi
		sLuigi[getSpriteID()]->getTexture()->draw(window, (int)fXPosCam, (int)fYPosCam + (Core::getMap()->getInEvent() ? 0 : 2), !moveDirection);
	}
	else {
		if (inLevelDownAnimationFrameID % 15 < (inLevelDownAnimationFrameID > 120 ? 7 : inLevelDownAnimationFrameID > 90 ? 9 : inLevelDownAnimationFrameID > 60 ? 11 : inLevelDownAnimationFrameID > 30 ? 13 : 14)) {
			sLuigi[getSpriteID()]->getTexture()->draw(window, (int)fXPosCam, (int)fYPosCam + (Core::getMap()->getInEvent() ? 0 : 2), !moveDirection);
		}
	}
}

void Luigi::useSkill(Minion* pMinion, float fXmap)
{
	float fXSkill = pSkill->getXPos() - fXmap;
	float fYSkill = pSkill->getYPos();
	if (pMinion->checkHorizontalOverlap(fYSkill - 2, fYSkill + pSkill->getHitBoxY() + 2)
		&& pMinion->checkVerticalOverlap(fXSkill - 2, fXSkill + pSkill->getHitBoxX() + 2)) {
		pMinion->killMinion();
	}
}

AniSprite* Luigi::getSprite()
{
	return sLuigi[1 + 11 * powerLVL];
}

std::string Luigi::getStringName()
{
	return "LUIGI";
}
