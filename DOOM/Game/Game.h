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
#include "../MainMenu/menu.h"


//De momento pongo el enum de la m�quina de estados aqu�, puede que luego haga falta moverlo
enum class Status {eMAINMENU = 0, ePLAYING, eCREDITS, eOPTIONS};	//Completar con tantos estados como se necesite

class Game
{
public:
	Game();
	~Game();

	void ProcessInput(Status status);	//Leer� y procesar� el input del usuario
	void Render();			//Dibujar cosas en pantalla
	void Update();			//C�lculos del juego como posiciones
	void Delay();			//Esperar, la usaremos para bloquear la tasa de FPS a 60

	bool IsOver();			//Para saber si la partida ha terminado
	bool Init();			//Inicializar todo

	Status getStatus();		//Devuelve el estado interno del juego

protected:

	int mainMenu();					//invent supremo
	void handleResize();			//Recalcula el tama�o de la pantalla para preservar el ratio de aspecto
	int id_new_player;				//El id de los jugadores que iremos creando
	sf::RenderWindow* m_pWindow;	//La ventana donde se ejecutar� el juego
	DoomEngine* m_pDoomEngine;		//Referencia a nuestro motor del juego
	Player* m_pPlayer;				//Referencia al jugador
	enum Status gameState;			//Estado del juego (m�quina de estados invent�)
};