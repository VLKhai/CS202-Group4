#pragma once
#include <vector>
#include "AniSprite.h"
#include "Vector2.h"

class Minion;

class Player {
private:
	static int iSpriteID;
	static unsigned int iMoveAnimationTime;

	static int iNumOfLives;

	static bool unKillAble;
	static bool starEffect;

	static int unKillAbleTimeFrameID;
	static int unKillAbleFrameID;

	static unsigned int iScore;
	static unsigned int iCoins;

	static int iComboPoints, iFrameID;

	// ----- LVL UP

	// -- LEVEL CHANGE ANIMATION
	static bool inLevelAnimation;
	static bool inLevelAnimationType; // -- true = UP, false = DOWN
	static int inLevelAnimationFrameID;

	// ----- LVL UP
	// ----- MOVE

	static bool bMove;
	static bool changeMoveDirection;
	static bool newMoveDirection;

	static const int maxMove = 4;
	static int currentMaxMove;
	static int moveSpeed;
	static unsigned int iTimePassed;

	static bool bSquat;

	static int onPlatformID;

	// ----- MOVE
	// ----- JUMP

	static int jumpState;

	static float startJumpSpeed;
	static float currentJumpSpeed;
	static float jumpDistance;
	static float currentJumpDistance;
	//float distPressJ

	static float currentFallingSpeed;

	static bool springJump;

	// ----- JUMP
	// ----- BUBBLE

	static unsigned int nextBubbleTime;
	static int nextFallFrameID;

	const static int iSmallX = 24, iSmallY = 32;
	const static int iBigX = 32, iBigY = 64;

	static int nextFireBallFrameID;

	bool bUseSkill;

	// ----- Method
	void movePlayer();

	bool checkCollisionBot(int nX, int nY);
	bool checkCollisionCenter(int nX, int nY);

	Vector2* getBlockLB(float nX, float nY);
	Vector2* getBlockRB(float nX, float nY);

	Vector2* getBlockLC(float nX, float nY);
	Vector2* getBlockRC(float nX, float nY);

	Vector2* getBlockLT(float nX, float nY);
	Vector2* getBlockRT(float nX, float nY);
protected:
	float fXPos, fYPos;

	bool moveDirection; // true = LEFT, false = RIGHT

	int powerLVL;

	// Block = 32px
	int limSmallJumpBlock; // Mario: 2 blocks, Luigi: 3 blocks
	int limBigJumpBlock; // Mario: 4 blocks, Luigi: 6 blocks	
	
	bool inLevelDownAnimation;
	int inLevelDownAnimationFrameID;

	Player(sf::RenderWindow& window, float fXPos, float fYPos);
	Player() = delete;
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
public:
	virtual ~Player() = default;
    
    virtual void draw(sf::RenderWindow& window) = 0;
	virtual void update();

    void playerPhysics(); 

	void updateXPos(int iN); 
	void updateYPos(int iN); 

	virtual void useSkill(Minion* pMinion) = 0;

	// --<ANIMATION>--
    void moveAnimation();
	void swimingAnimation();
	void powerUPAnimation();

	// --<MOVE>--
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

	// --<HIT BOX>--
    int getHitBoxX();
    int getHitBoxY();

	void createFireBall();
    
    // --<GET & SET>--
    void setSpriteID(int iID);
	int getSpriteID();

	bool getInLevelAnimation();
	void setInLevelAnimation(bool inLevelAnimation);

	int getMoveSpeed();
    bool getMove();
	bool getMoveDirection();
	void setNextFallFrameID(int nextFallFrameID);
	void setCurrentJumpSpeed(float currentJumpSpeed);
	void setMoveSpeed(int moveSpeed);
    
	bool getUnkillAble();

	int getNumOfLives();
	void setNumOfLives(int iNumOfLives);

	bool getStarEffect(); 
	void setStarEffect(bool starEffect);

	int getJumpState();

	int getXPos();
	void setXPos(float fXPos);
	int getYPos();
	void setYPos(float fYPos);

    int getPowerLVL();
    void setPowerLVL(int powerLVL);
    void resetPowerLVL();

	virtual AniSprite* getSprite() = 0;
	virtual std::string getStringName() = 0;

	bool getSquat();
	void setSquat(bool bSquat);

	unsigned int getScore();
	void setScore(unsigned int iScore);

	void addComboPoints();
	int getComboPoints();

	void addCoin();
	unsigned int getCoins();
	void setCoins(unsigned int iCoins);

	void setSpringJump(bool springJump);

	bool getUseSkill();
};