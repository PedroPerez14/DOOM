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

//TODO poner este código del averno bonito, y el que hay en Game.cpp en la función mainMenu()
//https://c.eev.ee/doom-text-generator/#:~:text=Font&text=Doom%20and%20Strife%20use%20hardcoded,support%20extended%20Latin%20and%20Cyrillic
Menu::Menu(float width, float height, DoomEngine* eng) : m_pDoomEngine(eng)
{
	//Inicializar variables
	m_width = width;
	m_height = height;
	selectedItemIndex = 0;

	srand(time(NULL));

	//Carga de las calaveras
	if (!textureSkull.loadFromFile("../../../../assets/MainMenu/skullMenu2.png")) {
		std::cout << "Error on load skull texture (menu.cpp)" << std::endl;
	}
	skullSprite.setTexture(textureSkull);
	skullSprite.scale((float)SCREENWIDTH * 0.1f / textureSkull.getSize().x, (float)SCREENHEIGHT * 0.2f / textureSkull.getSize().y);
	skullSprite.setPosition(sf::Vector2f((float)SCREENWIDTH * 4.0f / 20.0f, SCREENHEIGHT * 12.25f / 40.0f));
	
	//No hace falta cargar el background aquí porque se carga una imagen aleatoriamente antes de cada carga del menú

	//Carga del panel "DOOM"
	doomTexture.loadFromFile("../../../../assets/MainMenu/DOOM.png");
	doomSprite.setTexture(doomTexture);
	doomSprite.scale((float)SCREENWIDTH * 0.4f / doomTexture.getSize().x, (float)SCREENHEIGHT * 0.4f / doomTexture.getSize().y);
	doomSprite.setPosition((SCREENWIDTH / 2.0f) - doomTexture.getSize().x * doomSprite.getScale().x / 2.0f, 0.0f);

	//Carga del texto main menu
	if (!descripcionMenu.loadFromFile("../../../../assets/MainMenu/LetraMenu.png")) {
		std::cout << "Error on load LetraMenu texture (menu.cpp)" << std::endl;
	}
	descripcionMenuSprite.setTexture(descripcionMenu);
	descripcionMenuSprite.scale((float)SCREENWIDTH * 0.5 / descripcionMenu.getSize().x, (float)SCREENHEIGHT * 0.5 / descripcionMenu.getSize().y);
	descripcionMenuSprite.setPosition((SCREENWIDTH / 2.0f) - doomTexture.getSize().x * doomSprite.getScale().x / 2.0f + 3.0f, SCREENHEIGHT / 2.7f);

	//Carga el panel de creditos
	creditos.loadFromFile("../../../../assets/MainMenu/CreditPage.png");
	creditosSprite.setTexture(creditos);
	creditosSprite.scale((float)SCREENWIDTH * 0.75f / creditos.getSize().x, (float)SCREENHEIGHT * 0.6f / creditos.getSize().y);
	creditosSprite.setPosition((SCREENWIDTH / 2.0f) - creditos.getSize().x * creditosSprite.getScale().x / 2.0f, SCREENHEIGHT * 2.0f / 10.0f);

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

	//Dibuja el panel superior de "DOOM"
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
	skullSprite.setPosition((float)SCREENWIDTH * 4.0f / 20.0f, SCREENHEIGHT * 12.25f / 40.0f + ((SCREENHEIGHT * 5.0f / 40.0f) * selectedItemIndex));
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
	skullSprite.setPosition((float)SCREENWIDTH * 4.0f / 20.0f, SCREENHEIGHT * 12.25f / 40.0f + ((SCREENHEIGHT * 5.0f / 40.0f) * selectedItemIndex));

}

void Menu::creditPage(sf::RenderWindow* window) {

	bool skipIntro = false;
	sf::Event event;
	while (window->pollEvent(event) || !skipIntro) {
		switch (event.type) {
		case sf::Event::KeyReleased:
			skipIntro = true;
			break;

		case sf::Event::Closed:
			m_pDoomEngine->Quit();
			window->close();
			return;
			break;

		}
		window->clear(sf::Color::Black);

		window->draw(backgroundSprite);
		window->draw(doomSprite);
		window->draw(creditosSprite);

		window->display();
	}

}

void Menu::GetRandomMenuBackground()
{
	int backgroundID = rand() % 10;		//El background será aleatorio para cada vez que se cargue el menú
	std::cout << "Background numero " << backgroundID << " elegido" << std::endl;
	backgroundSprite = sf::Sprite();
	background = sf::Texture();
	background.loadFromFile("../../../../assets/MainMenu/background" + std::to_string(backgroundID) + ".png");
	backgroundSprite.setTexture(background);
	backgroundSprite.scale((float)SCREENWIDTH / background.getSize().x, (float)SCREENHEIGHT / background.getSize().y);
}

//Dibuja por pantalla la primera imagen y espera a que se pulse 1 tecla
void Menu::drawIntro(sf::RenderWindow* window) {

	sf::Texture* imageIntro = new sf::Texture;
	imageIntro->loadFromFile("../../../../assets/MainMenu/introImagen2.png");
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
			m_pDoomEngine->Quit();
			window->close();
			return;
			break;

		}
		window->clear(sf::Color::Black);

		window->draw(*spriteIntro);

		window->display();
	}


	std::cout << "Generando Sprites" << std::endl;

	GetRandomMenuBackground();

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