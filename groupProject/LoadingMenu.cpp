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

void LoadingMenu::Update() {
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

void LoadingMenu::Draw(sf::RenderWindow& window) {
	if (loadingType) {
		Core::getMap()->DrawGameLayout(window);
		CFG::getText()->Draw(window, "WORLD", 400, 130); 
		CFG::getText()->Draw(window, Core::getMap()->getLevelName(), 500, 130);

		Core::getMap()->getPlayer()->getSprite()->getTexture()->draw(window, 400, 180 - Core::getMap()->getPlayer()->getHitBoxY() / 2); 

		CFG::getText()->Draw(window, "y", 470, 176);

		if (Core::getMap()->getPlayer()->getNumOfLives() > 9) {
			Core::getMap()->getBlock(180)->getSprite()->getTexture()->draw(window, 500, 180);
		}
		CFG::getText()->Draw(window, std::to_string(Core::getMap()->getPlayer()->getNumOfLives()), 520, 180);
		CFG::getText()->DrawCenterX(window, "REMEMBER THAT YOU CAN RUN WITH " + CFG::getKeyString(CFG::keyIDShift), 400, 16);
		CFG::getText()->DrawCenterX(window, "PRESS ENTER TO CONTINUE THE NEXT LEVEL", 430, 16);
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