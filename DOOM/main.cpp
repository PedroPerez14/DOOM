/*
* Name: main.cpp
* Date: 03/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Punto de entrada del juego DOOM.
*/
#pragma once
#include "DOOM.h"
#include <SFML/Window.hpp>
using namespace std;

int main()
{
    Game doom_game;
    doom_game.Init();
    sf::Clock deltaClock;
    sf::Time deltaTime, elapsed;
    while (!doom_game.IsOver())
    {
        deltaTime = deltaClock.restart();
        doom_game.setDeltaTime(deltaTime.asSeconds());
        doom_game.ProcessInput(doom_game.getStatus());  
        doom_game.Update();                             
        doom_game.Render();                            
        elapsed = deltaClock.getElapsedTime();
        doom_game.Delay(elapsed.asSeconds());           
    }

    return 0;
}
