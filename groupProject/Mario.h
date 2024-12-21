#pragma once
#include "Player.h"
#include "ExplodeSkill.h"

class Mario : public Player {
private:
	std::vector<AniSprite*> sMario;
public:
	Mario(sf::RenderWindow& window, float fXPos, float fYPos);
	~Mario();
	
	void update();

	void draw(sf::RenderWindow& window) override;
	
	void useSkill(Minion* pMinion, float fXmap) override;

	AniSprite* getSprite() override;
	std::string getStringName() override;
};