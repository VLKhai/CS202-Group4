#include "CFG.h"
#include <SFML/Graphics.hpp>

/* ******************************************** */

CFG::CFG() { }

CFG::~CFG() {
    delete music;
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