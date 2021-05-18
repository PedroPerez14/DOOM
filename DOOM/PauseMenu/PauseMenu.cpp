/*
* Name: PauseMenu.cpp
* Date: 16/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que gestiona la lógica y los elementos del menú de pausa in-game.
*/

#pragma once

#include "PauseMenu.h"
#include "../doomdef.h"
#include <iostream>

PauseMenu::PauseMenu(sf::RenderWindow* m_pRenderWindow) : m_pRenderWindow(m_pRenderWindow)
{
    //Se cargan en memoria las imágenes que van a hacer falta posteriormente

    if (!PauseTextTexture.loadFromFile("../../../../assets/PauseMenu/Texto.png")) {
        std::cout << "Error on load LetraMenu texture (menu.cpp)" << std::endl;
    }
    PauseTextSprite.setTexture(PauseTextTexture);
    PauseTextSprite.scale((float)SCREENWIDTH * 0.75f / PauseTextTexture.getSize().x, (float)SCREENHEIGHT * 0.2 / PauseTextTexture.getSize().y);
    PauseTextSprite.setPosition((SCREENWIDTH / 2.0f) - PauseTextTexture.getSize().x * PauseTextSprite.getScale().x / 2.0f + 3.0f, SCREENHEIGHT / 2.7f);
}

PauseMenu::~PauseMenu()
{

}

void PauseMenu::RenderPauseMenu()
{
    //A no ser que queramos meter más cosas esto ya estaría

    m_pRenderWindow->draw(PauseTextSprite);
}