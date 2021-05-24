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
Menu::Menu(float width, float height, DoomEngine* eng) : m_pDoomEngine(eng)
{
	//Inicializar variables
	m_width = width;
	m_height = height;
	selectedItemIndex = 0;

	srand(time(NULL));

	//Carga de las calaveras
	if (!textureSkull.loadFromFile("assets/MainMenu/skullMenu2.png")) {
		std::cout << "Error on load skull texture (menu.cpp)" << std::endl;
	}
	skullSprite.setTexture(textureSkull);
	skullSprite.scale((float)SCREENWIDTH * 0.1f / textureSkull.getSize().x, (float)SCREENHEIGHT * 0.2f / textureSkull.getSize().y);
	skullSprite.setPosition(sf::Vector2f((float)SCREENWIDTH * 4.0f / 20.0f, SCREENHEIGHT * 12.25f / 40.0f));
	
	//No hace falta cargar el background aquí porque se carga una imagen aleatoriamente antes de cada carga del menú

	//Carga del panel "DOOM"
	doomTexture.loadFromFile("assets/MainMenu/DOOM.png");
	doomSprite.setTexture(doomTexture);
	doomSprite.scale((float)SCREENWIDTH * 0.4f / doomTexture.getSize().x, (float)SCREENHEIGHT * 0.4f / doomTexture.getSize().y);
	doomSprite.setPosition((SCREENWIDTH / 2.0f) - doomTexture.getSize().x * doomSprite.getScale().x / 2.0f, 0.0f);

	//Carga del texto main menu
	if (!descripcionMenu.loadFromFile("assets/MainMenu/LetraMenu.png")) {
		std::cout << "Error on load LetraMenu texture (menu.cpp)" << std::endl;
	}
	descripcionMenuSprite.setTexture(descripcionMenu);
	descripcionMenuSprite.scale((float)SCREENWIDTH * 0.5 / descripcionMenu.getSize().x, (float)SCREENHEIGHT * 0.5 / descripcionMenu.getSize().y);
	descripcionMenuSprite.setPosition((SCREENWIDTH / 2.0f) - doomTexture.getSize().x * doomSprite.getScale().x / 2.0f + 3.0f, SCREENHEIGHT / 2.7f);

	//Carga el panel de creditos
	creditos.loadFromFile("assets/MainMenu/CreditPage.png");
	creditosSprite.setTexture(creditos);
	creditosSprite.scale((float)SCREENWIDTH * 0.75f / creditos.getSize().x, (float)SCREENHEIGHT * 0.6f / creditos.getSize().y);
	creditosSprite.setPosition((SCREENWIDTH / 2.0f) - creditos.getSize().x * creditosSprite.getScale().x / 2.0f, SCREENHEIGHT * 2.0f / 10.0f);

	//Carga el panel de ConfirmarSalir
	confirmarSalirTexture.loadFromFile("assets/MainMenu/ConfirmarSalir.png");
	confirmarSalirSprite.setTexture(confirmarSalirTexture);
	confirmarSalirSprite.scale((float)SCREENWIDTH * 0.75f / confirmarSalirTexture.getSize().x, (float)SCREENHEIGHT * 0.48f / confirmarSalirTexture.getSize().y);
	confirmarSalirSprite.setPosition((SCREENWIDTH / 2.0f) - confirmarSalirTexture.getSize().x * confirmarSalirSprite.getScale().x / 2.0f, SCREENHEIGHT / 2.5f);


	//Cargar toda la parte de opciones:
	actualSound = 100;
	optionsTexture.loadFromFile("assets/MainMenu/options.png");
	optionsSprite.setTexture(optionsTexture);
	optionsSprite.scale((float)SCREENWIDTH * 0.4f / optionsTexture.getSize().x, (float)SCREENHEIGHT * 0.4f / optionsTexture.getSize().y);
	optionsSprite.setPosition((SCREENWIDTH / 2.0f) - optionsTexture.getSize().x * optionsSprite.getScale().x / 2.0f, SCREENHEIGHT / 2.7f);
	//std::cout << "Carga de menu completada" << std::endl;

	soundLevelTexture.loadFromFile("assets/MainMenu/SoundBar.png");
	for (int i = 0; i < 4; i++) {
		soundLevelSprite[i].setTexture(soundLevelTexture);
		soundLevelSprite[i].scale((float)SCREENWIDTH * 0.4f / soundLevelTexture.getSize().x * 0.13, (float)SCREENHEIGHT * 0.4f / soundLevelTexture.getSize().y * 0.30);
		soundLevelSprite[i].setPosition((SCREENWIDTH / 2.0f) - (soundLevelTexture.getSize().x * soundLevelSprite[i].getScale().x) / 2.0f + i*8 * (SCREENWIDTH / 320.0f) + SCREENWIDTH / 11.0f, SCREENHEIGHT / 2.35f);
	}

	if (!dificultadTexture.loadFromFile("assets/MainMenu/Dificultad.png")) {
		std::cout << "Error on load LetraMenu texture (menu.cpp)" << std::endl;
	}
	dificultadSprite.setTexture(dificultadTexture);
	dificultadSprite.scale((float)SCREENWIDTH * 0.5 / dificultadTexture.getSize().x, (float)SCREENWIDTH * 0.5 / dificultadTexture.getSize().x + 0.05);
	dificultadSprite.setPosition((SCREENWIDTH / 2.0f) - doomTexture.getSize().x * doomSprite.getScale().x / 2.0f + 3.0f, SCREENHEIGHT / 2.7f + 3);

	if (!dificultadesTexture.loadFromFile("assets/MainMenu/Dificultades.png")) {
		std::cout << "Error on load LetraMenu texture (menu.cpp)" << std::endl;
	}
	dificultadesSprite.setTexture(dificultadesTexture);
	dificultadesSprite.scale(dificultadSprite.getScale().x * 0.9, dificultadSprite.getScale().y * 0.9);
	dificultadesSprite.setPosition(dificultadSprite.getPosition().x - 40 , dificultadSprite.getPosition().y + dificultadTexture.getSize().y *dificultadSprite.getScale().y + 2);
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
		case sf::Event::KeyPressed:
			if (!skipIntro && event.key.code == sf::Keyboard::Enter)
			{
				skipIntro = true;
			}
			break;

		case sf::Event::Closed:
			m_pDoomEngine->Quit();
			window->close();
			return;
			break;

		default:
			break;

		}
		window->clear(sf::Color::Black);

		window->draw(backgroundSprite);
		window->draw(doomSprite);
		window->draw(creditosSprite);

		window->display();
	}

}

