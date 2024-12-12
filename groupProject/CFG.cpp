#include "CFG.h"
#include <SFML/Graphics.hpp>

/* ******************************************** */

Text* CFG::oText = new Text();
IMG* CFG::tSMBLOGO = new IMG();

MenuManager* CFG::oMM = new MenuManager();

CFG::CFG() { }

CFG::~CFG() {
    delete music;
    delete oMM;
    delete oText;
}

/* ******************************************** */

int CFG::GameWidth  = 1000;
int CFG::GameHeight = 560;

bool CFG::keySpace = false;

int CFG::keyIDA = 0;
int CFG::keyIDD = 0;
int CFG::keyIDS = 0;
int CFG::keyIDSpace = 0;
int CFG::keyIDShift = 0;

bool CFG::canMoveBackward = true;

sf::Music* CFG::music = new sf::Music();

/* ******************************************** */

sf::Music* CFG::getMusic() {
    return music;
}

Text* CFG::getText() {
    return oText;
}

MenuManager* CFG::getMenuManager() {
    return oMM;
}

IMG* CFG::getSMBLOGO() {
    return tSMBLOGO;
}