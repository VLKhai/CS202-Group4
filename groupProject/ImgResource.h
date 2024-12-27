#ifndef IMGRESOURCE_H
#define IMGRESOURCE_H

#include <vector>
#include "Block.h"
#include "AniSprite.h"

class ImgResource {
private:
    // Singleton instance
    static ImgResource* instance;
    // References to external resources 
    std::vector<Block*>* vBlock;
    std::vector<Block*>* vMinion;
    // Private constructor to enforce Singleton pattern
    ImgResource();

public:
    // Singleton accessor
    static ImgResource* getInstance();
    ImgResource(const ImgResource&) = delete;
    ImgResource& operator=(const ImgResource&) = delete;

    // Set resource references
    void setBlockResources(std::vector<Block*>* blockResources);
    void setMinionResources(std::vector<Block*>* minionResources);

    // Resource management methods
    void updateBlockByID(int blockID);    
    void updateMinionByID(int minionID); 
    AniSprite* getAniSprite(int blockID); 

    ~ImgResource() = default;
};

#endif
