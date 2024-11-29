#pragma once

#include "Map.h"

class Core {
public: 
    Core();
    void mainLoop();
    //void update();
    //void draw();

    static bool quitGame;
    static bool mouseLeftPressed, mouseRightPressed;
	static int mouseX, mouseY;
    

private:
    sf::RenderWindow mainWindow;
    sf::Event mainEvent;

    // ----- INPUT
	static bool movePressed, keyMenuPressed, keyS, keyW, keyA, keyD, keyShift;
	static bool keyAPressed, keyDPressed;
    bool firstDir;

    static Map* pMap;
    void inputPlayer();
};
