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
#include "GameStates.h"
#include "../PauseMenu/PauseMenu.h"
#include "../DisplayManager/DisplayManager.h"

class Game
{
public:
	Game();
	~Game();

	void ProcessInput(Status status);	//Leer� y procesar� el input del usuario
	void Render();						//Dibujar cosas en pantalla
	void Update();						//C�lculos del juego como posiciones
	void Delay(float elapsedFrameTime);	//Esperar, la usaremos para bloquear la tasa de FPS a 60

	bool IsOver();						//Para saber si la partida ha terminado
	bool Init();						//Inicializar todo

	Status getStatus();					//Devuelve el estado interno del juego

	void setDeltaTime(const float& dT);	//Lee el deltaTime del mainLoop
	float getDeltaTime();

protected:
	int mainMenu();						//Inicio de juego y main menu. Salir en start game
	void handleResize();				//Recalcula el tama�o de la pantalla para preservar el ratio de aspecto
	int id_new_player;					//El id de los jugadores que iremos creando
	float soundLevel;					//Nivel del sonido para ofrecerselo a las distintas clases
	float m_deltaTime;
	int actualLevel;
	DisplayManager* m_pDisplayManager;	//Referencia a la clase que gestionar� la ventana
	sf::RenderWindow* m_pWindow;		//La ventana donde se ejecutar� el juego
	DoomEngine* m_pDoomEngine;			//Referencia a nuestro motor del juego
	Player* m_pPlayer;					//Referencia al jugador
	PauseMenu* m_pPauseMenu;			//Puntero a la clase que almacenar� el men� de pausa

	//TODO PONER ESTO EN UNA ZONA DECENTE, PERO AHORA MISMO SE QUEDA EN TERMINAR MAIN MENU -> EJECUTAR ESTA CANCION DE JUEGO
	sf::Music e1m1Music;

	enum Status gameState;				//Estado del juego (m�quina de estados invent�)
};