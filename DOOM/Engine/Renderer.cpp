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
	automapScaleFactor = 2.0f * (2560.0f / SCREENWIDTH);	//Este valor da resultados decentes al hacer automap
	m_pRenderWindow = r_window;
}

Renderer::~Renderer()
{
}

void Renderer::Init(Map* pMap, Player* pPlayer)
{
	m_pMap = pMap;
	m_pPlayer = pPlayer;

	renderXSize = m_pRenderWindow->getView().getSize().x;
	renderYSize = m_pRenderWindow->getView().getSize().y;

	m_CeilingClipHeight.resize(renderXSize);
	m_FloorClipHeight.resize(renderXSize);

	m_halfRenderXSize = renderXSize / 2.0f;
	m_halfRenderYSize = renderYSize / 2.0f;
	Angle halfFOV = m_pPlayer->getFOV() / 2.0f;

	m_iDistancePlayerToScreen = m_halfRenderXSize / halfFOV.getTan();

	//Se inicializa la tabla de lookup m_ScreenXToAngle
	// contiene el ángulo desde doomguy hasta cada "pixel" de la pantalla
	
	for (int i = 0; i <= renderXSize; i++)
	{
		m_ScreenXToAngle[i] = Angle(atan((m_halfRenderXSize - i) / (float)m_iDistancePlayerToScreen) * 180.0f / (float)M_PI);
	}

	m_pHud = new Hud(m_pRenderWindow, pPlayer);
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
		m_pPlayer->renderPlayer(m_pRenderWindow);
		m_pHud->drawHud();
	}
}

