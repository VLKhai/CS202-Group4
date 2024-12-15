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

std::string CFG::getKeyString(int keyID) {
    if (keyID >= 97 && keyID <= 122) {
        return std::string(1, '0' + (keyID - 32) - 48);
    }

    if (keyID >= 48 && keyID <= 57) {
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
    case sf::Keyboard::Up:
        return "UP";
    case sf::Keyboard::Down:
        return "DOWN";
    case sf::Keyboard::Right:
        return "RIGHT";
    case sf::Keyboard::Left:
        return "LEFT";
    case sf::Keyboard::LControl:
        return "LCTRL";
    case sf::Keyboard::RControl:
        return "RCTRL";

        return "NONE";
    }
}