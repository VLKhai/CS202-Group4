#include <SFML/Graphics.hpp>

#include "Core.h"

Map* Core::pMap = new Map();
bool Core::mouseLeftPressed = false;
bool Core::mouseRightPressed = false;
int Core::mouseX = 0;
int Core::mouseY = 0;
bool Core::quitGame = false;

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
}

void Core::mainLoop()
{
    // std::vector<std::string> sFilename = {"goombas_0", "goombas_1"};
    // std::vector<unsigned int> iDelay = {200, 200};
    // AniSprite goomba(this->mainWindow, sFilename, iDelay, false);
    // Block block(0, &goomba, true, true, true, true);

    // std::vector<std::string> sFilename1 = {"koopa_0", "koopa_1"};
    // std::vector<unsigned int> iDelay1 = {200, 200};
    // AniSprite koopa(this->mainWindow, sFilename1, iDelay1, false);
    // Block block1(0, &koopa, true, true, true, true);

    // std::vector<std::string> sFilename2 = {"koopa_2", "koopa_3"};
    // std::vector<unsigned int> iDelay2 = {200, 200};
    // AniSprite koopa2(this->mainWindow, sFilename2, iDelay2, false);
    // Block block2(0, &koopa2, true, true, true, true);

    // Start the game loop
    while (mainWindow.isOpen()) {
        while (mainWindow.pollEvent(mainEvent)) {
            if (mainEvent.type == sf::Event::Closed) {
                mainWindow.close();
            }
            // Handle input events here
            inputPlayer();
        }
        // Draw here
        mainWindow.clear(sf::Color::White);

        mainWindow.display();
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
            if(!CFG::keySpace) {
				pMap->getPlayer()->jump();
				CFG::keySpace = true;
			}
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
        if (mainEvent.key.code == sf::Keyboard::D) {
            keyDPressed = true;
            if (!keyAPressed) firstDir = true;
        }

        if (mainEvent.key.code == sf::Keyboard::S) {
            if (!keyS) {
                keyS = true;
                //setSquat(); 
            }
        }

        if (mainEvent.key.code == sf::Keyboard::A) {
            keyAPressed = true;
            if (!keyDPressed) firstDir = false;
        }

        if (mainEvent.key.code == sf::Keyboard::Space) {
            CFG::keySpace = false;
        }
    }   

    if (keyAPressed) {}

    if (keyDPressed) {}

    // For Inertia Movement
    if (pMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
        pMap->getPlayer()->resetMove();
    }

}