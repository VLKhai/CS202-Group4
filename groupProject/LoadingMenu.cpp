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
	if (Core::coreClock.getElapsedTime().asMilliseconds() >= iTime + 2500 + (loadingType ? 0 : 2750)) {
		if (loadingType) {
			/*if (!Core::getMap()->getInEvent()) {
				Core::getMap()->setSpawnPoint();
				Core::getMap()->loadLVL();
			}*/
			Core::getMap()->loadLVL();
			CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eGame);
			//CFG::getMusic()->changeMusic(true, true);
		}
		else {
			//Core::getMap()->resetGameData();
			CFG::getMenuManager()->setViewID(CFG::getMenuManager()->eMainMenu);
		}
	}
	else {
		//CFG::getMusic()->StopMusic();
	}
	Core::getMap()->updateGifBlocks();
}

void LoadingMenu::Draw(sf::RenderWindow& window) {
	if (loadingType) {
		Core::getMap()->DrawGameLayout(window);
		CFG::getText()->Draw(window, "WORLD", 320, 144);
		CFG::getText()->Draw(window, Core::getMap()->getLevelName(), 416, 144);

		Core::getMap()->getPlayer()->getMarioSprite()->getTexture()->draw(window, 342, 210 - Core::getMap()->getPlayer()->getHitBoxY() / 2);

		CFG::getText()->Draw(window, "y", 384, 208);

		if (Core::getMap()->getPlayer()->getNumOfLives() > 9) {
			Core::getMap()->getBlock(180)->getSprite()->getTexture()->draw(window, 410, 210);
		}
		CFG::getText()->Draw(window, std::to_string(Core::getMap()->getPlayer()->getNumOfLives()), 432, 208);
		CFG::getText()->DrawCenterX(window, "REMEMBER THAT YOU CAN RUN WITH " + CFG::getKeyString(CFG::keyIDShift), 400, 16);
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