//Permite dibujar una pared en pantalla, creo (todavía muy WIP)
void Renderer::AddWallInFOV(Seg& seg, Angle V1Angle, Angle V2Angle, Angle V1AngleFromPlayer, Angle V2AngleFromPlayer)
{
	int V1XScreen = AngleToScreen(V1AngleFromPlayer);
	int V2XScreen = AngleToScreen(V2AngleFromPlayer);
	//TODO si voy a poner más casos aquí mejor poner un switch o qué (?)
	// Las paredes sólidas solo tienen lado derecho
	if (seg.pLeftSector == nullptr)
	{
		ClipSolidWallsHorizontal(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
		return;
	}

	//Y si tiene dos lados no tiene por qué ser un 'portal' (pared de 2 caras o 'window')
	//	para eso tenemos que comprobar que hay diferentes alturas de techo/suelo o diferentes techos
	//	en los sectores izquierdo y derecho del seg
	if (seg.pRightSector->CeilingHeight != seg.pLeftSector->CeilingHeight || 
		seg.pRightSector->FloorHeight != seg.pLeftSector->FloorHeight)
	{
		ClipPassWalls(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);	//Procede parecido a ClipWallsHorizontal pero con los 'portals'
		return;
	}

	//Podemos tratar las puertas cerradas como una pared sólida
	if (seg.pLeftSector->CeilingHeight <= seg.pRightSector->FloorHeight
		|| seg.pLeftSector->FloorHeight >= seg.pRightSector->CeilingHeight)
	{
		ClipSolidWallsHorizontal(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
		return;
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

	// -----------------------
	// Llenamos los vectores con -1's y el tamaño de la altura de la pantalla
	std::fill(m_CeilingClipHeight.begin(), m_CeilingClipHeight.end(), -1);
	std::fill(m_FloorClipHeight.begin(), m_FloorClipHeight.end(), renderYSize);
	//TODO al dibujar segs en pantalla, actualizar estos vectores (?)

	//TODO R_ClearPlanes ?????? ayuda me va a explotar la cabeza por favor
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
	sf::CircleShape player_triangle(3.0f, 3);
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
	Subsector &subsector = m_pMap->getSSec(subsectorID);

	for (int i = 0; i < subsector.seg_count; i++)
	{
		Seg &seg = m_pMap->getSeg(subsector.first_segID + i);
		Vertex v = *(seg.vert1);
		Vertex v2 = *(seg.vert2);
		Angle a1, a2, a1fromPlayer, a2fromPlayer;   //para invocar a clipvertexesinFOV()

		if (m_pPlayer->ClipVertexesInFOV(v, v2, a1, a2, a1fromPlayer, a2fromPlayer))
		{
			AddWallInFOV(seg, a1, a2, a1fromPlayer, a2fromPlayer);	//TODO ahora llamar a ClipSolidWallsHorizontal y luego para todas las supervivientes, AddWallInFOV (creo)
		}
	}
}

void Renderer::ClipSolidWallsHorizontal(Seg& seg, int VertX1, int VertX2, Angle a1, Angle a2)
{
	if (m_solidsegs.size() < 2)
	{
		return;
	}

	Cliprange current = Cliprange{ VertX1, VertX2 };

	std::list<Cliprange>::iterator FoundClipWall  = m_solidsegs.begin();
	while (FoundClipWall  != m_solidsegs.end() && FoundClipWall ->last < current.first - 1)
	{
		++FoundClipWall ;
	}

	if (current.first < FoundClipWall ->first)	//si el nuevo wall "empieza" antes que el almacenado
	{
		//Puede caber como elemento independiente o hay que actualizar el actual (corta al anterior)
		if (current.last < FoundClipWall ->first - 1)
		{
			//Independiente, insertar nuevo
			StoreWallRange(seg, current.first, current.last, a1, a2);
			m_solidsegs.insert(FoundClipWall , current);
			return;
		}
		//Actualizar el inicio del actual a un valor menor
		StoreWallRange(seg, current.first, FoundClipWall ->first - 1, a1, a2);
		FoundClipWall->first = current.first;
	}

	if (current.last <= FoundClipWall ->last)	//Si empieza y termina "dentro" de lo que ya había
		return;								// no hay que hacer nada

	//Si nada de lo anterior se ha dado, tenemos que fijarnos en la parte derecha
	//	puede estar ocultando parte del muro almacenado, o empalman a la perfección
	//	Además, también hay que considerar qué puede estar pasando con otros muros
	std::list<Cliprange>::iterator next_wall = FoundClipWall ;

	//Recorrer muros "a la derecha" que clipeen parcial (o totalmente) con el nuestro
	while (current.last >= next(next_wall, 1)->first - 1) 
	{
		StoreWallRange(seg, next_wall->last + 1, next(next_wall, 1)->first - 1, a1, a2);
		++next_wall;

		if (current.last <= next_wall->last)	//Si es el último con el que clipea o empalma
		{
			FoundClipWall ->last = next_wall->last;	//Actualizamos para incluir todo ese segmento en UNA entrada
			if (next_wall != FoundClipWall )
			{
				//Y si son diferentes entradas, borramos las demás entradas del medio ya que se unifican
				FoundClipWall ++;
				next_wall++;
				m_solidsegs.erase(FoundClipWall , next_wall);
			}
			return;
		}
	}

	//Si llegamos aquí, es porque no clipea con nada más a la derecha 
	//	pero sigue quedando pendiente el muro con el que estábamos comparando originalmente
	//	y los que hemos ido encontrando por el camino, actualizamos y borramos los elementos intermedios

	StoreWallRange(seg, next_wall->last + 1, current.last, a1, a2);
	FoundClipWall ->last = current.last;
	if (next_wall != FoundClipWall )
	{
		FoundClipWall ++;
		next_wall++;
		m_solidsegs.erase(FoundClipWall , next_wall);
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

void Renderer::ClipPassWalls(Seg& seg, int VertX1, int VertX2, Angle AngleV1, Angle AngleV2)
{
	Cliprange current = Cliprange{ VertX1, VertX2 };

	std::list<Cliprange>::iterator FoundClipWall = m_solidsegs.begin();
	while (FoundClipWall != m_solidsegs.end() && FoundClipWall->last < current.first - 1)
	{
		++FoundClipWall;
	}

	if (current.first < FoundClipWall->first)	//si el nuevo wall "empieza" antes que el almacenado
	{
		//Puede caber como elemento independiente o hay que actualizar el actual (corta al anterior)
		if (current.last < FoundClipWall->first - 1)
		{
			//Independiente, insertar nuevo
			StoreWallRange(seg, current.first, current.last, AngleV1, AngleV2);
			return;
		}
		//Actualizar el inicio del actual a un valor menor
		StoreWallRange(seg, current.first, FoundClipWall->first - 1, AngleV1, AngleV2);
	}

	if (current.last <= FoundClipWall->last)	//Si empieza y termina "dentro" de lo que ya había
		return;								// no hay que hacer nada

	//Si nada de lo anterior se ha dado, tenemos que fijarnos en la parte derecha
	//	puede estar ocultando parte del muro almacenado, o empalman a la perfección
	//	Además, también hay que considerar qué puede estar pasando con otros muros
	std::list<Cliprange>::iterator next_wall = FoundClipWall;

	//Recorrer muros "a la derecha" que clipeen parcial (o totalmente) con el nuestro
	while (current.last >= next(next_wall, 1)->first - 1)
	{
		StoreWallRange(seg, next_wall->last + 1, next(next_wall, 1)->first - 1, AngleV1, AngleV2);
		++next_wall;

		if (current.last <= next_wall->last)	//Si es el último con el que clipea o empalma
		{
			return;
		}
	}

	//Si llegamos aquí, es porque no clipea con nada más a la derecha 
	//	pero sigue quedando pendiente el muro con el que estábamos comparando originalmente
	//	y los que hemos ido encontrando por el camino, actualizamos y borramos los elementos intermedios

	StoreWallRange(seg, next_wall->last + 1, current.last, AngleV1, AngleV2);
}

void Renderer::StoreWallRange(Seg& seg, int VertX1, int VertX2, Angle a1, Angle a2)
{
	//TODO de momento vamos a llamar a la función que se encarga de controlar las alturas y las vamos a renderizar
	ClipSolidWallsVertical(seg, VertX1, VertX2, a1, a2);
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

sf::Color Renderer::SelectColor(Seg& seg)
{
	if (seg.pLeftSector)
	{
		return GetWallRenderColor(seg.pLinedef->sidedef_r->UpperTexture);
	}
	else
	{
		return GetWallRenderColor(seg.pLinedef->sidedef_r->MiddleTexture);
	}
}

//TODO cambiar para que use el SegRenderData
void Renderer::ClipSolidWallsVertical(Seg& seg, int VertX1, int VertX2, Angle AngleV1, Angle AngleV2)
{
	SegRenderData renderdata {0};

	renderdata.pSeg = &seg;
	renderdata.VertX1OnScreen = VertX1;
	renderdata.VertX2OnScreen = VertX2;
	renderdata.AngleV1 = AngleV1;
	renderdata.AngleV2 = AngleV2;
	renderdata.DistToV1 = m_pPlayer->distanceToEdge(*seg.vert1);

	Angle Angle90(90);
	Angle SegToNormalAngle = seg.angle + Angle90;
	Angle NomalToV1Angle = SegToNormalAngle.GetValue() - AngleV1.GetValue();
	// Normal angle is 90 degree to wall
	Angle SegToPlayerAngle = Angle90 - NomalToV1Angle;

	renderdata.DistToNormal = SegToPlayerAngle.getSin() * renderdata.DistToV1;
	renderdata.V1ScaleFactor = GetScaleFactor(VertX1, SegToNormalAngle, renderdata.DistToNormal);
	renderdata.V2ScaleFactor = GetScaleFactor(VertX2, SegToNormalAngle, renderdata.DistToNormal);
	renderdata.Steps = (renderdata.V2ScaleFactor - renderdata.V1ScaleFactor) / (VertX2 - VertX1);

	renderdata.RSecCeiling = seg.pRightSector->CeilingHeight - m_pPlayer->GetZPos();
	renderdata.RSecFloor = seg.pRightSector->FloorHeight - m_pPlayer->GetZPos();
	renderdata.CeilingStep = -(renderdata.RSecCeiling * renderdata.Steps);
	renderdata.CeilingEnd = m_halfRenderYSize - (renderdata.RSecCeiling * renderdata.V1ScaleFactor);
	renderdata.FloorStep = -(renderdata.RSecFloor * renderdata.Steps);
	renderdata.FloorStart = m_halfRenderYSize - (renderdata.RSecFloor * renderdata.V1ScaleFactor);

	if (seg.pLeftSector)
	{
		renderdata.LSecCeiling = seg.pLeftSector->CeilingHeight - m_pPlayer->GetZPos();
		renderdata.LSecFloor = seg.pLeftSector->FloorHeight - m_pPlayer->GetZPos();
		CeilingFloorUpdate(renderdata);	

		if (renderdata.LSecCeiling < renderdata.RSecCeiling)
		{
			renderdata.DrawUpper = true;
			renderdata.UpperHeightStep = -(renderdata.LSecCeiling * renderdata.Steps);
			renderdata.iUpperHeight = round(m_halfRenderYSize - (renderdata.LSecCeiling * renderdata.V1ScaleFactor));
		}
		if (renderdata.LSecFloor > renderdata.RSecFloor)
		{
			renderdata.DrawLower = true;
			renderdata.LowerHeightStep = -(renderdata.LSecFloor * renderdata.Steps);
			renderdata.iLowerHeight = round(m_halfRenderYSize - (renderdata.LSecFloor * renderdata.V1ScaleFactor));
		}
	}
	//Por fin tenemos todos los datos para renderizar un segmento
	RenderSegment(renderdata);
}

void Renderer::DrawUpperSection(SegRenderData& renderdata, int iXCurrent, int CurrentCeilingEnd, sf::Color color)
{
	if (renderdata.DrawUpper)
	{
		int iUpperHeight = renderdata.iUpperHeight;
		renderdata.iUpperHeight += renderdata.UpperHeightStep;

		if (iUpperHeight >= m_FloorClipHeight[iXCurrent])
		{
			iUpperHeight = m_FloorClipHeight[iXCurrent] - 1;
		}

		if (iUpperHeight >= CurrentCeilingEnd)
		{
			sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, CurrentCeilingEnd), color),
			sf::Vertex(sf::Vector2f(iXCurrent, iUpperHeight), color)
			};
			m_pRenderWindow->draw(line, 2, sf::Lines);
			m_CeilingClipHeight[iXCurrent] = iUpperHeight;
		}
		else
		{
			m_CeilingClipHeight[iXCurrent] = CurrentCeilingEnd - 1;
		}
	}
	else if (renderdata.UpdateCeiling)
	{
		m_CeilingClipHeight[iXCurrent] = CurrentCeilingEnd - 1;
	}
}

void Renderer::DrawMidSection(SegRenderData& renderdata, int iXCurrent, int CurrentCeilingEnd, int CurrentFloorStart, sf::Color color)
{
	sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, CurrentCeilingEnd), color),
			sf::Vertex(sf::Vector2f(iXCurrent, CurrentFloorStart), color)
	};
	m_pRenderWindow->draw(line, 2, sf::Lines);
	m_CeilingClipHeight[iXCurrent] = renderYSize;
	m_FloorClipHeight[iXCurrent] = -1;
}

