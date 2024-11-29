#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class CFG
{
public:  
    CFG();
    ~CFG();
    static int GameWidth, GameHeight;
	static bool keySpace;
	static int keyIDA, keyIDS, keyIDD, keyIDSpace, keyIDShift;
    static bool canMoveBackward;

    // Methods
    static sf::Music* getMusic();
private:
    static sf::Music* music;
};