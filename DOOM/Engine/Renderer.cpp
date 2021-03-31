/*
* Name: Renderer.cpp
* Date: 30/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase encargada de renderizar el mapeado en 3D.
*/

#pragma once
#include "Renderer.h"
#include "../doomdef.h"
#include <corecrt_math_defines.h>

Renderer::Renderer(sf::RenderWindow* r_window) : m_pMap(NULL), m_pPlayer(NULL)
{
	renderXSize = SCREENWIDTH;
	renderYSize = SCREENHEIGHT;
	automapScaleFactor = 15;	//Este valor da resultados decentes
	m_pRenderWindow = r_window;
}

Renderer::~Renderer()
{
}

void Renderer::Init(Map* pMap, Player* pPlayer)
{
	m_pMap = pMap;
	m_pPlayer = pPlayer;
}

void Renderer::Render(bool automap)
{
	//TODO
	/*
	if(automap)
	{
		poner el automap que tengo de momento en map.cpp
	}
	else
	{
		renderizar en 3d con la funcion de esta clase?
	}
	*/
}


//Permite dibujar una pared en pantalla, creo (todavía muy WIP)
void Renderer::AddWallInFOV(Seg seg, Angle V1Angle, Angle V2Angle)
{
	int V1XScreen = AngleToScreen(V1Angle);
	int V2XScreen = AngleToScreen(V2Angle);

	int w = m_pRenderWindow->getSize().x;
	int h = m_pRenderWindow->getSize().y;

	float x_scr, x_scr2, y_scr, y_scr2;	//Píxeles para plotear las líneas
	//convertToScreenCords(V1XScreen, x_scr, 0.0f, y_scr, w, h, renderXSize, renderYSize);
	//convertToScreenCords(V2XScreen, x_scr2, renderYSize, y_scr2, w, h, renderXSize, renderYSize);
	//TODO esto va a ser un dolor de cabeza, lo presiento
}

void Renderer::InitFrame()
{
}

void Renderer::DrawRect(int X1, int Y1, int X2, int Y2)
{
}

void Renderer::DrawLine(int X1, int Y1, int X2, int Y2)
{
}

void Renderer::RenderAutoMap()
{
}

void Renderer::Render3dView()
{
}

//Pasa de un ángulo (de un vértice o punto) a un píxel en la pantalla final
//	teniendo en cuenta que la resolución original de DOOM es de 320x200
//TODO a lo mejor no tengo que tener en cuenta el 320x200 y tengo que usar el tamaño de la ventana/view?
int Renderer::AngleToScreen(Angle angle)
{
	const float playerFOV = m_pPlayer->getFOV();
	const float half_width = (renderXSize / 2);
	int i_x = 0;	//Pixel de la pantalla a lo ancho

	//Si estamos en el lado izquierdo
	if (angle > playerFOV)
	{
		angle -= playerFOV;
		i_x = half_width - round(tanf(angle.GetValue() * (float)M_PI / 180.0f) * half_width);
	}
	else //Lado derecho
	{
		angle = playerFOV - angle.GetValue();
		i_x = half_width + round(tanf(angle.GetValue() * (float)M_PI / 180.0f) * half_width);
	}
	return i_x;
}


