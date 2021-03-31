/*
* Name: menu.cpp
* Date: 31/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que gestiona la lógica y los elementos del menú principal.
*/

#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector> 
#include "menu.h"
#include <chrono>
#include <thread>

#include "../doomdef.h"

//https://c.eev.ee/doom-text-generator/#:~:text=Font&text=Doom%20and%20Strife%20use%20hardcoded,support%20extended%20Latin%20and%20Cyrillic
Menu::Menu(float width, float height)
{
	//Inicializar variables
	m_width = width;
	m_height = height;
	float space = (1.5 * height / 3);
	selectedItemIndex = 0;

	//Carga de las calaveras
	if (!textureSkull.loadFromFile("../../../../assets/MainMenu/skullMenu2.png")) {
		std::cout << "Error on load skull texture (menu.cpp)" << std::endl;
	}
	skullSprite.setTexture(textureSkull);
	//skullSprite.setPosition(sf::Vector2f(width / 9, height - (space + (space / 4) * 0) - 9));
	skullSprite.setPosition(sf::Vector2f(65, 210));
	skullSprite.scale(0.6, 0.6);

	skullSpriteRight.setTexture(textureSkull);
	skullSpriteRight.setPosition(sf::Vector2f(530, 210));
	skullSpriteRight.scale(0.6, 0.6);

	//Carga del texto main menu
	if (!descripcionMenu.loadFromFile("../../../../assets/MainMenu/LetraMenu.png")) {
		std::cout << "Error on load LetraMenu texture (menu.cpp)" << std::endl;
	}
	descripcionMenuSprite.setTexture(descripcionMenu);
	descripcionMenuSprite.setPosition(sf::Vector2f(width / 5, height - (space + (space / 4) * 0)));

	//Carga del background despues de la intro
	background.loadFromFile("../../../../assets/MainMenu/background.png");
	backgroundSprite.setTexture(background);
	backgroundSprite.scale(0.6, 0.8);

	//Carga del panel "DOOM"
	doomTexture.loadFromFile("../../../../assets/MainMenu/DOOM.png");
	doomSprite.setTexture(doomTexture);
	doomSprite.scale(0.8, 0.8);
	doomSprite.setPosition(95, 25);

	//Carga el panel de creditos
	creditos.loadFromFile("../../../../assets/MainMenu/CreditPage.png");
	creditosSprite.setTexture(creditos);
	creditosSprite.setPosition(sf::Vector2f(width / 9, height - (space + (space / 4) * 0) + 30));

	std::cout << "Carga de menu completada" << std::endl;



}


Menu::~Menu()
{
}

//Dibuja el estado actual del Main Menu
void Menu::draw(sf::RenderWindow* window) {

	//Dibuja el background
	window->draw(backgroundSprite);

	//Dibuja ambas calaveras en sus posiciones
	window->draw(skullSprite);
	window->draw(skullSpriteRight);

	//Dibuja el pane superior de "DOOM"
	window->draw(doomSprite);

	//Dibuja el menú de opciones encima de todo
	window->draw(descripcionMenuSprite);
}

//Actualiza el estado cuando se pulsa la flecha up
void Menu::MoveUp()
{
	if (selectedItemIndex - 1 > -1)
	{
		selectedItemIndex--;
	}
	else {
		selectedItemIndex = 3;
	}
	//The left skull have the same x-range no matter the item selected now
	skullSprite.setPosition(sf::Vector2f(65, 210 + 42.5 * selectedItemIndex));

	//The right skull moves on x-range depends on the item selected
	switch (selectedItemIndex)
	{
	case 0:
		skullSpriteRight.setPosition(sf::Vector2f(530, 210 + 42.5 * selectedItemIndex));
		break;
	case 1:
		skullSpriteRight.setPosition(sf::Vector2f(350, 210 + 42.5 * selectedItemIndex));
		break;
	case 2:
		skullSpriteRight.setPosition(sf::Vector2f(345, 210 + 42.5 * selectedItemIndex));
		break;
	case 3:
		skullSpriteRight.setPosition(sf::Vector2f(250, 210 + 42.5 * selectedItemIndex));
		break;
	default:
		break;
	}


}