bool Menu::confirmarSalir(sf::RenderWindow* window) {
	sf::Event event;
	bool skipIntro = false;
	//std::cout << "Entra en confirmacion" << std::endl;
	while (window->pollEvent(event) || !skipIntro) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Enter)
			{
				skipIntro = true;
				return true;
			}
			else {
				return false;
			}
			break;

		case sf::Event::Closed:
			m_pDoomEngine->Quit();
			window->close();
			return true;
			break;

		default:
			break;

		}
		window->clear(sf::Color::Black);

		window->draw(backgroundSprite);
		window->draw(doomSprite);
		window->draw(confirmarSalirSprite);

		window->display();
	}
	return false;
}

double Menu::options(sf::RenderWindow* window, sf::Music* introMusic, sf::Sound* shot) {
	sf::Event event;
	bool salir = false;
	while (window->pollEvent(event) || !salir) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
				case sf::Keyboard::Left:	//Disminuir actualSound
					if (actualSound > 10) {
						actualSound -= 25;
						introMusic->setVolume(actualSound);
						shot->setVolume(actualSound);
						shot->play();
					}
					break;
				case sf::Keyboard::Right:	//Aumentar actualSound
					if (actualSound < 90) {
						actualSound += 25;
						introMusic->setVolume(actualSound);
						shot->setVolume(actualSound);
						shot->play();
					}
					break;
				case sf::Keyboard::Escape:	//Salir
					shot->play();
					return actualSound;
					break;
				default:
					break;
			}
			break;

		case sf::Event::Closed:
			m_pDoomEngine->Quit();
			window->close();
			return 1.0;
			break;
		}
		window->clear(sf::Color::Black);

		window->draw(backgroundSprite);
		window->draw(doomSprite);
		window->draw(optionsSprite);
		for (int i = 0; i < 4; i++) {
			if (i*(int)25 < actualSound) {
				window->draw(soundLevelSprite[i]);
			}
		}

		window->display();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << "Error (?)" << std::endl;
	return 0.1;
}

void Menu::GetRandomMenuBackground()
{
	int backgroundID = rand() % 10;		//El background será aleatorio para cada vez que se cargue el menú
	backgroundSprite = sf::Sprite();
	background = sf::Texture();
	background.loadFromFile("assets/MainMenu/background" + std::to_string(backgroundID) + ".png");
	backgroundSprite.setTexture(background);
	backgroundSprite.scale((float)SCREENWIDTH / background.getSize().x, (float)SCREENHEIGHT / background.getSize().y);
}

