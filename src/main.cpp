#include <SFML/Graphics.hpp>
#include "engine/GameEngine.h"
int main()
{
    auto fullScreen = sf::VideoMode::getFullscreenModes()[1];
    engine::GameEngine eng = engine::GameEngine(255, 680, 468);

    eng.startLoop();


}