/*
* Name: PauseMenu.h
* Date: 16/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que gestiona la lógica y los elementos del menú de pausa in-game.
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