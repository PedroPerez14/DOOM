/*
* Name: Game.h
* Date: 09/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que gestionará la lógica del juego y el gameloop
*/

#pragma once
#include "../Engine/DoomEngine.h"
#include "SFML/Graphics.hpp"

class Game
{
public:
	Game();
	~Game();

	void ProcessInput();	//Leerá y procesará el input del usuario
	void Render();			//Dibujar cosas en pantalla
	void Update();			//Cálculos del juego como posiciones
	void Delay();			//Esperar, la usaremos para bloquear la tasa de FPS a 60

	bool IsOver();			//Para saber si la partida ha terminado
	bool Init();			//Inicializar todo

protected:

	void handleResize();			//Recalcula el tamaño de la pantalla para preservar el ratio de aspecto
	int id_new_player;				//El id de los jugadores que iremos creando
	sf::RenderWindow* m_pWindow;	//La ventana donde se ejecutará el juego
	DoomEngine* m_pDoomEngine;		//Referencia a nuestro motor del juego
	Player* m_pPlayer;				//Referencia al jugador
};