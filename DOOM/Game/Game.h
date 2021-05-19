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
#include "GameStates.h"
#include "../PauseMenu/PauseMenu.h"
#include "../DisplayManager/DisplayManager.h"

class Game
{
public:
	Game();
	~Game();

	void ProcessInput(Status status);	//Leerá y procesará el input del usuario
	void Render();						//Dibujar cosas en pantalla
	void Update();						//Cálculos del juego como posiciones
	void Delay(float elapsedFrameTime);	//Esperar, la usaremos para bloquear la tasa de FPS a 60

	bool IsOver();						//Para saber si la partida ha terminado
	bool Init();						//Inicializar todo

	Status getStatus();					//Devuelve el estado interno del juego

	void setDeltaTime(const float& dT);	//Lee el deltaTime del mainLoop
	float getDeltaTime();

protected:
	int mainMenu();						//Inicio de juego y main menu. Salir en start game
	void handleResize();				//Recalcula el tamaño de la pantalla para preservar el ratio de aspecto
	int id_new_player;					//El id de los jugadores que iremos creando
	float soundLevel;					//Nivel del sonido para ofrecerselo a las distintas clases
	float m_deltaTime;
	int actualLevel;
	DisplayManager* m_pDisplayManager;	//Referencia a la clase que gestionará la ventana
	sf::RenderWindow* m_pWindow;		//La ventana donde se ejecutará el juego
	DoomEngine* m_pDoomEngine;			//Referencia a nuestro motor del juego
	Player* m_pPlayer;					//Referencia al jugador
	PauseMenu* m_pPauseMenu;			//Puntero a la clase que almacenará el menú de pausa

	//TODO PONER ESTO EN UNA ZONA DECENTE, PERO AHORA MISMO SE QUEDA EN TERMINAR MAIN MENU -> EJECUTAR ESTA CANCION DE JUEGO
	sf::Music e1m1Music;

	enum Status gameState;				//Estado del juego (máquina de estados invent®)
};