
#include "MenuManager.h"
#include "Core.h"

MenuManager::MenuManager()
{
	this->currentGameState = eMainMenu;
	this->oMainMenu = new MainMenu();
	this->oLoadingMenu = new LoadingMenu();
	this->oOptionsMenu = new OptionsMenu();
}

MenuManager::~MenuManager()
{
	delete activeOption;
	delete oMainMenu;
	delete oLoadingMenu;
	delete oOptionsMenu;
}

void MenuManager::update()
{
	switch (currentGameState) {
	case eMainMenu:// contain list of options + logo
		oMainMenu->Update();
		Core::getMap()->updateGifBlocks();
		break;
	case eGameLoading:// loading screen after choosing level
		oLoadingMenu->Update();
		break;
	case eGame:// game screen
		Core::getMap()->update();
		Core::getMap()->updateMinionsCollisions();
		//oLE->Update();
		break;
	case eOptions:// options screen
		oOptionsMenu->Update();
		break;
	}
}

void MenuManager::draw(sf::RenderWindow& window)
{
	switch (currentGameState) {
	case eMainMenu:
		Core::getMap()->drawMap(window);
		Core::getMap()->getPlayer()->draw(window);
		Core::getMap()->DrawGameLayout(window);
		oMainMenu->Draw(window);
		break;
	case eGameLoading:
		oLoadingMenu->Draw(window);
		break;
	case eGame:
		Core::getMap()->draw(window);
		//oLE->Draw(rR);
		break;
	case eOptions:
		Core::getMap()->drawMap(window);
		Core::getMap()->DrawMinions(window);
		Core::getMap()->getPlayer()->draw(window);
		Core::getMap()->DrawGameLayout(window);
		oOptionsMenu->Draw(window);
		break;
	}
}

void MenuManager::setBackgroundColor(sf::RenderWindow& window) {
	switch (currentGameState) {
	case eMainMenu:
		Core::getMap()->setBackgroundColor(window);
		break;
	case eGameLoading:
		window.clear(sf::Color(0, 0, 0, 255));
		break;
	case eGame:
		Core::getMap()->setBackgroundColor(window);
		break;
	}
}

void MenuManager::enter() {
	switch (currentGameState) {
	case eMainMenu:
		oMainMenu->enter();
		break;
	case eGame:
		Core::getMap()->setDrawLines(!Core::getMap()->getDrawLines());
		break;
	case eOptions:
		oOptionsMenu->enter();
		break;
	}
}

void MenuManager::resetActiveOptionID(gameState ID) {
	switch (ID) {
	case eMainMenu:
		oMainMenu->activeMenuOption = 0;
		break;
	case eOptions:
		oOptionsMenu->activeMenuOption = 0;
		break;
	/*
	case ePasue:
		oPauseMenu->activeMenuOption = 0;
		break;*/
	}
}

void MenuManager::keyPressed(int iDir) {
	switch (currentGameState) {
	case eMainMenu:
		oMainMenu->updateActiveButton(iDir);
		break;
	case eOptions:
		oOptionsMenu->updateActiveButton(iDir);
		break;
	/*case ePasue:
		oPauseMenu->updateActiveButton(iDir);
		break;*/
	}
}

int MenuManager::getViewID() {
	return currentGameState;
}

IMG* MenuManager::getActiveOption() {
	return activeOption;
}

LoadingMenu* MenuManager::getLoadingMenu() {
	return oLoadingMenu;
}

void MenuManager::setViewID(gameState viewID) {
	this->currentGameState = viewID;
}

void MenuManager::setKey(int keyID) {
	switch (currentGameState) {
	case eOptions:
		oOptionsMenu->setKey(keyID);
		break;
	}
}

void MenuManager::setActiveOption(sf::RenderWindow& window) {
	activeOption = new IMG("active_option", window);
}
