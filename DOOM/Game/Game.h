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
#include "../MainMenu/menu.h"


//De momento pongo el enum de la máquina de estados aquí, puede que luego haga falta moverlo
enum class Status {eMAINMENU = 0, ePLAYING, eCREDITS, eOPTIONS};	//Completar con tantos estados como se necesite

class Game
{
public:
	Game();
	~Game();

	void ProcessInput(Status status);	//Leerá y procesará el input del usuario
	void Render();			//Dibujar cosas en pantalla
	void Update();			//Cálculos del juego como posiciones
	void Delay();			//Esperar, la usaremos para bloquear la tasa de FPS a 60

	bool IsOver();			//Para saber si la partida ha terminado
	bool Init();			//Inicializar todo

	Status getStatus();		//Devuelve el estado interno del juego

protected:

	int mainMenu();					//invent supremo
	void handleResize();			//Recalcula el tamaño de la pantalla para preservar el ratio de aspecto
	int id_new_player;				//El id de los jugadores que iremos creando
	sf::RenderWindow* m_pWindow;	//La ventana donde se ejecutará el juego
	DoomEngine* m_pDoomEngine;		//Referencia a nuestro motor del juego
	Player* m_pPlayer;				//Referencia al jugador
	enum Status gameState;			//Estado del juego (máquina de estados invent®)
};