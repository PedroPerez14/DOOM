/*
* Name: DoomEngine.h
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Motor del juego DOOM.
*/

#pragma once
#include "SFML/Graphics.hpp"
#include "../WAD/wad_loader.h"
#include "../maps/map.h"
#include "../Game/GameStates.h"
#include "Renderer.h"
#include "../DisplayManager/DisplayManager.h"

class DoomEngine
{
public:
	DoomEngine(Player* player, DisplayManager* dm);
	~DoomEngine();

	virtual void Render();							//Dibujar en pantalla
	virtual void KeyPressed(sf::Event& event);		//Teclas(s) pulsada(s)
	virtual void KeyReleased(sf::Event& event);		//Teclas(s) soltada(s)
	virtual void Quit();							//Cerrar el juego
	virtual void Update(Status status);

	virtual bool isOver();							//Para saber si ha terminado el juego
	virtual bool Init(sf::RenderWindow* r_window);	//Inicializar DoomEngine (que a su vez inicaliza el renderer)
	virtual int GetRendererWidth();					//Anchura del renderer
	virtual int GetRendererHeight();				//Altura del renderer
	virtual int GetTimePerFrame();					//Tiempo para renderizar un frame

	virtual std::string GetName();
	virtual std::string GetWADFileName();
protected:
	int step;
	int rendererWidth, rendererHeight;
	bool m_isOver;									//True sii el juego ha terminado y debe cerrar la ventana
	bool showAutomap;								//Indica si a la hora de renderizar hay que mostrar el automapa o el juego
	WADLoader m_WADLoader;							//El engine se encarga de gestionar el cargador de .WADs
	Map* m_pMap;									//Y tambi�n el mapa
	Player* m_pPlayer;								//Y al jugador
	Renderer* m_pRenderer;							//Ahora tambi�n tenemos una clase especializada en renderizar
	DisplayManager* m_pDisplayManager;
	std::vector<Soldier*> enemyList;				//Lista de enemigos presentes en el nivel
};
