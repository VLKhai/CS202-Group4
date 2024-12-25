#pragma once
#include "Player.h"

class Luigi : public Player {
private:
	std::vector<AniSprite*> sLuigi;
public:
	Luigi(sf::RenderWindow& window, float fXPos, float fYPos);
	~Luigi();
	
	void update() override;
	void draw(sf::RenderWindow& window) override;
	
	void useSkill(Minion* pMinion, float fXmap) override;
	AniSprite* getSprite() override;
	std::string getStringName() override;
};