void Renderer::DrawLowerSection(SegRenderData& renderdata, int iXCurrent, int CurrentFloorStart, sf::Color color)
{
	if (renderdata.DrawLower)
	{
		int iLowerHeight = renderdata.iLowerHeight;
		renderdata.iLowerHeight += renderdata.LowerHeightStep;

		if (iLowerHeight <= m_CeilingClipHeight[iXCurrent])
		{
			iLowerHeight = m_CeilingClipHeight[iXCurrent] + 1;
		}

		if (iLowerHeight <= CurrentFloorStart)
		{
			sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, iLowerHeight), color),
			sf::Vertex(sf::Vector2f(iXCurrent, CurrentFloorStart), color)
			};
			m_pRenderWindow->draw(line, 2, sf::Lines);
			m_FloorClipHeight[iXCurrent] = iLowerHeight;
		}
		else
			m_FloorClipHeight[iXCurrent] = CurrentFloorStart + 1;
	}
	else if (renderdata.UpdateFloor)
	{
		m_FloorClipHeight[iXCurrent] = CurrentFloorStart + 1;
	}
}

void Renderer::RenderSegment(SegRenderData& renderdata)
{
	sf::Color color;
	color = SelectColor(*(renderdata.pSeg));
	int iXCurrent = renderdata.VertX1OnScreen;

	while (iXCurrent <= renderdata.VertX2OnScreen)
	{
		int currentCeilingEnd = renderdata.CeilingEnd;
		int currentFloorStart = renderdata.FloorStart;

		if (!ValidateRange(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart))	//Solo hacemos cosas si el rango es válido
		{
			continue;
		}
		if (renderdata.pSeg->pLeftSector)
		{
			//pintar arriba y abajo
			//techo
			sf::Color color3 = GetWallRenderColor(renderdata.pSeg->pRightSector->CeilingTexture);
			sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, currentCeilingEnd), color3),
			sf::Vertex(sf::Vector2f(iXCurrent, m_CeilingClipHeight[iXCurrent]), color3)
			};
			m_pRenderWindow->draw(line, 2, sf::Lines);
			DrawUpperSection(renderdata, iXCurrent, currentCeilingEnd, color);

			//Suelo
			sf::Color color2 = GetWallRenderColor(renderdata.pSeg->pRightSector->FloorTexture);
			sf::Vertex line2[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, currentFloorStart), color2),
			sf::Vertex(sf::Vector2f(iXCurrent, m_FloorClipHeight[iXCurrent]), color2)
			};
			m_pRenderWindow->draw(line2, 2, sf::Lines);
			DrawLowerSection(renderdata, iXCurrent, currentFloorStart, color);

		}
		else
		{
			//pintar el medio de todo
			//techo

			sf::Color color3 = GetWallRenderColor(renderdata.pSeg->pRightSector->CeilingTexture);
			sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, currentCeilingEnd), color3),
			sf::Vertex(sf::Vector2f(iXCurrent, m_CeilingClipHeight[iXCurrent]), color3)
			};
			m_pRenderWindow->draw(line, 2, sf::Lines);

			//Suelo
			sf::Color color2 = GetWallRenderColor(renderdata.pSeg->pRightSector->FloorTexture);
			sf::Vertex line2[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, currentFloorStart), color2),
			sf::Vertex(sf::Vector2f(iXCurrent, m_FloorClipHeight[iXCurrent]), color2)
			};
			m_pRenderWindow->draw(line2, 2, sf::Lines);
			DrawMidSection(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, color);
		}

		++iXCurrent;
		renderdata.CeilingEnd += renderdata.CeilingStep;
		renderdata.FloorStart += renderdata.FloorStep;
	}
}


