#include "CFG.h"

/* ******************************************** */

CFG::CFG() { }

CFG::~CFG() {
    delete music;
}

/* ******************************************** */

int CFG::GameWidth  = 1000;
int CFG::GameHeight = 560;

bool CFG::keySpace = false;
int CFG::keyIDA = sf::Keyboard::A;
int CFG::keyIDD = sf::Keyboard::D;
int CFG::keyIDS = sf::Keyboard::S;
int CFG::keyIDSpace = sf::Keyboard::Space;
int CFG::keyIDShift = sf::Keyboard::LShift;

bool CFG::canMoveBackward = true;

/* ******************************************** */

sf::Music* CFG::getMusic() {
    return music;
}