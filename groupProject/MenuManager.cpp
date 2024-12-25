
#include "MenuManager.h"
#include "Core.h"

MenuManager::MenuManager()
{
	this->currentGameState = eMainMenu;

	vMenu.push_back(new MainMenu());
	vMenu.push_back(new LoadingMenu());
	vMenu.push_back(new OffMenu());
	vMenu.push_back(new OptionsMenu());
	vMenu.push_back(new PauseMenu());
	
	pMenu = vMenu[currentGameState];
}

MenuManager::~MenuManager()
{
	for (Menu* pMenu : vMenu) {
		delete pMenu;
	}
}

void MenuManager::update()
{
	pMenu->update();
}

void MenuManager::draw(sf::RenderWindow& window)
{
	pMenu->draw(window);
}

void MenuManager::setBackgroundColor(sf::RenderWindow& window) {
	pMenu->setBackgroundColor(window);
}

void MenuManager::enter() {
	pMenu->enter();
}

void MenuManager::escape() {
	pMenu->escape();
}

void MenuManager::resetActiveOptionID(gameState ID) {
	pMenu->resetActiveOptionID();
}

void MenuManager::keyPressed(int iDir) {
	pMenu->updateActiveButton(iDir);
}

int MenuManager::getViewID() {
	return currentGameState;
}

IMG* MenuManager::getActiveOption() {
	return activeOption;
}

LoadingMenu* MenuManager::getLoadingMenu() {
	return dynamic_cast<LoadingMenu*>(vMenu[1]);
}

void MenuManager::setViewID(gameState viewID) {
	this->currentGameState = viewID;
	this->pMenu = vMenu[viewID];
}


void MenuManager::setKey(int keyID) {
	pMenu->setKey(keyID);
}

void MenuManager::setActiveOption(sf::RenderWindow& window) {
	activeOption = new IMG("active_option", window);
}

OptionsMenu* MenuManager::getOptions() {
	return dynamic_cast<OptionsMenu*>(vMenu[3]);
}
