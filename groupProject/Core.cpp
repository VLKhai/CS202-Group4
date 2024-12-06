#include <SFML/Graphics.hpp>
#include <iostream>

#include "Core.h"

Map* Core::pMap = new Map();
bool Core::mouseLeftPressed = false;
bool Core::mouseRightPressed = false;
int Core::mouseX = 0;
int Core::mouseY = 0;
bool Core::quitGame = false;
sf::Clock Core::coreClock;

bool Core::movePressed = false;
bool Core::keyMenuPressed = false;
bool Core::keyS = false;
bool Core::keyW = false;
bool Core::keyA = false;
bool Core::keyD = false;
bool Core::keyShift = false;
bool Core::keyAPressed = false;
bool Core::keyDPressed = false;

Core::Core()
{   //1000 X 560 or 800 x 448
    this->mainWindow.create(sf::VideoMode(1000, 560), "Mario Game");
	this->mainWindow.setFramerateLimit(60);

	this->pMap = new Map(this->mainWindow);

    this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;
    this->keyAPressed = this->keyDPressed = this->firstDir = false;

    this->mouseX = this->mouseY = 0;

    CFG::keyIDA = sf::Keyboard::A;
    CFG::keyIDD = sf::Keyboard::D;
    CFG::keyIDS = sf::Keyboard::S;
    CFG::keyIDSpace = sf::Keyboard::Space;
    CFG::keyIDShift = sf::Keyboard::LShift;
}

void Core::mainLoop()
{
    while (mainWindow.isOpen()) {
        input();
		update();
        draw();       
    }
}

void Core::update()
{
	pMap->getPlayer()->update();
}

void Core::draw()
{
	//CFG::getMM()->draw(mainWindow);
	//pMap->draw(mainWindow);
	mainWindow.clear(sf::Color::White);
    pMap->draw(mainWindow);
	mainWindow.display();
}

Map* Core::getMap()
{
    return pMap;
}

void Core::input()
{
    while (mainWindow.pollEvent(mainEvent)) {
        if (mainEvent.type == sf::Event::Closed) {
            mainWindow.close();
        }
        inputPlayer();
        //resetMove();
    }
}

void Core::inputPlayer() {
    
    if (mainEvent.type == sf::Event::KeyReleased) {
 
        // Handle [D] key
        if (mainEvent.key.code == CFG::keyIDD) {
            if (firstDir) firstDir = false;
            keyDPressed = false;
        }

        // Handle [S] key
        if (mainEvent.key.code == CFG::keyIDS) {
            pMap->getPlayer()->setSquat(false); // Why is false?
            keyS = false;
        }

        // Handle [A] key
        if (mainEvent.key.code == CFG::keyIDA) {
            if (!firstDir) firstDir = true;
            keyAPressed = false;
        }
        
        // Handle [Space] key
        if (mainEvent.key.code == CFG::keyIDSpace) {
			CFG::keySpace = false;
        }
    
        // Handle [LShift] key
        if (mainEvent.key.code == CFG::keyIDShift) {
            if(!keyShift) {
				pMap->getPlayer()->jump();
				CFG::keySpace = true;
			}
        }
    }

    if (mainEvent.type == sf::Event::KeyPressed) {
        if (mainEvent.key.code == CFG::keyIDD) {
            keyDPressed = true;
            if (!keyAPressed) firstDir = true;
        }

        if (mainEvent.key.code == CFG::keyIDS) {
            if (!keyS) {
                keyS = true;
				pMap->getPlayer()->setSquat(true);
            }
        }

        if (mainEvent.key.code == CFG::keyIDA) {
            keyAPressed = true;
            if (!keyDPressed) firstDir = false;
        }

        if (mainEvent.key.code == CFG::keyIDSpace) {
			if (!CFG::keySpace) {
				pMap->getPlayer()->jump();
				CFG::keySpace = true;
			}
        }

        if (mainEvent.key.code == CFG::keyIDShift && !keyShift) {
			pMap->getPlayer()->startRun();
            keyShift = true;
        }
            
    }   

    if (keyAPressed) {
        if (!pMap->getPlayer()->getMove() && firstDir == false
            && !pMap->getPlayer()->getChangeMoveDirection()
            && !pMap->getPlayer()->getSquat()) {
			
            pMap->getPlayer()->startMove();
			pMap->getPlayer()->setMoveDirection(false);
        }
        else if (!keyDPressed && pMap->getPlayer()->getMoveSpeed() > 0
            && firstDir != pMap->getPlayer()->getMoveDirection()) {
			
            pMap->getPlayer()->setChangeMoveDirection();
        }
    
    }

    if (keyDPressed) {
        if (!pMap->getPlayer()->getMove() && firstDir == true
            && !pMap->getPlayer()->getChangeMoveDirection() 
            && !pMap->getPlayer()->getSquat()) {
            
            pMap->getPlayer()->startMove();
            pMap->getPlayer()->setMoveDirection(true);
        }
        else if (!keyAPressed && pMap->getPlayer()->getMoveSpeed() > 0 
            && firstDir != pMap->getPlayer()->getMoveDirection()) {
            
            pMap->getPlayer()->setChangeMoveDirection();
        }
    }

    // For Inertia Movement
    if (pMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
        pMap->getPlayer()->resetMove();
    }
}

void Core::mouseInput()
{
}

void Core::inputMenu()
{
}

void Core::resetMove()
{
    this->keyAPressed = this->keyDPressed = false;
}
