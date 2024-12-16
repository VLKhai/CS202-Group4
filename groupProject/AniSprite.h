#pragma once

#include "IMG.h"

class AniSprite {
public:
	AniSprite() = default;
    AniSprite(sf::RenderWindow& window, std::vector<std::string> sSprite, std::vector<unsigned int> iDelay, bool bRotate);
    ~AniSprite();
    void update();
    
    IMG* getFrame();
    IMG* getFrame(int iID);
    IMG* getTexture();
    IMG* getTexture(int iID);
private:
    std::vector<IMG*> imgFrame;
    std::vector<unsigned int> iDelay;
    bool bRotate;
    int iCurrentFrame;
    int iStartFrame;
    int iEndFrame;
    unsigned long lTimePassed;
};
