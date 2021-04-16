/*
* Name: PauseMenu.h
* Date: 16/04/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase que gestiona la l�gica y los elementos del men� de pausa in-game.
*/

#pragma once
#include "SFML/Graphics.hpp"

class PauseMenu
{
public:
	PauseMenu(sf::RenderWindow* m_pWindow);
	~PauseMenu();

	void RenderPauseMenu();

protected:

    sf::Texture PauseTextTexture;
    sf::Sprite PauseTextSprite;
    sf::RenderWindow* m_pRenderWindow;
};