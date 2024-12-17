#include "Music.h"
#include "Core.h"  // Keep Core integration
#include <iostream>

/* ******************************************** */

Music::Music() {
    // Load background music
    for (auto& name : { "overworld", "overworld-fast", "underground", "underground-fast",
                       "underwater", "underwater-fast", "castle", "castle-fast",
                       "lowtime", "starmusic", "starmusic-fast", "scorering" }) {
        vMusic.push_back(loadMusic(name));
    }

    // Load sound effects
    for (auto& name : { "coin", "blockbreak", "blockhit", "boom", "bowserfall", "bridgebreak",
                       "bulletbill", "death", "fire", "fireball", "gameover", "intermission",
                       "jump", "jumpbig", "levelend", "lowtime", "mushroomappear", "mushroomeat",
                       "oneup", "pause", "pipe", "rainboom", "shot", "shrink", "stomp", "swim",
                       "vine", "castleend", "princessmusic" }) {
        vChunkBuffers.push_back(loadChunk(name));
        vSounds.emplace_back();
        vSounds.back().setBuffer(*vChunkBuffers.back());
    }

    setVolume(100);
    this->currentMusic = mNOTHING;
}


Music::~Music() {
    for (auto& music : vMusic) delete music;
    for (auto& buffer : vChunkBuffers) delete buffer;
    vMusic.clear();
    vChunkBuffers.clear();
    vSounds.clear();
}

/* ******************************************** */

void Music::changeMusic(bool musicByLevel, bool forceChange) {
    eMusic eNew = currentMusic;

    if (musicByLevel) {
        if (Core::getMap()->getInEvent() && Core::getMap()->getEvent()->inEvent) {
            eNew = mNOTHING;
            PlayChunk(cINTERMISSION);
        }
        else {
            switch (Core::getMap()->getLevelType()) {
            case 0: case 4:
                eNew = (Core::getMap()->getMapTime() > 90) ? mOVERWORLD : mOVERWORLDFAST;
                break;
            case 1:
                eNew = (Core::getMap()->getMapTime() > 90) ? mUNDERWORLD : mUNDERWORLDFAST;
                break;
            case 2:
                eNew = (Core::getMap()->getMapTime() > 90) ? mUNDERWATER : mUNDERWATERFAST;
                break;
            case 3:
                eNew = (Core::getMap()->getMapTime() > 90) ? mCASTLE : mCASTLEFAST;
                break;
            case 100:
                eNew = mNOTHING;
                PlayChunk(cINTERMISSION);
                Core::getMap()->setLevelType(0);
                break;
            default:
                eNew = mNOTHING;
                break;
            }
        }
    }

    if (currentMusic != eNew || forceChange) {
        StopMusic();
        currentMusic = eNew;
        PlayMusic();
    }
}

/* ******************************************** */

void Music::PlayMusic() {
    if (currentMusic != mNOTHING && vMusic[currentMusic - 1]) {
        vMusic[currentMusic - 1]->play();
        musicStopped = false;
    }
    else {
        StopMusic();
    }
}

void Music::PlayMusic(eMusic musicID) {
    if (musicID != mNOTHING && vMusic[musicID - 1]) {
        vMusic[musicID - 1]->play();
        musicStopped = false;
        currentMusic = musicID;
    }
    else {
        StopMusic();
        currentMusic = mNOTHING;
    }
}

void Music::StopMusic() {
    for (auto& music : vMusic) {
        if (music) music->stop();
    }
    musicStopped = true;
}

void Music::PauseMusic() {
    for (auto& music : vMusic) {
        if (music && music->getStatus() == sf::Music::Playing) {
            music->pause();
            musicStopped = true;
        }
        else if (music) {
            music->play();
            musicStopped = false;
        }
    }
}

/* ******************************************** */

void Music::PlayChunk(eChunk chunkID) {
    if (chunkID < vSounds.size()) {
        vSounds[chunkID].setVolume(iVolume);
        vSounds[chunkID].play();
    }
}

sf::Music* Music::loadMusic(std::string fileName) {
    auto* music = new sf::Music();
    if (!music->openFromFile("files/sounds/" + fileName + ".wav")) {
        std::cerr << "Failed to load music: " << fileName << std::endl;
        delete music;
        return nullptr;
    }
    return music;
}

sf::SoundBuffer* Music::loadChunk(std::string fileName) {
    auto* buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile("files/sounds/" + fileName + ".wav")) {
        std::cerr << "Failed to load chunk: " << fileName << std::endl;
        delete buffer;
        return nullptr;
    }
    return buffer;
}

int Music::getVolume() {
    return iVolume;
}

void Music::setVolume(int iVolume) {
    this->iVolume = iVolume;
    for (auto& sound : vSounds) {
        sound.setVolume(iVolume);
    }
}
