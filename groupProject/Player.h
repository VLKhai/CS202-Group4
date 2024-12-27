#pragma once
#include <vector>
#include "AniSprite.h"
#include "Vector2.h"
#include "Minion.h"
#include "Skill.h"

class Player {
private:
	int hackMove = 1;
	int hackJump = 1;
	
	int iSpriteID;
	unsigned int iMoveAnimationTime;

	int iNumOfLives;

	bool unKillAble;
	bool starEffect;

	int unKillAbleTimeFrameID;
	int unKillAbleFrameID;

	unsigned int iScore;
	unsigned int iCoins;

	int iComboPoints, iFrameID;

	// ----- LVL UP

	// -- LEVEL CHANGE ANIMATION
	bool inLevelAnimation;
	bool inLevelAnimationType; // -- true = UP, false = DOWN
	int inLevelAnimationFrameID;

	// ----- LVL UP
	// ----- MOVE

	bool bMove;
	bool changeMoveDirection;
	bool newMoveDirection;

	const int maxMove = 4;
	int currentMaxMove;
	int moveSpeed;
	unsigned int iTimePassed;

	bool bSquat;

	int onPlatformID;

	// ----- MOVE
	// ----- JUMP

	int jumpState;

	float startJumpSpeed;
	float currentJumpSpeed;
	float jumpDistance;
	float currentJumpDistance;

	float currentFallingSpeed;

	bool springJump;

	// ----- JUMP
	// ----- BUBBLE

	unsigned int nextBubbleTime;
	int nextFallFrameID;

	const static int iSmallX = 24, iSmallY = 32;
	const static int iBigX = 32, iBigY = 64;

	int nextFireBallFrameID;

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
	int iIDPlayer;

	bool moveDirection; // true = LEFT, false = RIGHT
	bool bJumpPressed;

	int powerLVL;

	// Block = 32px
	int limSmallJumpBlock; // Mario: 2 blocks, Luigi: 3 blocks
	int limBigJumpBlock; // Mario: 4 blocks, Luigi: 6 blocks	
	
	bool inLevelDownAnimation;
	int inLevelDownAnimationFrameID;

	int iSkillCooldown = 4000;
	int iTimeUsedSkill = 0;
	bool bUseSkill;
	Skill* pSkill;
public:
	Player(sf::RenderWindow& window, float fXPos, float fYPos);
	virtual ~Player() = default;
    
    virtual void draw(sf::RenderWindow& window) = 0;
	virtual void update();
	void updateXPos(int iN); 
	void updateYPos(int iN); 

	void copyStats(Player* pPlayer);

    void playerPhysics(); 

	virtual void useSkill(Minion* pMinion, float fXmap) = 0;

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

	int getIDPlayer();
	void setIDPlayer(int iIDPlayer);	

	bool getUseSkill();
	virtual void setUseSkill(bool bUseSkill);

	void Save(std::ofstream& outFile) const;

	void Load(std::ifstream& inFile);
};