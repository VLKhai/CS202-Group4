#include "Minion.h"
#include "Core.h"

class Spring : public Minion {
private:
	bool inAnimation;
	int nextFrameID;

	bool extraJump;

public:
	Spring(int iXPos, int iYPos);
	~Spring(void);

	void Update();
	void Draw(sf::RenderWindow& window, IMG* iIMG);

	void minionPhysics();

	void collisionWithPlayer(bool TOP);

	void setMinionState(int minionState);
};

/* ******************************************** */

Spring::Spring(int iXPos, int iYPos) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos;

	this->inAnimation = false;
	this->nextFrameID = 0;

	this->extraJump = false;
}

Spring::~Spring(void) {}

/* ******************************************** */

void Spring::Update() {
	if (inAnimation) {
		if (nextFrameID > 0) {
			--nextFrameID;
		}
		else {
			inAnimation = false;
		}
	}
}

void Spring::Draw(sf::RenderWindow& window, IMG* iIMG) {
	iIMG->draw(window, (int)fXPos, (int)fYPos, false);
}

void Spring::minionPhysics() {
	if (!inAnimation) {
		Minion::minionPhysics();
	}
}

void Spring::collisionWithPlayer(bool TOP) {
	if (TOP) {
		extraJump = true;
		inAnimation = true;
		nextFrameID = 10; // Example frame duration
	}
}

void Spring::setMinionState(int minionState) {
	this->minionState = minionState;
}