bool Renderer::ValidateRange(SegRenderData& renderdata, int& iXCurrent, int& CurrentCeilingEnd, int& CurrentFloorStart)
{
	if (CurrentCeilingEnd < m_CeilingClipHeight[iXCurrent] + 1)
	{
		CurrentCeilingEnd = m_CeilingClipHeight[iXCurrent] + 1;
	}

	if (CurrentFloorStart >= m_FloorClipHeight[iXCurrent])
	{
		CurrentFloorStart = m_FloorClipHeight[iXCurrent] - 1;
	}

	if (CurrentCeilingEnd > CurrentFloorStart)
	{
		renderdata.CeilingEnd += renderdata.CeilingStep;
		renderdata.FloorStart += renderdata.FloorStep;
		++iXCurrent;
		return false;
	}
	return true;
}

//Pasa de un ángulo (de un vértice o punto) a un píxel en la pantalla final
//	teniendo en cuenta que la resolución original de DOOM es de 320x200
int Renderer::AngleToScreen(Angle angle)
{
	const float playerFOV = m_pPlayer->getFOV();
	
	int i_x = 0;	//Pixel de la pantalla a lo ancho

	//Si estamos en el lado izquierdo
	if (angle > playerFOV)
	{
		angle -= playerFOV;
		//i_x = m_halfRenderXSize - round(tanf(angle.GetValue() * (float)M_PI / 180.0f) * m_halfRenderXSize);
		i_x = m_iDistancePlayerToScreen - round(angle.getTan() * m_halfRenderXSize);
	}
	else //Lado derecho
	{
		angle = playerFOV - angle.GetValue();
		//i_x = m_halfRenderXSize + round(tanf(angle.GetValue() * (float)M_PI / 180.0f) * m_halfRenderXSize);
		i_x = round(angle.getTan() * m_halfRenderXSize);
		i_x += m_iDistancePlayerToScreen;
	}
	return i_x;
}

