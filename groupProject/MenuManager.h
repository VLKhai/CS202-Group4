#pragma once
#include <SFML/Graphics.hpp>

class MenuManager {
private:
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
};