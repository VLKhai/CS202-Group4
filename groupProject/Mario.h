#pragma once
#include "Player.h"

class Mario : public Player {
private:
	std::vector<AniSprite*> sMario;
public:
	Mario(sf::RenderWindow& window, float fXPos, float fYPos);
	~Mario();
	void draw(sf::RenderWindow& window) override;
	AniSprite* getMarioSprite() override;
};