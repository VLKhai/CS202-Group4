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
	static bool keyAPressed, keyDPressed, keyLeftPressed, keyRightPressed;
    bool firstDir1, firstDir2;

    static bool keyUp, keyDown, keyShiftRight;

    static Map* pMap;
   
    void input();
    void inputPlayer();
    void inputPlayerPressed(bool pressA, bool pressD, bool firstD, Player* pPlayer);
    void mouseInput();
    void inputMenu();

};