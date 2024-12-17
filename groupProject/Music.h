#pragma once

#ifndef MUSIC_H
#define MUSIC_H

#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class Music {
private:
    std::vector<sf::Music*> vMusic;                // For background music
    std::vector<sf::SoundBuffer*> vChunkBuffers;   // Buffers for sound effects
    std::vector<sf::Sound> vSounds;                // Sound instances to play buffers

    int iVolume;

public:
    Music();
    ~Music();

    bool musicStopped;

    enum eMusic {
        mNOTHING,
        mOVERWORLD,
        mOVERWORLDFAST,
        mUNDERWORLD,
        mUNDERWORLDFAST,
        mUNDERWATER,
        mUNDERWATERFAST,
        mCASTLE,
        mCASTLEFAST,
        mLOWTIME,
        mSTAR,
        mSTARFAST,
        mSCORERING,
    };

    eMusic currentMusic;

    enum eChunk {
        cCOIN,
        cBLOCKBREAK,
        cBLOCKHIT,
        cBOOM,
        cBOWSERFALL,
        cBRIDGEBREAK,
        cBULLETBILL,
        cDEATH,
        cFIRE,
        cFIREBALL,
        cGAMEOVER,
        cINTERMISSION,
        cJUMP,
        cJUMPBIG,
        cLEVELEND,
        cLOWTIME,
        cMUSHROOMAPPER,
        cMUSHROOMMEAT,
        cONEUP,
        cPAUSE,
        cPIPE,
        cRAINBOOM,
        cSHOT,
        cSHRINK,
        cSTOMP,
        cSWIM,
        cVINE,
        cCASTLEEND,
        cPRINCESSMUSIC,
    };
    void changeMusic(bool musicByLevel, bool forceChange);

    void PlayMusic();
    void PlayMusic(eMusic musicID);
    void StopMusic();
    void PauseMusic();

    void PlayChunk(eChunk chunkID);

    sf::Music* loadMusic(std::string fileName);
    sf::SoundBuffer* loadChunk(std::string fileName);

    // -- get & set
    int getVolume();
    void setVolume(int iVolume);
};

#endif
