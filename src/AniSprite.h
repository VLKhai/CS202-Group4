#ifndef ANISPRITE_H
#define ANISPRITE_H
#include "IMG.h"

class AniSprite {
public:
    AniSprite();
    AniSprite(sf::RenderWindow& window, std::vector<std::string> sSprite, std::vector<unsigned int> iDelay, bool bRotate);
    ~AniSprite();

    void updateFrame();
    IMG* getFrame();
    IMG* getFrame(int iID);

private:
    std::vector<IMG*> imgFrame;
    std::vector<unsigned int> iDelay;
    bool bRotate;
    int iCurrentFrame;
    int iStartFrame;
    int iEndFrame;
    sf::Clock clock;
};

#endif // SPRITE_H