//Dibuja por pantalla la primera imagen y espera a que se pulse 1 tecla
void Menu::drawIntro(sf::RenderWindow* window) {

	sf::Texture* imageIntro = new sf::Texture;
	imageIntro->loadFromFile("assets/MainMenu/introImagen2.png");
	sf::Sprite* spriteIntro = new sf::Sprite(*imageIntro);
	spriteIntro->scale((float)SCREENWIDTH / imageIntro->getSize().x, (float)SCREENHEIGHT / imageIntro->getSize().y);

	window->clear(sf::Color::Black);

	window->draw(*spriteIntro);

	window->display();

	bool skipIntro = false;
	bool poll = false;
	sf::Event event;
	poll = window->pollEvent(event);
	while (poll || !skipIntro) {
		switch (event.type) {
			case sf::Event::KeyPressed:
				if (!skipIntro)
				{
					skipIntro = true;
				}
				break;

			case sf::Event::Closed:
				if (poll)
				{
					m_pDoomEngine->Quit();
					window->close();
					return;
				}
				break;

			defalut:
				break;

		}
		window->clear(sf::Color::Black);

		window->draw(*spriteIntro);

		window->display();

		poll = window->pollEvent(event);
	}


	GetRandomMenuBackground();

	sf::Sprite cortesIntro[300];
	for (int i = 0; i < 300; i++) {
		cortesIntro[i].setTexture(*imageIntro);
		cortesIntro[i].setTextureRect(sf::IntRect(1 * i, 0, 1, 400));
		cortesIntro[i].setPosition(((float)SCREENWIDTH * (float)i / (float)300), 0);
		cortesIntro[i].scale((float)SCREENWIDTH / imageIntro->getSize().x, (float)SCREENHEIGHT / imageIntro->getSize().y);
	}

	srand(time(NULL));

	int randomVal = (rand() % 100);
	cortesIntro[0].move(0, randomVal);
	for (int i = 1; i < 300; i++) {
		randomVal = ((randomVal + (rand() % 50 - 25)) % 100);
		cortesIntro[i].move(0, randomVal);
	}


	for (int i = 0; i < 100; i++) {

		for (int i = 0; i < 300; i++) {
			cortesIntro[i].move(0, 3 + (rand() % 3));		//Edita esto para cambiar lo que baja por render
		}

		window->clear(sf::Color::Black);
		draw(window);
		for (int i = 0; i < 300; i++) {
			window->draw(cortesIntro[i]);
		}
		window->display();

		std::this_thread::sleep_for(std::chrono::milliseconds(8));		//Edita esto para cambiar la velocidad de render
	}

}

int dificultadToGameDiff(int dificultad) {
	if (dificultad == 0) { return 0x0001; }
	if (dificultad == 1) { return 0x0002; }
	if (dificultad == 2) { return 0x0004; }
	if (dificultad == 3) { return 0x0005; }
	return -1;
}

int Menu::selectDificultad (sf::RenderWindow* window, sf::Sound* shot){
	sf::Event event;
	bool salir = false;
	dificultad = 0;

	sf::Sprite skullSpriteDificultad (textureSkull);
	skullSpriteDificultad.scale(skullSprite.getScale().x * 0.6, skullSprite.getScale().y * 0.6);
	skullSpriteDificultad.setPosition(dificultadesSprite.getPosition().x - 20, dificultadesSprite.getPosition().y - 5);

	while (window->pollEvent(event) || !salir) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::Up:	//Mover calaveras, sumar +1 al int
				shot->play();
				dificultad = dificultad-1;
				if (dificultad == -1) { dificultad = 3; }
				skullSpriteDificultad.setPosition(dificultadesSprite.getPosition().x - 20, dificultadesSprite.getPosition().y - 5 + (42 * dificultad * dificultadesSprite.getScale().y));
				break;

			case sf::Keyboard::Down:	//Mover calaveras, sumar -1 al int
				shot->play();
				dificultad = dificultad + 1;
				if (dificultad == 4) { dificultad = 0; }
				skullSpriteDificultad.setPosition(dificultadesSprite.getPosition().x - 20, dificultadesSprite.getPosition().y - 5 + (42 * dificultad * dificultadesSprite.getScale().y));
				break;

			case sf::Keyboard::Escape:	//Salir
				shot->play();
				return -1;
				break;
			case sf::Keyboard::Enter:	//Salir
				shot->play();
				return dificultad;
				break;
			default:
				break;
			}
			break;

		case sf::Event::Closed:
			m_pDoomEngine->Quit();
			window->close();
			return -1;
			break;
		}
		window->clear(sf::Color::Black);

		window->draw(backgroundSprite);
		window->draw(doomSprite);
		window->draw(dificultadSprite);
		window->draw(dificultadesSprite);
		window->draw(skullSpriteDificultad);

		window->display();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << "Error (?)" << std::endl;
	return -1;
}