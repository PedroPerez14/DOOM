/*
* Name: PauseMenu.cpp
* Date: 16/04/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase que gestiona la l�gica y los elementos del men� de pausa in-game.
*/

#pragma once

#include "PauseMenu.h"
#include "../doomdef.h"
#include <iostream>

PauseMenu::PauseMenu(sf::RenderWindow* m_pRenderWindow) : m_pRenderWindow(m_pRenderWindow)
{
    //Se cargan en memoria las im�genes que van a hacer falta posteriormente

    if (!PauseTextTexture.loadFromFile("../../../../assets/PauseMenu/TextoPausa.png")) {
        std::cout << "Error on load LetraMenu texture (Pausemenu.cpp)" << std::endl;
    }
    PauseTextSprite.setTexture(PauseTextTexture);
    PauseTextSprite.scale((float)SCREENWIDTH * 0.75f / PauseTextTexture.getSize().x, (float)SCREENHEIGHT * 0.2 / PauseTextTexture.getSize().y);
    PauseTextSprite.setPosition((SCREENWIDTH / 2.0f) - PauseTextTexture.getSize().x * PauseTextSprite.getScale().x / 2.0f + 3.0f, SCREENHEIGHT / 2.7f);

    if (!DeathTextTexture.loadFromFile("../../../../assets/PauseMenu/TextoMuerte.png")) {
        std::cout << "Error on load texto muerte texture (Pausemenu.cpp)" << std::endl;
    }
    DeathTextSprite.setTexture(DeathTextTexture);
    DeathTextSprite.scale((float)SCREENWIDTH * 0.75f / DeathTextTexture.getSize().x, (float)SCREENHEIGHT * 0.35 / DeathTextTexture.getSize().y);
    DeathTextSprite.setPosition((SCREENWIDTH / 2.0f) - DeathTextTexture.getSize().x * DeathTextSprite.getScale().x / 2.0f + 3.0f, SCREENHEIGHT / 3.2f);
}

PauseMenu::~PauseMenu()
{

}

void PauseMenu::RenderPauseMenu()
{
    //A no ser que queramos meter m�s cosas esto ya estar�a
    m_pRenderWindow->draw(PauseTextSprite);
}

void PauseMenu::RenderDeathMenu() {
    m_pRenderWindow->draw(DeathTextSprite);
}