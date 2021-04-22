/*
* Name: HUD.h
* Date: 22/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que engloba todo el HUD del jugador
*/

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "SFML/Graphics.hpp"
#include "../Player/Player.h"

class Hud
{

public:
	Hud();
	Hud(sf::RenderWindow* renderwindow, Player* player);
	~Hud();
	void setSpriteToPosition(int number, int x);
	void drawNumer(int number);
	void drawLifePoints();
	void drawArmorValue();
	void drawAmmoValue();
	void drawHud();

protected:

	//Jugador y pantalla de DOOM
	sf::RenderWindow* m_pRenderWindow;
	Player* m_pPlayer;

	//Texturas y spites privados de la clase
	sf::Texture numerosTexture;
	sf::Sprite numerosSprite[10];
	sf::Texture hudTexture;
	sf::Sprite hudSprite;
	int yValue;

};