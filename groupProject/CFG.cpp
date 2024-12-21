#include "CFG.h"
#include <SFML/Graphics.hpp>

/* ******************************************** */

Text* CFG::oText = new Text();
IMG* CFG::tSMBLOGO = new IMG();

MenuManager* CFG::oMM = new MenuManager();
Music* CFG::oMusic = new Music();

CFG::CFG() { }

CFG::~CFG() {
    delete oMusic;
    delete oMM;
    delete oText;
}

/* ******************************************** */

int CFG::GameWidth  = 1000;
int CFG::GameHeight = 560;




int CFG::keyIDA = 0;
int CFG::keyIDD = 0;
int CFG::keyIDS = 0;
int CFG::keyIDSpace = 0;
int CFG::keyIDShift = 0;
int CFG::keyIDSkill = 0;

bool CFG::canMoveBackward = true;
bool CFG::keySpace = false;
bool CFG::keyEnter = false;
bool CFG::keyLeft = false;
bool CFG::keyRight = false;
bool CFG::keySkill = false;

/* ******************************************** */

Music* CFG::getMusic() {
    return oMusic;
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

std::string CFG::getKeyString(int keyID) {
   if (keyID >= 97 && keyID <= 122) {
        return std::string(1, '0' + (keyID - 32) - 48);
    }

    if (keyID >= 48 && keyID < 57) {
        return std::string(1, '0' + (keyID - 32) - 48);
    }

    switch (keyID) {
    case sf::Keyboard::Escape:
        return "ESCAPE";
    case sf::Keyboard::Space:
        return "SPACE";
    case sf::Keyboard::LShift:
        return "LSHIFT";
    case sf::Keyboard::RShift:
        return "RSHIFT";
    case sf::Keyboard::A:
        return "A";
    case sf::Keyboard::S:
        return "S";
    case sf::Keyboard::D:
        return "D";
    case sf::Keyboard::Left:
        return "LEFT";
    case sf::Keyboard::LControl:
        return "LCTRL";
    case sf::Keyboard::RControl:
        return "RCTRL";
    }
    return "NONE";
}