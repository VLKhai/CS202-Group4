#include "LoadingMenu.h"
#include "CFG.h"
#include "Core.h"

/* ******************************************** */

LoadingMenu::LoadingMenu(void) {
	this->iTime = 0;
	this->loadingType = true;
}

LoadingMenu::~LoadingMenu(void) {

}

/* ******************************************** */

void LoadingMenu::update() {
	if (CFG::keyLeft) {
		Core::getMap()->moveSelectPlayer(-1);
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);
		Core::resetKeys();
	}
	else if (CFG::keyRight) {
		Core::getMap()->moveSelectPlayer(1);
		CFG::getMusic()->PlayChunk(CFG::getMusic()->cCOIN);
		Core::resetKeys();
	}
	
	if (CFG::keyEnter ||
		(Core::coreClock.getElapsedTime().asMilliseconds() >= iTime + 5250 && !loadingType)) { // Game Over
		if (loadingType) {
			if (!Core::getMap()->getInEvent()) {
				Core::getMap()->setSpawnPoint();
				Core::getMap()->loadLVL();
			}
			Core::getMap()->loadLVL();
			CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGame);
			CFG::getMusic()->changeMusic(true, true);
		}
		else {
			Core::getMap()->resetGameData();
			CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eMainMenu);
		}
	}
	else {
		CFG::getMusic()->StopMusic();
	}
	Core::getMap()->updateGifBlocks();
}

void LoadingMenu::draw(sf::RenderWindow& window) {
	if (loadingType) {
		Core::getMap()->DrawGameLayout(window);
		CFG::getText()->DrawCenterX(window, "WORLD " + Core::getMap()->getLevelName(), 210, 16);

		CFG::getText()->Draw(window, Core::getMap()->getPlayer()->getStringName(), 400-20, 240+30, 8);
		Core::getMap()->getPlayer()->getSprite()->getTexture()->draw(window, 400+30, 270 - Core::getMap()->getPlayer()->getHitBoxY() / 2); 

		CFG::getText()->Draw(window, "y", 495, 265);
		if (Core::getMap()->getPlayer()->getNumOfLives() > 9) {
			Core::getMap()->getBlock(180)->getSprite()->getTexture()->draw(window, 500, 180);
		}
		CFG::getText()->Draw(window, std::to_string(Core::getMap()->getPlayer()->getNumOfLives()), 550, 260, 24);
		
		CFG::getText()->DrawCenterX(window, "PRESS <- -> TO SELECT CHARACTER", 400, 16);
		CFG::getText()->DrawCenterX(window, "PRESS ENTER TO CONTINUE THE NEXT LEVEL", 430, 16);
		CFG::getText()->DrawCenterX(window, "REMEMBER THAT YOU CAN RUN WITH " + CFG::getKeyString(CFG::keyIDShift), 460, 16);
	}
	else {
		Core::getMap()->DrawGameLayout(window);
		CFG::getText()->DrawCenterX(window, "GAME OVER", 240, 16);
	}
}

/* ******************************************** */

void LoadingMenu::updateTime() {
	this->iTime = Core::coreClock.getElapsedTime().asMilliseconds();
}

void LoadingMenu::setBackgroundColor(sf::RenderWindow& window)
{
	window.clear(sf::Color(0, 0, 0, 255));
}
