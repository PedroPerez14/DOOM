/*
* Name: DoomEngine.h
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Motor del juego DOOM.
*/

#pragma once
#include "SFML/Graphics.hpp"
#include "../WAD/wad_loader.h"
#include "../maps/map.h"
#include "../Game/GameStates.h"
#include "Renderer.h"
#include "../DisplayManager/DisplayManager.h"
#include "../PatchesTextures/AssetsManager.h"
#include "../Enemy/Soldier.h"
#include "../Game/GameStates.h"

class DoomEngine
{
public:
	DoomEngine(Player* player, DisplayManager* dm, std::string level);
	~DoomEngine();

	virtual void Render();							//Dibujar en pantalla
	virtual void KeyPressed(sf::Event& event);		//Teclas(s) pulsada(s)
	virtual void KeyReleased(sf::Event& event);		//Teclas(s) soltada(s)
	virtual void releasePlayerInputs();				//Para cuando abramos el menú de pausa
	virtual void Quit();							//Cerrar el juego
	virtual bool Update(Status status);
	virtual void endProcess();
	bool testIfVisible(Soldier* enemigo, Vertex* v3);							//Testea si el enemigo en concreto es visible al jugador

	virtual bool isOver();							//Para saber si ha terminado el juego
	virtual bool Init(sf::RenderWindow* r_window, Status* gameState);	//Inicializar DoomEngine (que a su vez inicaliza el renderer)
	virtual int GetRendererWidth();					//Anchura del renderer
	virtual int GetRendererHeight();				//Altura del renderer

	virtual std::string GetName();
	virtual std::string GetWADFileName();

	virtual void setDeltaTime(const float& dT);		//El game loop le informa al motor
	virtual float getDeltaTime();					//Para obtener el deltaTime

protected:
	float m_deltaTime;								//Tiempo entre frames, registrado por el game loop
	int step;
	int rendererWidth, rendererHeight;
	bool m_isOver;									//True sii el juego ha terminado y debe cerrar la ventana
	bool showAutomap;								//Indica si a la hora de renderizar hay que mostrar el automapa o el juego
	WADLoader m_WADLoader;							//El engine se encarga de gestionar el cargador de .WADs
	Map* m_pMap;									//Y también el mapa
	Player* m_pPlayer;								//Y al jugador
	Renderer* m_pRenderer;							//Ahora también tenemos una clase especializada en renderizar
	DisplayManager* m_pDisplayManager;
	AssetsManager* m_pAssetsManager;				//estos dos punteros me los invento un poco pero tiene sentido que estén (creo)
	sf::RenderWindow* m_pRenderWindow;				//borrar luego si eso

	std::vector<Soldier*> enemyList;

};
