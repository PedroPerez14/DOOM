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

protected:

	void RenderAutoMap();
	void Render3dView();
	
	int AngleToScreen(Angle angle);

	int renderXSize;
	int renderYSize;
	int automapScaleFactor;

	Map* m_pMap;
	Player* m_pPlayer;
	sf::RenderWindow* m_pRenderWindow;
};
