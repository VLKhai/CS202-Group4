#pragma once

#include "Text.h"
#include "MenuManager.h"
#include "Music.h"

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
    static Text* getText();
    static MenuManager* getMenuManager();
    static IMG* getSMBLOGO();
    static Music* getMusic();

    static std::string getKeyString(int keyID);
private:
    static Text* oText;
    static MenuManager* oMM;
    static IMG* tSMBLOGO;
    static Music* oMusic;
};