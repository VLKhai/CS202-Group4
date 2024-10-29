#include <SFML/Graphics.hpp>
#include "Core.h"
#include "IMG.h"
#include "AniSprite.h"
#include "Block.h"

Core::Core()
{//1000 X 560 & 800 x 448
    this->mainWindow.create(sf::VideoMode(1000, 560), "Mario Game");
}

void Core::mainLoop()
{
    std::vector<std::string> sFilename = {"goombas_0", "goombas_1"};
    std::vector<unsigned int> iDelay = {200, 200};
    AniSprite goomba(this->mainWindow, sFilename, iDelay, false);
    Block block(0, &goomba, true, true, true, true);

    std::vector<std::string> sFilename1 = {"koopa_0", "koopa_1"};
    std::vector<unsigned int> iDelay1 = {200, 200};
    AniSprite koopa(this->mainWindow, sFilename1, iDelay1, false);
    Block block1(0, &koopa, true, true, true, true);

    std::vector<std::string> sFilename2 = {"koopa_2", "koopa_3"};
    std::vector<unsigned int> iDelay2 = {200, 200};
    AniSprite koopa2(this->mainWindow, sFilename2, iDelay2, false);
    Block block2(0, &koopa2, true, true, true, true);

    // Start the game loop
    while (mainWindow.isOpen()) {
        while (mainWindow.pollEvent(mainEvent)) {
            if (mainEvent.type == sf::Event::Closed) {
                mainWindow.close();
            }
            // Handle other events like keyboard input here
        }
        mainWindow.clear(sf::Color::White);
        block.draw(mainWindow, 100, 100);
        goomba.updateFrame();

        block1.draw(mainWindow, 200, 200);
        koopa.updateFrame();

        block2.draw(mainWindow, 300, 300);
        koopa2.updateFrame();

        mainWindow.display();
    }
}