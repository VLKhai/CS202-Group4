#pragma once

#include "Map.h"

class Core {
public: 
    Core();

    static bool quitGame;
    static bool mouseLeftPressed, mouseRightPressed;
	static int mouseX, mouseY;
	static sf::Clock coreClock;

	// Methods
    void mainLoop();
    void update();
    void draw();

    static Map* getMap();

    void resetMove();
    static void resetKeys();

private:
    sf::RenderWindow mainWindow;
    sf::Event mainEvent;

    // ----- INPUT
	static bool movePressed, keyMenuPressed, keyS, keyW, keyA, keyD, keyShift;
	static bool keyAPressed, keyDPressed;
    bool firstDir;

    static Map* pMap;
   
    void input();
    void inputPlayer();
    void mouseInput();
    void inputMenu();
};