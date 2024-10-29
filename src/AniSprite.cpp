#include "AniSprite.h"

AniSprite::AniSprite() {}

AniSprite::AniSprite(sf::RenderWindow &window, std::vector<std::string> sFilename, std::vector<unsigned int> iDelay, bool bRotate)
{
    this->iDelay = iDelay;
    this->bRotate = bRotate;

    this->iCurrentFrame = 0;
    this->iStartFrame = 0;
    this->iEndFrame = sFilename.size() - 1;

    for (int i = 0; i <= this->iEndFrame; i++) {
        this->imgFrame.push_back(new IMG(sFilename[i], window));
    }
}

AniSprite::~AniSprite()
{
    for (int i = 0; i <= this->iEndFrame; i++) {
        delete this->imgFrame[i];
    }
}

void AniSprite::updateFrame() 
{
    if (clock.getElapsedTime().asMilliseconds() > iDelay[iCurrentFrame]) {
        clock.restart();
        (iCurrentFrame == iEndFrame) ? iCurrentFrame = 0 : ++iCurrentFrame;
    }
}

IMG *AniSprite::getFrame()
{
    return this->imgFrame[iCurrentFrame];
}

IMG *AniSprite::getFrame(int iID)
{
    return this->imgFrame[iID];
}
