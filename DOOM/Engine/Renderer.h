/*
* Name: Renderer.h
* Date: 30/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase encargada de renderizar el mapeado en 3D.
*/

#pragma once
#include "../maps/map.h"
#include <SFML/Graphics/RenderWindow.hpp>


class Renderer
{
public:
	Renderer(sf::RenderWindow* r_window);
	~Renderer();

	void Init(Map* pMap, Player* pPlayer);
	void Render(bool automap);
	void AddWallInFOV(Seg seg, Angle V1Angle, Angle V2Angle);
	void InitFrame();
	void DrawRect(int X1, int Y1, int X2, int Y2);
	void DrawLine(int X1, int Y1, int X2, int Y2);
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


	void RecalculateAutomapInScreen(const float& Xin, float& Xout, const float& Yin, float& Yout);
	//Auxiliar a todas las demás funciones Automap, pasa de puntos en el mapa de juego (-1700, 500 p.ej) a coordenadas de pantalla
	
	int AngleToScreen(Angle angle);

	int renderXSize;
	int renderYSize;
	float automapScaleFactor;

	Map* m_pMap;
	Player* m_pPlayer;
	sf::RenderWindow* m_pRenderWindow;
};
