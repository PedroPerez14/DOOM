/*
* Name: DoomEngine.h
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Motor del juego DOOM.
*/

#pragma once
#include "../maps/map.h"
#include "../WAD/wad_loader.h"

class DoomEngine
{
public:
	DoomEngine();
	~DoomEngine();

	virtual void Render(sf::RenderWindow r_window);	//Dibujar en pantalla
	virtual void KeyPressed(sf::Event& event);		//Teclas(s) pulsada(s)
	virtual void KeyReleased(sf::Event& event);		//Teclas(s) soltada(s)
	virtual void Quit();							//Cerrar el juego
	virtual void Update();

	virtual bool isOver();							//Para saber si ha terminado el juego
	virtual void Init();							//Inicializar DoomEngine
	virtual int GetRendererWidth();					//Anchura del renderer
	virtual int GetRendererHeight();				//Altura del renderer
	virtual int GetTimePerFrame();					//Tiempo para renderizar un frame

	virtual std::string GetName();
	virtual std::string GetWADFileName();
protected:
	int renderWidth, renderHeight;
	bool isOver;
	WADLoader m_WADLoader;							//El engine se encarga de gestionar el cargador de .WADs
	Map* m_pMap;									//Y también el mapa
};