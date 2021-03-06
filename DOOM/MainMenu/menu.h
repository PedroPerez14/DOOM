/*
* Name: menu.h
* Date: 31/03/2021 (DD/MM/YYYY)
* Author: V?ctor Mart?nez Lascorz (738845)
* Author: Pedro Jos? P?rez garc?a (756642)
* Coms: Clase que gestiona la l?gica y los elementos del men? principal.
*/


#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "../Engine/DoomEngine.h"


class Menu {
public:
	Menu(float width, float height, DoomEngine* eng);
	~Menu();

	void draw(sf::RenderWindow* window);
	void MoveUp();
	void MoveDown();
	void drawIntro(sf::RenderWindow* window);
	void creditPage(sf::RenderWindow* window);
	bool confirmarSalir(sf::RenderWindow* window);
	double options(sf::RenderWindow* window, sf::Music* introMusic, sf::Sound* shot);
	int GetPressedItem() { return selectedItemIndex; }

	int selectDificultad(sf::RenderWindow* window, sf::Sound* shot);

private:

	void GetRandomMenuBackground();

	int dificultad;
	int selectedItemIndex;
	sf::Texture background;
	sf::Sprite backgroundSprite;
	sf::Texture doomTexture;
	sf::Sprite doomSprite;
	sf::Texture textureSkull;
	sf::Sprite skullSprite;
	sf::Texture descripcionMenu;
	sf::Sprite descripcionMenuSprite;
	sf::Texture creditos;
	sf::Sprite creditosSprite;
	sf::Texture optionsTexture;
	sf::Sprite optionsSprite;
	sf::Texture confirmarSalirTexture;
	sf::Sprite confirmarSalirSprite;
	sf::Texture soundLevelTexture;
	sf::Sprite soundLevelSprite[4];

	sf::Texture dificultadTexture;
	sf::Sprite dificultadSprite;
	sf::Texture dificultadesTexture;
	sf::Sprite dificultadesSprite;

	float m_width;
	float m_height;
	double actualSound;
	DoomEngine* m_pDoomEngine;
};
