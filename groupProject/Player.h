#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Core.h"
#include "AniSprite.h"

class Player {
private:
    std::vector<AniSprite*> sMario;
    int iSpriteID;
    unsigned int iMoveAnimationTime;

    float fXPos, fYPos;

    bool bMove;

    int moveSpeed;
public:
    Player(sf::RenderWindow& window, float fXPos, float fYPos);
    ~Player();
    
    void draw(sf::RenderWindow& window);
    void update();

    // --<MOVE>--
    void moveAnimation();
	void swimingAnimation();

	void startMove();
	void resetMove();
	void stopMove();
	void setMoveDirection(bool moveDirection);
	bool getChangeMoveDirection();
	void setChangeMoveDirection();

	void startRun();
	void resetRun();
    // --</MOVE>--

    // --<JUMP>--
    void jump();
    void startJump(int iHeight);
    void resetJump();   
    // --</JUMP>--
    

    // --<GET & SET>--
    
    bool getMove();

    void getSquat();
    void setSquat(bool bSquat);
};