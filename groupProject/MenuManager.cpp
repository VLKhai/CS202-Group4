
#include "MenuManager.h"
#include "Core.h"

MenuManager::MenuManager()
{
	this->currentGameState = eMainMenu;
	//this->oMainMenu = new MainMenu();
	//this->oLoadingMenu = new LoadingMenu();
	//this->oOptionsMenu = new OptionsMenu();
	//this->oPauseMenu = new PauseMenu();
	vMenu.push_back(new MainMenu());
	vMenu.push_back(new LoadingMenu());
	vMenu.push_back(new OffMenu());
	vMenu.push_back(new OptionsMenu());
	vMenu.push_back(new PauseMenu());

	pMenu = vMenu[currentGameState];

}

MenuManager::~MenuManager()
{
	//delete activeOption;
	//delete oMainMenu;
	//delete oLoadingMenu;
	//delete oOptionsMenu;
	//delete oPauseMenu;
}

void MenuManager::update()
{
	//switch (currentGameState) {
	//case eMainMenu:// contain list of options + logo
	//	oMainMenu->Update();
	//	break;
	//case eGameLoading:// loading screen after choosing level
	//	oLoadingMenu->Update();
	//	break;
	//case eGame:// game screen
	//	Core::getMap()->update();
	//	Core::getMap()->updateMinionsCollisions();
	//	//oLE->Update();
	//	break;
	//case eOptions:// options screen
	//	oOptionsMenu->Update();
	//	break;
	//case ePasue:// pause screen
	//	oPauseMenu->Update();
	//	break;
	//}
	pMenu->update();
}

void MenuManager::draw(sf::RenderWindow& window)
{
	//switch (currentGameState) {
	//case eMainMenu:
	//	oMainMenu->Draw(window);
	//	break;
	//case eGameLoading:
	//	oLoadingMenu->Draw(window);
	//	break;
	//case eGame:
	//	Core::getMap()->draw(window);
	//	//oLE->Draw(rR);
	//	break;
	//case eOptions:
	//	oOptionsMenu->Draw(window);
	//	break;
	//case ePasue:
	//	oPauseMenu->Draw(window);
	//	break;
	//}
	pMenu->draw(window);
}

void MenuManager::setBackgroundColor(sf::RenderWindow& window) {
	//switch (currentGameState) {
	//case eMainMenu:
	//	Core::getMap()->setBackgroundColor(window);
	//	break;
	//case eGameLoading:
	//	window.clear(sf::Color(0, 0, 0, 255));
	//	break;
	//case eGame:
	//	Core::getMap()->setBackgroundColor(window);
	//	break;
	//}
	pMenu->setBackgroundColor(window);
}

void MenuManager::enter() {
	//switch (currentGameState) {
	//case eMainMenu:
	//	oMainMenu->enter();
	//	break;
	//case eGame:
	//	Core::getMap()->setDrawLines(!Core::getMap()->getDrawLines());
	//	break;
	//case eOptions:
	//	oOptionsMenu->enter();
	//	break;
	//case ePasue:
	//	oPauseMenu->enter();
	//	break;
	//}
	pMenu->enter();
}

void MenuManager::escape() {
	//switch (currentGameState) {
	//case eGame:
	//	break;
	//case eOptions:
	//	oOptionsMenu->escape();
	//	break;
	//case eMainMenu:
	//	oMainMenu->escape();
	//	break;
	//case ePasue:
	//	oPauseMenu->escape();
	//	break;
	//}
	pMenu->escape();
}

void MenuManager::resetActiveOptionID(gameState ID) {
	//switch (ID) {
	//case eMainMenu:
	//	oMainMenu->activeMenuOption = 0;
	//	break;
	//case eOptions:
	//	oOptionsMenu->activeMenuOption = 0;
	//	break;
	//case ePasue:
	//	oPauseMenu->activeMenuOption = 0;
	//	break;
	//}
	pMenu->resetActiveOptionID();
}

void MenuManager::keyPressed(int iDir) {
	//switch (currentGameState) {
	//case eMainMenu:
	//	oMainMenu->updateActiveButton(iDir);
	//	break;
	//case eOptions:
	//	oOptionsMenu->updateActiveButton(iDir);
	//	break;
	//case ePasue:
	//	oPauseMenu->updateActiveButton(iDir);
	//	break;
	//}
	pMenu->updateActiveButton(iDir);
}

int MenuManager::getViewID() {
	return currentGameState;
}

IMG* MenuManager::getActiveOption() {
	return activeOption;
}

LoadingMenu* MenuManager::getLoadingMenu() {
	//return oLoadingMenu;
	return dynamic_cast<LoadingMenu*>(vMenu[1]);
}

void MenuManager::setViewID(gameState viewID) {
	this->currentGameState = viewID;
	this->pMenu = vMenu[viewID];
}


void MenuManager::setKey(int keyID) {
	/*switch (currentGameState) {
	case eOptions:
		oOptionsMenu->setKey(keyID);
		break;
	}*/
	pMenu->setKey(keyID);
}

void MenuManager::setActiveOption(sf::RenderWindow& window) {
	activeOption = new IMG("active_option", window);
}

OptionsMenu* MenuManager::getOptions() {
	return dynamic_cast<OptionsMenu*>(vMenu[3]);
}
