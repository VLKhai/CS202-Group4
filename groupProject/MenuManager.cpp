
#include "MenuManager.h"
#include "Core.h"

MenuManager::MenuManager()
{
	this->currentGameState = eMainMenu;
	this->oMainMenu = new MainMenu();
}

MenuManager::~MenuManager()
{
	delete activeOption;
	delete oMainMenu;
}

void MenuManager::update()
{
	switch (currentGameState) {
	case eMainMenu:// contain list of options + logo
		oMainMenu->Update();
		Core::getMap()->updateGifBlocks();
		break;
	}
}

void MenuManager::draw(sf::RenderWindow& window)
{
	switch (currentGameState) {
	case eMainMenu:
		Core::getMap()->drawMap(window);
		Core::getMap()->getPlayer()->draw(window);
		//Core::getMap()->DrawGameLayout(window);
		oMainMenu->Draw(window);
		break;
	}
}

void MenuManager::setBackgroundColor(sf::RenderWindow& window) {
	switch (currentGameState) {
	case eMainMenu:
		Core::getMap()->setBackgroundColor(window);
		break;
	}
}