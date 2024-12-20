#pragma once
#include "Player.h"

class Luigi : public Player {
private:
	std::vector<AniSprite*> sLuigi;
public:
	Luigi(sf::RenderWindow& window, float fXPos, float fYPos);
	~Luigi();
	void draw(sf::RenderWindow& window) override;
	AniSprite* getSprite() override;
};