float Renderer::GetScaleFactor(int VXScreen, Angle SegToNormalAngle, float DistanceToNormal)
{
	static float MAX_SCALEFACTOR = 64.0f;
	static float MIN_SCALEFACTOR = 0.00390625f;

	Angle Angle90(90);

	Angle ScreenXAngle = m_ScreenXToAngle[VXScreen];
	Angle SkewAngle = m_ScreenXToAngle[VXScreen] + m_pPlayer->GetAngle() - SegToNormalAngle;

	float ScreenXAngleCos = ScreenXAngle.getCos();
	float SkewAngleCos = SkewAngle.getCos();
	float ScaleFactor = (m_iDistancePlayerToScreen * SkewAngleCos) / (DistanceToNormal * ScreenXAngleCos);

	if (ScaleFactor > MAX_SCALEFACTOR)
	{
		ScaleFactor = MAX_SCALEFACTOR;
	}
	else if (MIN_SCALEFACTOR > ScaleFactor)
	{
		ScaleFactor = MIN_SCALEFACTOR;
	}

	return ScaleFactor;
}

//Creo que ya no la uso para nada
void Renderer::PartialSeg(Seg& seg, Angle& V1Angle, Angle& V2Angle, float& DistanceToV, bool IsLeftSide)
{
	float SideC = sqrt(powf(seg.vert1->x - seg.vert2->x, 2) + powf(seg.vert1->y - seg.vert2->y, 2));
	Angle V1toV2Span = V1Angle - V2Angle;
	float SINEAngleB = DistanceToV * V1toV2Span.getSin() / SideC;
	Angle AngleB(asinf(SINEAngleB) * 180.0f / (float)M_PI);
	Angle AngleA(180.0f - V1toV2Span.GetValue() - AngleB.GetValue());

	Angle AngleVToFOV;
	if (IsLeftSide)
	{
		AngleVToFOV = V1Angle - (m_pPlayer->GetAngle() + 45);
	}
	else
	{
		AngleVToFOV = (m_pPlayer->GetAngle() - 45) - V2Angle;
	}

	Angle NewAngleB(180 - AngleVToFOV.GetValue() - AngleA.GetValue());
	DistanceToV = DistanceToV * AngleA.getSin() / NewAngleB.getSin();
}

