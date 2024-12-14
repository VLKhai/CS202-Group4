#pragma once
#include "MainMenu.h"
#include "IMG.h"

class MenuManager {
private:
	IMG* activeOption;

	MainMenu* oMainMenu;
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
};