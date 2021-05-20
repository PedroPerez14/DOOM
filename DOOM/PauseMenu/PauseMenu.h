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
	void RenderDeathMenu();
	void renderSmeltEffect();
	void RenderCarga1(int porcentaje);
	void RenderCarga2(int porcentaje);
	void RenderCarga3(int porcentaje);
	void RenderEnd();

	void renderPorcentajeKills(int porcentaje);


protected:

	sf::Texture PauseTextTexture;
	sf::Sprite PauseTextSprite;
	sf::Texture DeathTextTexture;
	sf::Sprite DeathTextSprite;

	sf::Texture fondoTexture;
	sf::Sprite fondoSprite;
	sf::Texture finalizado;
	sf::Sprite finalizadoSprite;

	sf::Texture bajas;
	sf::Sprite bajasSprite;
	sf::Texture porcentaje;
	sf::Sprite porcentajeSprite;

	sf::Texture Hangar;
	sf::Sprite HangarSprite;
	sf::Texture Nuclear;
	sf::Sprite NuclearSprite;
	sf::Texture Toxic;
	sf::Sprite ToxicSprite;

	sf::Texture cargaCompletaTexture;
	sf::Sprite cargaCompletaSprite;

	sf::Texture interNuclearTexture;
	sf::Sprite interNuclearSprite;
	sf::Texture interToxicTexture;
	sf::Sprite interToxicSprite;

	sf::Texture numerosTexture;
	sf::Sprite numerosSprite[10];

	sf::Texture endTextTexture;
	sf::Sprite endTextSprite;
	sf::Texture endBackgroundTexture;
	sf::Sprite endBackgroundSprite;
	sf::Texture enhorabuena;
	sf::Sprite enhorabuenaSprite;

    sf::RenderWindow* m_pRenderWindow;
};