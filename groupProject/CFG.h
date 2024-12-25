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
	static bool keySpace, keyEnter, keyLeft, keyRight, keySkill;
	static int keyIDA, keyIDS, keyIDD, keyIDSpace, keyIDShift, keyIDSkill;

	static int keyIDRight, keyIDLeft, keyIDUp, keyIDDown, keyIDShiftRight, keyIDSkill2;
    static bool keyUp, keySkill2;

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