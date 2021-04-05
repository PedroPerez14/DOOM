/*
* Name: Renderer.h
* Date: 30/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase encargada de renderizar el mapeado en 3D.
*/

#pragma once
#include "../maps/map.h"
#include "../doomdef.h"
#include "RenderTypes.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include <map>


class Renderer
{
public:
	Renderer(sf::RenderWindow* r_window);
	~Renderer();

	void Init(Map* pMap, Player* pPlayer);
	void Render(bool automap);
	void AddWallInFOV(Seg seg, Angle V1Angle, Angle V2Angle);
	void InitFrame();						//Inicializa las estructuras de datos internas que ayudan a renderizar correctamente el frame

	void RenderBSPNodes();					//Auxiliar a RenderAutoMap, pinta los segs visibles al jugador

protected:

	void RenderAutoMap();
	void Render3dView();

	void AutomapPlayer();					//Auxiliar a RenderAutoMap, pinta al jugador
	void AutomapWalls();					//Auxiliar a RenderAutoMap, pinta las paredes
	void RenderBSPNodes(int16_t nodeID);	//Auxiliar a RenderBSPNodes(), pinta los segs visibles al jugador
	bool IsPointOnLeftSide(int XPosition, int YPosition, int iNodeID);	//Útil para recorrer el árbol de nodos BSP
	void RenderSubsector(int subsectorID);	//Auxiliar a RenderBSPNodes(int nodeID), renderiza ssec hoja

	void AddSolidWall(Seg seg, Angle a1, Angle a2);

	void ClipSolidWallsHorizontal(Seg& seg, int VertX1, int VertX2, Angle a1, Angle a2);		//Elige las solid walls cuyas vistas no están obstruidas por otras totalmente
	void ClipSolidWallsVertical(Seg& seg, int VertX1, int VertX2, Angle AngleV1, Angle AngleV2);	//Calcula la altura de las paredes a renderizar (y las renderiza de momento)
	void CeilingFloorHeight(Seg& seg, int& VXScreen, float& DistToVertex, float& CeilingVScreen, float& FloorVScreen);
	void PartialSeg(Seg& seg, Angle& V1Angle, Angle& V2Angle, float& DistanceToV, bool IsLeftSide);


	void StoreWallRange(Seg& seg, int VertX1, int VertX2, Angle a1, Angle a2);	//Auxiliar a la función de encima, de momento pinta en pantalla //WIP
	sf::Color GetWallRenderColor(std::string textName);				//Auxiliar y temporal, a cada textura del juego le asigna un color

	void RecalculateAutomapInScreen(const float& Xin, float& Xout, const float& Yin, float& Yout);
	//Auxiliar a todas las demás funciones Automap, pasa de puntos en el mapa de juego (-1700, 500 p.ej) a coordenadas de pantalla
	
	int AngleToScreen(Angle angle);


	/*
	 *	Variables
	 */

	int renderXSize;								//Anchura de la view en sfml
	int renderYSize;								//Altura de la view de sfml
	float automapScaleFactor;						//15, solo para que quede bonito el automap

	//Variables necesarias para trigonometría
	float m_halfRenderXSize;						// renderXSize / 2
	float m_halfRenderYSize;						// renderYSize / 2
	float m_iDistancePlayerToScreen;				// debería ser 160 si todo es estándar 

	Map* m_pMap;									//Referencia al mapa
	Player* m_pPlayer;								//Referencia al jugador
	std::list<Cliprange> m_solidsegs;				//Lista de segmentos para pintar en pantalla
	sf::RenderWindow* m_pRenderWindow;				//Puntero a la pantalla de renderizado
	std::map<std::string, sf::Color> m_WallColor;	//Asocia un color a un nombre de textura, para renderizado temporal de paredes
	std::map<int, Angle> m_ScreenXToAngle;			//Tabla de lookup con ángulos desde Doomguy hasta cada píxel en pantalla
};
