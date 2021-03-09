/*
* Name: Game.h
* Date: 09/03/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase que gestionar� la l�gica del juego y el gameloop
*/

#pragma once
#include "../Engine/DoomEngine.h"
#include "SFML/Graphics.hpp"

class Game
{
public:
	Game();
	~Game();

	void ProcessInput();	//Leer� y procesar� el input del usuario
	void Render();			//Dibujar cosas en pantalla
	void Update();			//C�lculos del juego como posiciones
	void Delay();			//Esperar, la usaremos para bloquear la tasa de FPS a 60

	bool IsOver();			//Para saber si la partida ha terminado
	bool Init();			//Inicializar todo

protected:
	sf::RenderWindow* m_pWindow;	//La ventana donde se ejecutar� el juego
	DoomEngine* m_pDoomEngine;		//Referencia a nuestro motor del juego
};