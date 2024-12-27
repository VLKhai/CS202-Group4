#include "ImgResource.h"

// Initialize the singleton instance
ImgResource* ImgResource::instance = nullptr;

// Private constructor
ImgResource::ImgResource() : vBlock(nullptr), vMinion(nullptr) {}

// Singleton accessor
ImgResource* ImgResource::getInstance() {
    if (instance == nullptr) {
        instance = new ImgResource();
    }
    return instance;
}

// Set the reference to block resources
void ImgResource::setBlockResources(std::vector<Block*>* blockResources) {
    vBlock = blockResources;
}

// Set the reference to minion resources
void ImgResource::setMinionResources(std::vector<Block*>* minionResources) {
    vMinion = minionResources;
}

// Update a specific block by ID
void ImgResource::updateBlockByID(int blockID) {
    if (vBlock != nullptr && blockID >= 0 && blockID < static_cast<int>(vBlock->size())) {
        Block* block = (*vBlock)[blockID];
        if (block != nullptr) {
            block->getAniSprite()->update();
        }
    }
}

// Update a specific minion by ID
void ImgResource::updateMinionByID(int minionID) {
    if (vMinion != nullptr && minionID >= 0 && minionID < static_cast<int>(vMinion->size())) {
        Block* minion = (*vMinion)[minionID];
        if (minion != nullptr) {
            minion->getAniSprite()->update();
        }
    }
}

// Retrieve AniSprite for a specific block ID
AniSprite* ImgResource::getAniSprite(int blockID) {
    if (vBlock != nullptr && blockID >= 0 && blockID < static_cast<int>(vBlock->size())) {
        return (*vBlock)[blockID]->getAniSprite();
    }
    return nullptr;
}