void Renderer::CeilingFloorUpdate(SegRenderData& render_data)
{
	// no entiendo mucho qué estoy haciendo pero necesito renderizar mapas cuanto antes AAAAAAA
	if (!render_data.pSeg->pLeftSector)
	{
		render_data.UpdateFloor = true;
		render_data.UpdateCeiling = true;
		return;
	}

	if (render_data.LSecCeiling != render_data.RSecCeiling)
	{
		render_data.UpdateCeiling = true;
	}
	else
	{
		render_data.UpdateCeiling = false;
	}

	if (render_data.LSecFloor != render_data.RSecFloor)
	{
		render_data.UpdateFloor = true;
	}
	else
	{
		render_data.UpdateFloor = false;
	}
	

	if (render_data.pSeg->pLeftSector->CeilingHeight <= render_data.pSeg->pRightSector->FloorHeight ||
		render_data.pSeg->pLeftSector->FloorHeight >= render_data.pSeg->pRightSector->CeilingHeight)
	{
		//Si es una puerta cerrada (?) magia de doom i guess
		render_data.UpdateCeiling = true;
		render_data.UpdateFloor = true;
	}

	if (render_data.pSeg->pRightSector->CeilingHeight <= m_pPlayer->GetZPos())
	{
		//Un techo por debajo del plano de vista de doomguy
		render_data.UpdateCeiling = false;
	}

	if (render_data.pSeg->pRightSector->FloorHeight >= m_pPlayer->GetZPos())
	{
		//Un suelo por encima del plano de vista de doomguy
		render_data.UpdateFloor = false;
	}
}

void Renderer::CeilingFloorHeight(Seg& seg, int& VXScreen, float& DistToVertex, float& CeilingVScreen, float& FloorVScreen)
{
	//Longitudes de los lados del triángulo
	float ceiling = seg.pRightSector->CeilingHeight - m_pPlayer->GetZPos();
	float floor = seg.pRightSector->FloorHeight - m_pPlayer->GetZPos();

	//Como sabemos el píxel de pantalla que ocupa el vértice, sabemos el ángulo que forma con Doomguy
	//		Solo lo miramos en la tabla de lookup
	Angle VScreenAngle = m_ScreenXToAngle[VXScreen];

	//Para evitar efecto ojo de pez dividimos por el coseno del ángulo que forman la mirada hacia adelante    V
	//		de Doomguy y el vértice, que seguramente no estará delante D ----- V, sino que será D ------------
	float distToVScreen = m_iDistancePlayerToScreen / VScreenAngle.getCos();

	CeilingVScreen = (abs(ceiling) * distToVScreen) / DistToVertex;	//Trigonometría, triángulos similares
	FloorVScreen = (abs(floor) * distToVScreen) / DistToVertex;		// con ello sacamos la altura de la pared en pantalla

	//Si está en la mitad superior o en la inferior de la pantalla
	if (ceiling > 0)
	{
		CeilingVScreen = m_halfRenderYSize - CeilingVScreen;
	}
	else
	{
		CeilingVScreen += m_halfRenderYSize;
	}

	//Si está en la mitad superior o en la inferior de la pantalla
	if (floor > 0)
	{
		FloorVScreen = m_halfRenderYSize - FloorVScreen;
	}
	else
	{
		FloorVScreen += m_halfRenderYSize;
	}
}

