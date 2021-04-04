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
	m_solidsegs.clear();
	Cliprange infinity_neg, infinity_pos;
	infinity_neg.first = INT_MIN;
	infinity_neg.last = -1;

	infinity_pos.first = renderXSize;
	infinity_pos.last = INT_MAX;

	//[-inf, -1] , [320, +inf], la "pantalla" se deja vacía de momento
	m_solidsegs.push_back(infinity_neg);
	m_solidsegs.push_back(infinity_pos);
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
			AddWallInFOV(seg, a1, a2);	//TODO ahora llamar a ClipSolidWallsHorizontal y luego para todas las supervivientes, AddWallInFOV (creo)
		}
	}
}

void Renderer::AddSolidWall(Seg seg, Angle a1, Angle a2)
{
	int V1XScreen = AngleToScreen(a1);
	int V2XScreen = AngleToScreen(a2);
	ClipSolidWallsHorizontal(seg, V1XScreen, V2XScreen);
}

void Renderer::ClipSolidWallsHorizontal(Seg& seg, int VertX1, int VertX2)
{
	Cliprange current = Cliprange{ VertX1, VertX2 };

	std::list<Cliprange>::iterator wall_range = m_solidsegs.begin();
	while (wall_range != m_solidsegs.end() && wall_range->last < current.first - 1)
	{
		++wall_range;
	}

	if (current.first < wall_range->first)	//si el nuevo wall "empieza" antes que el almacenado
	{
		//Puede caber como elemento independiente o hay que actualizar el actual (corta al anterior)
		if (current.last < wall_range->first - 1)
		{
			//Independiente, insertar nuevo
			StoreWallRange(seg, current.first, current.last);
			m_solidsegs.insert(wall_range, current);
			return;
		}
		//Actualizar el inicio del actual a un valor menor
		StoreWallRange(seg, current.first, wall_range->first - 1);
		wall_range->first = current.first;
	}

	if (current.last <= wall_range->last)	//Si empieza y termina "dentro" de lo que ya había
		return;								// no hay que hacer nada

	//Si nada de lo anterior se ha dado, tenemos que fijarnos en la parte derecha
	//	puede estar ocultando parte del muro almacenado, o empalman a la perfección
	//	Además, también hay que considerar qué puede estar pasando con otros muros
	std::list<Cliprange>::iterator next_wall = wall_range;

	//Recorrer muros "a la derecha" que clipeen parcial (o totalmente) con el nuestro
	while (current.last >= next(next_wall, 1)->first - 1) 
	{
		StoreWallRange(seg, next_wall->last + 1, next(next_wall, 1)->first - 1);
		++next_wall;

		if (current.last <= next_wall->last)	//Si es el último con el que clipea o empalma
		{
			wall_range->last = next_wall->last;	//Actualizamos para incluir todo ese segmento en UNA entrada
			if (next_wall != wall_range)
			{
				//Y si son diferentes entradas, borramos las demás entradas del medio ya que se unifican
				wall_range++;
				next_wall++;
				m_solidsegs.erase(wall_range, next_wall);
			}
			return;
		}
	}

	//Si llegamos aquí, es porque no clipea con nada más a la derecha 
	//	pero sigue quedando pendiente el muro con el que estábamos comparando originalmente
	//	y los que hemos ido encontrando por el camino, actualizamos y borramos los elementos intermedios

	StoreWallRange(seg, next_wall->last + 1, current.last);
	wall_range->last = current.last;
	if (next_wall != wall_range)
	{
		wall_range++;
		next_wall++;
		m_solidsegs.erase(wall_range, next_wall);
	}
}

void Renderer::RecalculateAutomapInScreen(const float& Xin, float& Xout, const float& Yin, float& Yout)
{
	float currentheight = m_pRenderWindow->getView().getSize().y;
	float min_x = m_pMap->getAutomapInfo().minX;
	float min_y = m_pMap->getAutomapInfo().minY;
	Xout = 5.0f + (Xin - min_x) / automapScaleFactor;
	Yout = currentheight - 5.0f - (Yin - min_y) / automapScaleFactor;
}

void Renderer::StoreWallRange(Seg& seg, int VertX1, int VertX2)
{
	sf::RectangleShape rect(sf::Vector2f(VertX2 - VertX1 + 1, renderYSize));
	rect.setPosition(VertX1, 0);
	rect.setFillColor(GetWallRenderColor(seg.pLinedef->sidedef_r->MiddleTexture));
	rect.setOutlineThickness(0.0f);
	m_pRenderWindow->draw(rect);

	/*sf::Vertex vertex[] = {
				sf::Vector2f(VertX1, 0),
				sf::Vector2f(VertX1, renderYSize)
	};
	sf::Vertex vertex2[] = {
				sf::Vector2f(VertX2, 0),
				sf::Vector2f(VertX2, renderYSize)
	};
	m_pRenderWindow->draw(vertex, 2, sf::Lines);
	m_pRenderWindow->draw(vertex2, 2, sf::Lines);*/
}

sf::Color Renderer::GetWallRenderColor(std::string textName)
{
	if (m_WallColor.count(textName))
	{
		return m_WallColor[textName];
	}
	else
	{
		sf::Color color{ (uint8_t)(rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255) };
		m_WallColor[textName] = color;
		return color;
	}
	return sf::Color();
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


