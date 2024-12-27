#include <SFML/Graphics.hpp>
#include <iostream>

#include "Core.h"


Map* Core::pMap;
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

bool Core::keyLeftPressed = false;
bool Core::keyRightPressed = false;
bool Core::keyUp = false;
bool Core::keyDown = false;
bool Core::keyShiftRight = false;

Core::Core()
{   //1000 X 560 or 800 x 448
    this->mainWindow.create(sf::VideoMode(CFG::GameWidth, CFG::GameHeight), "Mario Game");
	this->mainWindow.setFramerateLimit(60);
    
    this->pMap = &Map::Instance();
    this->pMap->loadMap(this->mainWindow);

    this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;
    this->keyAPressed = this->keyDPressed = this->firstDir1 = false;
	this->keyLeftPressed = this->keyRightPressed = this->firstDir2 = false;

    this->mouseX = this->mouseY = 0;

    CFG::keyIDA = sf::Keyboard::A;
    CFG::keyIDD = sf::Keyboard::D;
    CFG::keyIDS = sf::Keyboard::S;
    CFG::keyIDSpace = sf::Keyboard::Space;
    CFG::keyIDShift = sf::Keyboard::LShift;
	CFG::keyIDSkill = sf::Keyboard::E;

	CFG::keyIDLeft = sf::Keyboard::Left;
	CFG::keyIDRight = sf::Keyboard::Right;
	CFG::keyIDUp = sf::Keyboard::Up;
	CFG::keyIDDown = sf::Keyboard::Down;
	CFG::keyIDShiftRight = sf::Keyboard::RShift;
	CFG::keyIDSkill2 = sf::Keyboard::RAlt;

	CFG::keySpace = false;
	CFG::keyEnter = false;
	CFG::keyLeft = false;
	CFG::keyRight = false;
    CFG::getText()->setFont(mainWindow, "font");
    CFG::getSMBLOGO()->setIMG("super_mario_bros", mainWindow);
    CFG::getMenuManager()->setActiveOption(mainWindow); 
    CFG::getMusic()->PlayMusic();
}

void Core::mainLoop()
{
    while (mainWindow.isOpen()) {
        CFG::getMenuManager()->setBackgroundColor(mainWindow);
        input();
		update();
        draw();       
        if (quitGame) {
            mainWindow.close();
        }
    }
}

void Core::update()
{
    CFG::getMenuManager()->update();
}

void Core::draw()
{
    CFG::getMenuManager()->draw(mainWindow);
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
        inputMenu();
    }
}