//Actualiza el estado cuando se pulsa la flecha down
void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < 4)
	{
		selectedItemIndex++;
	}
	else {
		selectedItemIndex = 0;
	}
	//The left skull have the same x-range no matter the item selected now
	skullSprite.setPosition(sf::Vector2f(65, 210 + 42.5 * selectedItemIndex));

	//The right skull moves on x-range depends on the item selected
	switch (selectedItemIndex)
	{
	case 0:
		skullSpriteRight.setPosition(sf::Vector2f(530, 210 + 42.5 * selectedItemIndex));
		break;
	case 1:
		skullSpriteRight.setPosition(sf::Vector2f(350, 210 + 42.5 * selectedItemIndex));
		break;
	case 2:
		skullSpriteRight.setPosition(sf::Vector2f(345, 210 + 42.5 * selectedItemIndex));
		break;
	case 3:
		skullSpriteRight.setPosition(sf::Vector2f(250, 210 + 42.5 * selectedItemIndex));
		break;
	default:
		break;
	}

}

void Menu::creditPage(sf::RenderWindow* window) {

	window->clear();

	window->draw(backgroundSprite);
	window->draw(doomSprite);
	window->draw(creditosSprite);

	window->display();
	bool skipIntro = false;
	sf::Event event;
	while (window->pollEvent(event) || !skipIntro) {
		switch (event.type) {
		case sf::Event::KeyReleased:
			skipIntro = true;
			break;

		case sf::Event::Closed:
			window->close();
			break;

		}
	}

}

//Dibuja por pantalla la primera imagen y espera a que se pulse 1 tecla
void Menu::drawIntro(sf::RenderWindow* window) {

	sf::Texture* imageIntro = new sf::Texture;
	imageIntro->loadFromFile("../../../../assets/MainMenu/introImagen.png");
	sf::Sprite* spriteIntro = new sf::Sprite(*imageIntro);
	spriteIntro->scale((float)SCREENWIDTH / imageIntro->getSize().x, (float)SCREENHEIGHT / imageIntro->getSize().y);

	window->clear(sf::Color::Black);

	window->draw(*spriteIntro);

	window->display();

	bool skipIntro = false;
	sf::Event event;
	while (window->pollEvent(event) || !skipIntro) {
		switch (event.type) {
		case sf::Event::KeyReleased:
			skipIntro = true;
			break;

		case sf::Event::Closed:
			window->close();
			break;

		}
		window->clear(sf::Color::Black);

		window->draw(*spriteIntro);

		window->display();
	}


	std::cout << "Generando Sprites" << std::endl;
	sf::Sprite asdasSprite[300];
	for (int i = 0; i < 300; i++) {
		asdasSprite[i].setTexture(*imageIntro);
		asdasSprite[i].setTextureRect(sf::IntRect(4.75 * i, 0, 14, 890));
		asdasSprite[i].setPosition((float)(window->getView().getSize().x * i / 300), 0);
		asdasSprite[i].scale(0.42, 0.52);
	}

	srand(time(NULL));

	int randomVal = (rand() % 100);
	asdasSprite[0].move(0, randomVal);
	for (int i = 1; i < 300; i++) {
		randomVal = ((randomVal + (rand() % 50 - 25)) % 100);
		asdasSprite[i].move(0, randomVal);
	}


	for (int i = 0; i < 100; i++) {

		for (int i = 0; i < 300; i++) {
			asdasSprite[i].move(0, 5 + (rand() % 4));
		}

		window->clear(sf::Color::Black);
		draw(window);
		for (int i = 0; i < 300; i++) {
			window->draw(asdasSprite[i]);
		}
		window->display();

		std::this_thread::sleep_for(std::chrono::milliseconds(6));
	}

}