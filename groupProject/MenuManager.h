#pragma once
#include "MainMenu.h"
#include "LoadingMenu.h"
#include "IMG.h"
#include "OptionsMenu.h"
#include "PauseMenu.h"

class MenuManager {
private:
	IMG* activeOption;

	MainMenu* oMainMenu;
	LoadingMenu* oLoadingMenu;
	OptionsMenu* oOptionsMenu;
	PauseMenu* oPauseMenu;
public:
	MenuManager();
	~MenuManager();

	enum gameState {
		eMainMenu,
		eGameLoading,
		eGame,
		eAbout,
		eOptions,
		ePasue,
	};

	gameState currentGameState;

	void update();
	void draw(sf::RenderWindow& window);
	void setBackgroundColor(sf::RenderWindow& window);

	void enter();
	void escape();
	void setKey(int keyID);
	void keyPressed(int iDir);

	void resetActiveOptionID(gameState ID);

	int getViewID();
	void setViewID(gameState viewID);

	IMG* getActiveOption();
	void setActiveOption(sf::RenderWindow& window);

	LoadingMenu* getLoadingMenu(); 
	OptionsMenu* getOptions();
};