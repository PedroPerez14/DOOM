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
#include <SFML/Graphics.hpp>

Renderer::Renderer(sf::RenderWindow* r_window) : m_pMap(NULL), m_pPlayer(NULL)
{
	renderXSize = SCREENWIDTH;
	renderYSize = SCREENHEIGHT;
	automapScaleFactor = 15.0f;	//Este valor da resultados decentes al hacer automap
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
	m_pRenderWindow->clear(sf::Color::Black);
	if(automap)
	{
		RenderAutoMap();
	}
	else
	{
		Render3dView();
	}
	m_pRenderWindow->display();
}

//Permite dibujar una pared en pantalla, creo (todavía muy WIP)
void Renderer::AddWallInFOV(Seg seg, Angle V1Angle, Angle V2Angle)
{
	// Las paredes sólidas solo tienen lado derecho, luego trataré las paredes de dos caras
	if (seg.pLeftSector == nullptr)
	{
		AddSolidWall(seg, V1Angle, V2Angle);
	}
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
	AutomapPlayer();
	AutomapWalls();
}

void Renderer::Render3dView()
{
	RenderBSPNodes();
}

void Renderer::AutomapPlayer()
{
	int w = m_pRenderWindow->getView().getSize().x;
	int h = m_pRenderWindow->getView().getSize().y;
	float x_pos, y_pos;

	RecalculateAutomapInScreen(m_pPlayer->GetXPos(), x_pos, m_pPlayer->GetYPos(), y_pos);
	sf::CircleShape player_triangle(1.0f, 3);
	player_triangle.setFillColor(sf::Color::Green);
	sf::Vector2f v = sf::Vector2f(x_pos, y_pos);
	player_triangle.setPosition(v);
	m_pRenderWindow->draw(player_triangle);
}

void Renderer::AutomapWalls()
{
	for (int i = 0; i < m_pMap->getLinedefsSize(); i++)
	{
		Vertex v = *(m_pMap->getLinedef(i).vert1);
		Vertex v2 = *(m_pMap->getLinedef(i).vert2);
		float x_pos, y_pos, x_pos2, y_pos2;
		RecalculateAutomapInScreen(v.x, x_pos, v.y, y_pos);
		RecalculateAutomapInScreen(v2.x, x_pos2, v2.y, y_pos2);
		sf::Vertex vertex[] = {
			sf::Vector2f(x_pos, y_pos),
			sf::Vector2f(x_pos2, y_pos2)
		};
		m_pRenderWindow->draw(vertex, 2, sf::Lines);
	}
	/*
	sf::Vertex point[] = { (sf::Vector2f(10, 10))
	, (sf::Vector2f(10, r_window->getView().getSize().y - 10)) };
	r_window->draw(point, 2, sf::Lines);
	*/ //Dejo esto aquí como recordatorio de que soy gilipollas
}

void Renderer::RenderBSPNodes()
{
	RenderBSPNodes(m_pMap->getNodesSize() - 1);
}

void Renderer::RenderBSPNodes(int16_t nodeID)
{
	//Comprobar con la máscara si es un nodo hoja == subsector (el que buscamos)
	if ((int16_t)(nodeID & SUBSECTORIDENTIFIER))
	{
		RenderSubsector((int16_t)(nodeID & (~SUBSECTORIDENTIFIER)));
		return;
		//hay que volver a hacer el casteo porque si no saca -326XX en vez del ID que debería y explota
	}

	if (IsPointOnLeftSide(m_pPlayer->GetXPos(), m_pPlayer->GetYPos(), nodeID))
	{
		RenderBSPNodes(m_pMap->getNode(nodeID).LeftChild);
		RenderBSPNodes(m_pMap->getNode(nodeID).RightChild);
		//izquierda y luego derecha, de cerca a lejos (creo que es por eso)
	}
	else
	{
		RenderBSPNodes(m_pMap->getNode(nodeID).RightChild);
		RenderBSPNodes(m_pMap->getNode(nodeID).LeftChild);
		//derecha y luego izquierda, de cerca a lejos (creo que es por eso)
	}
}

bool Renderer::IsPointOnLeftSide(int XPosition, int YPosition, int nodeID)
{
	int dx = XPosition - m_pMap->getNode(nodeID).XPartition;
	int dy = YPosition - m_pMap->getNode(nodeID).YPartition;

	return (((dx * m_pMap->getNode(nodeID).YPartDir) - (dy * m_pMap->getNode(nodeID).XPartDir)) <= 0);    //Prod. vectorial
}

void Renderer::RenderSubsector(int subsectorID)
{
	Subsector subsector = m_pMap->getSSec(subsectorID);

	sf::Color color(rand() % 255, rand() % 255, rand() % 255);

	for (int i = 0; i < subsector.seg_count; i++)
	{
		//Para todos los segmentos del ssec pinto de un color aleatorio las paredes
		Seg seg = m_pMap->getSeg(subsector.first_segID + i);
		Vertex v = *(seg.vert1);
		Vertex v2 = *(seg.vert2);
		Angle a1, a2;   //para invocar a clipvertexesinFOV()

		if (m_pPlayer->ClipVertexesInFOV(v, v2, a1, a2))
		{
			float x_pos, y_pos, x_pos2, y_pos2;
			/*RecalculateAutomapInScreen(v.x, x_pos, v.y, y_pos);
			RecalculateAutomapInScreen(v2.x, x_pos2, v2.y, y_pos2);
			sf::Vertex vertex[] = {
				sf::Vertex(sf::Vector2f(x_pos, y_pos), color),
				sf::Vertex(sf::Vector2f(x_pos2, y_pos2), color)
			};
			m_pRenderWindow->draw(vertex, 2, sf::Lines);*/ //Esto lo hacía para el automap
			AddWallInFOV(seg, a1, a2);
		}
	}
}

void Renderer::AddSolidWall(Seg seg, Angle a1, Angle a2)
{
	int V1XScreen = AngleToScreen(a1);
	int V2XScreen = AngleToScreen(a2);

	sf::Vertex vertex[] = {
				sf::Vector2f(V1XScreen, 0),
				sf::Vector2f(V1XScreen, renderYSize)
	};
	sf::Vertex vertex2[] = {
				sf::Vector2f(V2XScreen, 0),
				sf::Vector2f(V2XScreen, renderYSize)
	};
	m_pRenderWindow->draw(vertex, 2, sf::Lines);
	m_pRenderWindow->draw(vertex2, 2, sf::Lines);
}

void Renderer::RecalculateAutomapInScreen(const float& Xin, float& Xout, const float& Yin, float& Yout)
{
	float currentheight = m_pRenderWindow->getView().getSize().y;
	float min_x = m_pMap->getAutomapInfo().minX;
	float min_y = m_pMap->getAutomapInfo().minY;
	Xout = 5.0f + (Xin - min_x) / automapScaleFactor;
	Yout = currentheight - 5.0f - (Yin - min_y) / automapScaleFactor;
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


