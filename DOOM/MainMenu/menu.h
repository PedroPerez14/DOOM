/*
* Name: menu.h
* Date: 31/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que gestiona la lógica y los elementos del menú principal.
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
	int GetPressedItem() { return selectedItemIndex; }

private:

	void GetRandomMenuBackground();

	int selectedItemIndex;
	sf::Texture background;
	sf::Sprite backgroundSprite;
	sf::Texture doomTexture;
	sf::Sprite doomSprite;
	sf::Texture textureSkull;
	sf::Sprite skullSprite;
	sf::Sprite skullSpriteRight;
	sf::Texture descripcionMenu;
	sf::Sprite descripcionMenuSprite;
	sf::Texture creditos;
	sf::Sprite creditosSprite;
	float m_width;
	float m_height;
	DoomEngine* m_pDoomEngine;
};