void Core::inputPlayer() {
    // Pause game when focus is lost

    if (mainEvent.type == sf::Event::KeyReleased) {
       
        // Handle [D] key
        if (mainEvent.key.code == CFG::keyIDD) {
            if (firstDir1) firstDir1 = false;
            keyDPressed = false;
        }

        // Handle [S] key
        if (mainEvent.key.code == CFG::keyIDS) {
            pMap->getPlayer()->setSquat(false);
            keyS = false;
        }

        // Handle [A] key
        if (mainEvent.key.code == CFG::keyIDA) {
            if (!firstDir1) firstDir1 = true;
            keyAPressed = false;
        }
        
        // Handle [Space] key
        if (mainEvent.key.code == CFG::keyIDSpace) {
			CFG::keySpace = false;
        }
    
        // Handle [LShift] key
        if (mainEvent.key.code == CFG::keyIDShift) {
            if (keyShift) {
                pMap->getPlayer()->resetRun();
                keyShift = false;
            }
        }

        if (mainEvent.key.code == CFG::keyIDSkill) {
            CFG::keySkill = false;
        }

        if (pMap->getNumOfPlayers() == 2) {
            // Handle [D] key
            if (mainEvent.key.code == CFG::keyIDRight) {
                if (firstDir2) firstDir2 = false;
                keyRightPressed = false;
            }

            // Handle [S] key
            if (mainEvent.key.code == CFG::keyIDDown) {
                pMap->getPlayer2()->setSquat(false);
                keyDown = false;
            }

            // Handle [A] key
            if (mainEvent.key.code == CFG::keyIDLeft) {
                if (!firstDir2) firstDir2 = true;
                keyLeftPressed = false;
            }

            // Handle [Space] key
            if (mainEvent.key.code == CFG::keyIDUp) {
                CFG::keyUp = false;
            }

            // Handle [LShift] key
            if (mainEvent.key.code == CFG::keyIDShiftRight) {
                if (keyShiftRight) {
                    pMap->getPlayer2()->resetRun();
                    keyShiftRight = false;
                }
            }
        }
    }

    if (mainEvent.type == sf::Event::KeyPressed) {
        if (mainEvent.key.code == CFG::keyIDD) {
            keyDPressed = true;
            if (!keyAPressed) firstDir1 = true;
        }

        if (mainEvent.key.code == CFG::keyIDS) {
            if (!keyS) {
                keyS = true;
                if (!pMap->getUnderWater() && !pMap->getPlayer()->getInLevelAnimation()) 
                    pMap->getPlayer()->setSquat(true);
            }
        }

        if (mainEvent.key.code == CFG::keyIDA) {
            keyAPressed = true;
            if (!keyDPressed) firstDir1 = false;
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
        
        if (mainEvent.key.code == sf::Keyboard::Escape) {
            if (!keyMenuPressed && CFG::getMenuManager()->getViewID() == CFG::getMenuManager()->eGame) {
                CFG::getMenuManager()->resetActiveOptionID(CFG::getMenuManager()->ePasue);
                CFG::getMenuManager()->setViewID(CFG::getMenuManager()->ePasue);
                CFG::getMusic()->PlayChunk(CFG::getMusic()->cPAUSE);
                CFG::getMusic()->PauseMusic();
                keyMenuPressed = true;
            }
        }

        if (mainEvent.key.code == CFG::keyIDSkill) {
            if (!CFG::keySkill) {
                pMap->getPlayer()->setUseSkill(true);
                CFG::keySkill = true;
            }
        }
        
		if (pMap->getNumOfPlayers() == 2) {
			if (mainEvent.key.code == CFG::keyIDRight) {
				keyRightPressed = true;
				if (!keyLeftPressed) firstDir2 = true;
			}
			if (mainEvent.key.code == CFG::keyIDDown) {
				if (!keyDown) {
					keyDown = true;
					if (!pMap->getUnderWater() && !pMap->getPlayer2()->getInLevelAnimation())
						pMap->getPlayer2()->setSquat(true);
				}
			}
			if (mainEvent.key.code == CFG::keyIDLeft) {
				keyLeftPressed = true;
				if (!keyRightPressed) firstDir2 = false;
			}
			if (mainEvent.key.code == CFG::keyIDUp) {
				if (!CFG::keyUp) {
					pMap->getPlayer2()->jump();
					CFG::keyUp = true;
				}
			}
			if (mainEvent.key.code == CFG::keyIDShiftRight && !keyShiftRight) {
				pMap->getPlayer2()->startRun();
				keyShiftRight = true;
			}
		}
    }   

	inputPlayerPressed(keyAPressed, keyDPressed, firstDir1, pMap->getPlayer());
	inputPlayerPressed(keyLeftPressed, keyRightPressed, firstDir2, pMap->getPlayer2());

   // if (keyAPressed) {
   //     if (!pMap->getPlayer()->getMove() && firstDir1 == false
   //         && !pMap->getPlayer()->getChangeMoveDirection()
   //         && !pMap->getPlayer()->getSquat()) {
			//
   //         pMap->getPlayer()->startMove();
			//pMap->getPlayer()->setMoveDirection(false);
   //     }
   //     else if (!keyDPressed && pMap->getPlayer()->getMoveSpeed() > 0
   //         && firstDir1 != pMap->getPlayer()->getMoveDirection()) {
   //         pMap->getPlayer()->setChangeMoveDirection();
   //     }
   // 
   // }

   // if (keyDPressed) {
   //     if (!pMap->getPlayer()->getMove() && firstDir1 == true
   //         && !pMap->getPlayer()->getChangeMoveDirection() 
   //         && !pMap->getPlayer()->getSquat()) {
   //         
   //         pMap->getPlayer()->startMove();
   //         pMap->getPlayer()->setMoveDirection(true);
   //     }
   //     else if (!keyAPressed && pMap->getPlayer()->getMoveSpeed() > 0 
   //         && firstDir1 != pMap->getPlayer()->getMoveDirection()) {
   //         
   //         pMap->getPlayer()->setChangeMoveDirection();
   //     }
   // }

   // // For Inertia Movement
   // if (pMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
   //     pMap->getPlayer()->resetMove();
   // }
}

void Core::inputPlayerPressed(bool pressA, bool pressD, bool firstD, Player* pPlayer)
{
	if (pressA) {
		if (!pPlayer->getMove() && firstD == false
			&& !pPlayer->getChangeMoveDirection()
			&& !pPlayer->getSquat()) {
			pPlayer->startMove();
			pPlayer->setMoveDirection(false);
		}
		else if (!pressD && pPlayer->getMoveSpeed() > 0
			&& firstD != pPlayer->getMoveDirection()) {
			pPlayer->setChangeMoveDirection();
		}
	}

	if (pressD) {
		if (!pPlayer->getMove() && firstD == true
			&& !pPlayer->getChangeMoveDirection()
			&& !pPlayer->getSquat()) {
			pPlayer->startMove();
			pPlayer->setMoveDirection(true);
		}
		else if (!pressA && pPlayer->getMoveSpeed() > 0
			&& firstD != pPlayer->getMoveDirection()) {
			pPlayer->setChangeMoveDirection();
		}
	}

	if (pPlayer->getMove() && !pressA && !pressD) {
		pPlayer->resetMove();
	}
}

//void Core::inputMultiplayer()
//{
//}

void Core::inputMenu()
{
    if (mainEvent.type == sf::Event::KeyPressed) {
        CFG::getMenuManager()->setKey(mainEvent.key.code);
        switch (mainEvent.key.code) {
        case sf::Keyboard::S: case sf::Keyboard::Down:
            if (!keyMenuPressed) {
                CFG::getMenuManager()->keyPressed(2);
                keyMenuPressed = true;
            }
            break;
        case sf::Keyboard::W: case sf::Keyboard::Up:
            if (!keyMenuPressed) {
                CFG::getMenuManager()->keyPressed(0);
                keyMenuPressed = true;
            }
            break;
        case sf::Keyboard::Enter:
            CFG::keyEnter = true;
            if (!keyMenuPressed) {
                CFG::getMenuManager()->enter();
                keyMenuPressed = true;
            }
            break;
        /*case sf::Keyboard::Escape:
            if (!keyMenuPressed) {
                CFG::getMenuManager()->escape();
                keyMenuPressed = true;
            }
            break;*/
        case sf::Keyboard::Right: case sf::Keyboard::D:
			CFG::keyRight = true;
            if (!keyMenuPressed) {
                //CFG::getMenuManager()->keyPressed(3);
				CFG::getMenuManager()->keyPressed(1);
                keyMenuPressed = true;
            }
            break;
        case sf::Keyboard::Left: case sf::Keyboard::A:
			CFG::keyLeft = true;
            if (!keyMenuPressed) {
                //CFG::getMenuManager()->keyPressed(1);
				CFG::getMenuManager()->keyPressed(3);
                keyMenuPressed = true;
            }
            break;
        }
    }
    if (mainEvent.type == sf::Event::KeyReleased) {
		if (mainEvent.key.code == sf::Keyboard::Enter) CFG::keyEnter = false;
		if (mainEvent.key.code == sf::Keyboard::Right || mainEvent.key.code == sf::Keyboard::D) 
            CFG::keyRight = false;
		if (mainEvent.key.code == sf::Keyboard::Left || mainEvent.key.code == sf::Keyboard::A)
			CFG::keyLeft = false;

        switch (mainEvent.key.code) {
        case sf::Keyboard::S: case sf::Keyboard::Down:
        case sf::Keyboard::W: case sf::Keyboard::Up:
		case sf::Keyboard::Escape: case sf::Keyboard::Enter:
        case sf::Keyboard::A: case sf::Keyboard::Right:
        case sf::Keyboard::Left: case sf::Keyboard::D:
            keyMenuPressed = false;
            break;
        default:
            break;
        }
    }
}

void Core::resetMove()
{
    this->keyAPressed = this->keyDPressed = false;
}

void Core::resetKeys() {
	CFG::keyEnter = CFG::keySpace = CFG::keyLeft = CFG::keyRight = false;
        movePressed = keyMenuPressed = keyS = keyW = keyA = keyD =  keyShift 
        = keyAPressed = keyDPressed = false;
}


