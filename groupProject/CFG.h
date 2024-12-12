#pragma once

#include "Text.h"
#include "MenuManager.h"
#include <SFML/Audio.hpp>

class CFG
{
public:  
    CFG();
    ~CFG();
    static int GameWidth, GameHeight;
	static bool keySpace;
	static int keyIDA, keyIDS, keyIDD, keyIDSpace, keyIDShift;
    static bool canMoveBackward;

    // Methods
    static sf::Music* getMusic();
    static Text* getText();
    static MenuManager* getMenuManager();
    static IMG* getSMBLOGO();
private:
    static sf::Music* music;
    static Text* oText;
    static MenuManager* oMM;
    static IMG* tSMBLOGO;
};