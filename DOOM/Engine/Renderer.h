/*
* Name: Renderer.h
* Date: 30/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase encargada de renderizar el mapeado en 3D.
*/

#pragma once
#include "../maps/map.h"
#include "../HUD/HUD.h"
#include "../doomdef.h"
#include "RenderTypes.h"
#include "../DisplayManager/DisplayManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include <map>

#include "../Enemy/Soldier.h"


class Renderer
{
public:
	Renderer(sf::RenderWindow* r_window);
	~Renderer();

	void Init(Map* pMap, Player* pPlayer, DisplayManager* dm, std::vector<Soldier*> enemyList_);
	void Render(bool automap);
	void AddWallInFOV(Seg& seg, Angle V1Angle, Angle V2Angle, Angle V1AngleFromPlayer, Angle V2AngleFromPlayer);
	void InitFrame();						//Inicializa las estructuras de datos internas que ayudan a renderizar correctamente el frame

	void RenderBSPNodes();					//Auxiliar a RenderAutoMap, pinta los segs visibles al jugador

protected:

	void RenderAutoMap();
	void Render3dView();

	void AutomapPlayer();					//Auxiliar a RenderAutoMap, pinta al jugador
	void AutomapEnemy();					//Auxiliar a RenderAutoMap, pinta a los enemigos
	void AutomapWalls();					//Auxiliar a RenderAutoMap, pinta las paredes
	void RenderBSPNodes(int16_t nodeID);	//Auxiliar a RenderBSPNodes(), pinta los segs visibles al jugador
	bool IsPointOnLeftSide(int XPosition, int YPosition, int iNodeID);	//Útil para recorrer el árbol de nodos BSP
	void RenderSubsector(int subsectorID);	//Auxiliar a RenderBSPNodes(int nodeID), renderiza ssec hoja

	void ClipSolidWallsHorizontal(Seg& seg, int VertX1, int VertX2, Angle a1, Angle a2);		//Elige las solid walls cuyas vistas no están obstruidas por otras totalmente
	void ClipSolidWallsVertical(Seg& seg, int VertX1, int VertX2, Angle AngleV1, Angle AngleV2);	//Calcula la altura de las paredes a renderizar (y las renderiza de momento)
	void ClipPassWalls(Seg& seg, int VertX1, int VertX2, Angle AngleV1, Angle AngleV2);
	
	void CeilingFloorHeight(Seg& seg, int& VXScreen, float& DistToVertex, float& CeilingVScreen, float& FloorVScreen);
	void PartialSeg(Seg& seg, Angle& V1Angle, Angle& V2Angle, float& DistanceToV, bool IsLeftSide);
	void CeilingFloorUpdate(SegRenderData& render_data);

	void DrawUpperSection(SegRenderData& renderdata, int iXCurrent, int CurrentCeilingEnd, sf::Color color);
	void DrawMidSection(SegRenderData& renderdata, int iXCurrent, int CurrentCeilingEnd, int CurrentFloorStart, sf::Color color);
	void DrawLowerSection(SegRenderData& renderdata, int iXCurrent, int CurrentFloorStart, sf::Color color);

	void RenderSegment(SegRenderData& renderdata);	//Pinta por pantalla
	bool ValidateRange(SegRenderData& renderdata, int& iXCurrent, int& CurrentCeilingEnd, int& CurrentFloorStart);

	void StoreWallRange(Seg& seg, int VertX1, int VertX2, Angle a1, Angle a2);	//Auxiliar a la función de encima, de momento pinta en pantalla //WIP
	sf::Color GetWallRenderColor(std::string textName);				//Auxiliar y temporal, a cada textura del juego le asigna un color
	sf::Color SelectColor(Seg& seg);

	void RecalculateAutomapInScreen(const float& Xin, float& Xout, const float& Yin, float& Yout);
	//Auxiliar a todas las demás funciones Automap, pasa de puntos en el mapa de juego (-1700, 500 p.ej) a coordenadas de pantalla
	
	int AngleToScreen(Angle angle);

	float GetScaleFactor(int VXScreen, Angle SegToNormalAngle, float DistanceToNormal);

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
	Hud* m_pHud;									//Referencia al hud del jugador
	std::list<Cliprange> m_solidsegs;				//Lista de segmentos para pintar en pantalla
	sf::RenderWindow* m_pRenderWindow;				//Puntero a la pantalla de renderizado
	std::map<std::string, sf::Color> m_WallColor;	//Asocia un color a un nombre de textura, para renderizado temporal de paredes
	std::map<int, Angle> m_ScreenXToAngle;			//Tabla de lookup con ángulos desde Doomguy hasta cada píxel en pantalla
	std::vector<int> m_CeilingClipHeight;			//Para saber en qué y se puede dibujar el techo para cada x de pantalla
	std::vector<int> m_FloorClipHeight;				//Para saber en qué y se puede dibujar el suelo para cada x de pantalla
	std::vector<Soldier*> enemyList;				//Para saber en qué y se puede dibujar el suelo para cada x de pantalla

	/* Intento de renderizar bien los suelos, aquí ya no hay tutorial que valga, necesito bucear al código fuente :') */
	/* De momento no se usan aunque probablemente harán falta */
	std::vector<Visplane> m_visplanes;				//No tengo ni idea de lo que estoy haciendo //TODO
	Visplane* m_ceilingPlane;
	Visplane* m_floorPlane;

	DisplayManager* m_pDisplayManager;
};
