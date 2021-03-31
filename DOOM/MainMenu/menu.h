/*
* Name: menu.h
* Date: 31/03/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase que gestiona la l�gica y los elementos del men� principal.
*/


#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>


class Menu {
public:
	Menu(float width, float height);
	~Menu();

	void draw(sf::RenderWindow* window);
	void MoveUp();
	void MoveDown();
	void drawIntro(sf::RenderWindow* window);
	void creditPage(sf::RenderWindow* window);
	int GetPressedItem() { return selectedItemIndex; }

private:
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

};
