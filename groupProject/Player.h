#pragma once
#include <vector>
#include "AniSprite.h"

class Player {
private:
    std::vector<AniSprite*> sMario;
    int iSpriteID;
    unsigned int iMoveAnimationTime;

    float fXPos, fYPos;
    int iComboPoints, iFrameID;

    const static int iSmallX = 24, iSmallY = 32;
    const static int iBigX = 32, iBigY = 64;

	// ---<LEVEL UP>---
    int powerLVL;
    bool inLevelAnimation;
    bool inLevelAnimationType; // -- true = UP, false = DOWN
    int inLevelAnimationFrameID;

    // ---<MOVE>---
    bool moveDirection;
    bool bMove;
    bool changeMoveDirection;
    bool newMoveDirection;
    static const int maxMove = 4;
    int currentMaxMove;
    int moveSpeed;
    unsigned int iTimePassed;
    bool bSquat;

    int onPlatformID;

	// ---<JUMP>---
    int jumpState;
    float startJumpSpeed;
    float currentJumpSpeed;
    float jumpDistance;
    float currentJumpDistance;
    float currentFallingSpeed;
	int nextFallFrameID;
    bool springJump;

    
	// ---<METHODS>---
    void movePlayer();
    bool checkCollisionBot(int nX, int nY);
	bool checkCollisionCenter(int nX, int nY);

public:
    Player(sf::RenderWindow& window, float fXPos, float fYPos);
    ~Player();
    
    void draw(sf::RenderWindow& window);
    void update();

    void playerPhysics();

	void updateXPos(int iD); // iD: displacement
	void updateYPos(int iD); // iD: displacement

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

    // --<JUMP>--
    void jump();
    void startJump(int iHeight);
    void resetJump();   
    

    // --<GET & SET>--
    void setMarioSpriteID(int iID);
	int getMarioSpriteID();

	int getMoveSpeed();
    bool getMove();
	bool getMoveDirection();

    bool getSquat();
    void setSquat(bool bSquat);

    int getHitBoxX();
    int getHitBoxY();
};