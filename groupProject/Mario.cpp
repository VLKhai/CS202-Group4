#include "Mario.h"
#include "Core.h"

Mario::Mario(sf::RenderWindow& window, float fXPos, float fYPos) : Player(window, fXPos, fYPos) {
	limSmallJumpBlock = 2;
	limBigJumpBlock = 4;

	// LOAD SPRITE
	std::vector<std::string> tempS;
	std::vector<unsigned int> tempI;
	// ----- 0
	tempS.push_back("mario/mario_death");
	tempI.push_back(0);
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 1
	tempS.push_back("mario/mario");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 2
	tempS.push_back("mario/mario_move0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 3
	tempS.push_back("mario/mario_move1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 4
	tempS.push_back("mario/mario_move2");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 5
	tempS.push_back("mario/mario_jump");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 6
	tempS.push_back("mario/mario_st");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 7
	tempS.push_back("mario/mario"); // SQUAT
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 8
	tempS.push_back("mario/mario_underwater0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 9
	tempS.push_back("mario/mario_underwater1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 10
	tempS.push_back("mario/mario_end");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 11
	tempS.push_back("mario/mario_end1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ---------- BIG
	// ----- 12
	tempS.push_back("mario/mario1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 13
	tempS.push_back("mario/mario1_move0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 14
	tempS.push_back("mario/mario1_move1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 15
	tempS.push_back("mario/mario1_move2");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 16
	tempS.push_back("mario/mario1_jump");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 17
	tempS.push_back("mario/mario1_st");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 18
	tempS.push_back("mario/mario1_squat"); // SQUAT
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 19
	tempS.push_back("mario/mario1_underwater0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 20
	tempS.push_back("mario/mario1_underwater1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 21
	tempS.push_back("mario/mario1_end");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 22
	tempS.push_back("mario/mario1_end1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- 23
	tempS.push_back("mario/mario2");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 24
	tempS.push_back("mario/mario2_move0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 25
	tempS.push_back("mario/mario2_move1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 26
	tempS.push_back("mario/mario2_move2");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 27
	tempS.push_back("mario/mario2_jump");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 28
	tempS.push_back("mario/mario2_st");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 29
	tempS.push_back("mario/mario2_squat"); // SQUAT
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 30
	tempS.push_back("mario/mario2_underwater0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 31
	tempS.push_back("mario/mario2_underwater1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 32
	tempS.push_back("mario/mario2_end");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 33
	tempS.push_back("mario/mario2_end1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- 34
	tempS.push_back("mario/mario2s");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 35
	tempS.push_back("mario/mario2s_move0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 36
	tempS.push_back("mario/mario2s_move1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 37
	tempS.push_back("mario/mario2s_move2");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 38
	tempS.push_back("mario/mario2s_jump");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 39
	tempS.push_back("mario/mario2s_st");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 40
	tempS.push_back("mario/mario2s_squat"); // SQUAT
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 41
	tempS.push_back("mario/mario2s_underwater0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 42
	tempS.push_back("mario/mario2s_underwater1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 43
	tempS.push_back("mario/mario2s_end");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 44
	tempS.push_back("mario/mario2s_end1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- 45
	tempS.push_back("mario/mario_s0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 46
	tempS.push_back("mario/mario_s0_move0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 47
	tempS.push_back("mario/mario_s0_move1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 48
	tempS.push_back("mario/mario_s0_move2");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 49
	tempS.push_back("mario/mario_s0_jump");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 50
	tempS.push_back("mario/mario_s0_st");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 51
	tempS.push_back("mario/mario_s0"); // SQUAT
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 52
	tempS.push_back("mario/mario_s0_underwater0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 53
	tempS.push_back("mario/mario_s0_underwater1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 54
	tempS.push_back("mario/mario_s0_end");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 55
	tempS.push_back("mario/mario_s0_end1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- 56
	tempS.push_back("mario/mario_s1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 57
	tempS.push_back("mario/mario_s1_move0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 58
	tempS.push_back("mario/mario_s1_move1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 59
	tempS.push_back("mario/mario_s1_move2");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 60
	tempS.push_back("mario/mario_s1_jump");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 61
	tempS.push_back("mario/mario_s1_st");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 62
	tempS.push_back("mario/mario_s1"); // SQUAT
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 63
	tempS.push_back("mario/mario_s1_underwater0");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 64
	tempS.push_back("mario/mario_s1_underwater1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 65
	tempS.push_back("mario/mario_s1_end");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
	// ----- 66
	tempS.push_back("mario/mario_s1_end1");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();

	// ----- LOAD SPRITE

	// ----- 67
	tempS.push_back("mario/mario_lvlup");
	sMario.push_back(new AniSprite(window, tempS, tempI, true));
	tempS.clear();
}

Mario::~Mario()
{
	for (std::vector<AniSprite*>::iterator i = sMario.begin(); i != sMario.end(); i++) {
		delete (*i);
	}
}

void Mario::draw(sf::RenderWindow& window)
{
	if (!inLevelDownAnimation || Core::getMap()->getInEvent()) {
		// Super Mario
		sMario[getSpriteID()]->getTexture()->draw(window, (int)fXPos, (int)fYPos + (Core::getMap()->getInEvent() ? 0 : 2), !moveDirection);
	}
	else {
		if (inLevelDownAnimationFrameID % 15 < (inLevelDownAnimationFrameID > 120 ? 7 : inLevelDownAnimationFrameID > 90 ? 9 : inLevelDownAnimationFrameID > 60 ? 11 : inLevelDownAnimationFrameID > 30 ? 13 : 14)) {
			sMario[getSpriteID()]->getTexture()->draw(window, (int)fXPos, (int)fYPos + (Core::getMap()->getInEvent() ? 0 : 2), !moveDirection);
		}
	}
}

AniSprite* Mario::getSprite()
{
	return sMario[1 + 11 * powerLVL];
}

std::string Mario::getStringName()
{
	return "MARIO";
}
