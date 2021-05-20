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
#include <SFML/Graphics.hpp>
#include "../Misc/Geometry.h"
#include <corecrt_math_defines.h>
#include "../PatchesTextures/Texture.h"
#include "../PatchesTextures/AssetsManager.h"

Renderer::Renderer(sf::RenderWindow* r_window) : m_pMap(NULL), m_pPlayer(NULL)
{
	automapScaleFactor = 2.55f * (2560.0f / SCREENWIDTH);	//Este valor da resultados decentes al hacer automap, pero está hecho a mano -->TODO
	m_pRenderWindow = r_window;
}

Renderer::~Renderer()
{
}

void Renderer::Init(Map* pMap, Player* pPlayer, DisplayManager* dm, std::vector<Soldier*> enemyList_)
{
	m_pDisplayManager = dm;
	m_pMap = pMap;
	m_pPlayer = pPlayer;
	m_pixels = new uint8_t[SCREENWIDTH * SCREENHEIGHT * 4];
	m_texture = sf::Texture();
	m_texture.create(SCREENWIDTH, SCREENHEIGHT);

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

	enemyList = enemyList_;
}

void Renderer::Render(bool automap, const float& deltaTime)
{
	m_pRenderWindow->clear(sf::Color::Black);
	if(automap)
	{
		RenderAutoMap();
	}
	else
	{
		Render3dView();											//Pinta el nivel en 3d
		m_pPlayer->renderPlayer(m_pRenderWindow, deltaTime);	//Pinta la escopeta
		m_pHud->drawHud();										//Pinta el HUD del jugador
	}
}

//Permite dibujar una pared en pantalla
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
	memset(m_pixels, 0, SCREENWIDTH * SCREENHEIGHT * 4);
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

}

void Renderer::RenderAutoMap()
{
	AutomapPlayer();
	AutomapWalls();
	AutomapEnemy();
}

bool compareDistEnemy(Soldier* a, Soldier* b) {
	return (a->getDistToPlayer() > b->getDistToPlayer());
}

void Renderer::Render3dView()
{
	//m_texture.create(SCREENWIDTH, SCREENHEIGHT);
	RenderBSPNodes();
	m_texture.update(m_pixels);
	m_pRenderWindow->draw(sf::Sprite(m_texture));

	std::sort(enemyList.begin(), enemyList.end(), compareDistEnemy);
	//mover a una función de renderizado de enemigos?
	for (auto a : enemyList) {
		Vertex v;
		v.x = a->xValue();
		v.y = a->yValue();
		Angle a1, a1fromPlayer;		//para invocar a clipvertexesinFOV()
		if (a->getVisible()) {		//Si el enemigo es visible
			if (m_pPlayer->ClipOneVertexInFOV(v, a1, a1fromPlayer)) {
				//std::cout << a1fromPlayer.GetValue() << std::endl;
				a->renderEnemy(a1fromPlayer.GetValue(), m_pRenderWindow);
			}
		}
	}
}

void Renderer::AutomapPlayer()
{
	int w = m_pRenderWindow->getView().getSize().x;
	int h = m_pRenderWindow->getView().getSize().y;
	float x_pos, y_pos;

	RecalculateAutomapInScreen(m_pPlayer->GetXPos(), x_pos, m_pPlayer->GetYPos(), y_pos);
	sf::CircleShape player_triangle(3.0f, 3);
	player_triangle.setFillColor(sf::Color::Green);
	sf::Vector2f v = sf::Vector2f(x_pos - 0.5f, y_pos - 0.5f);
	player_triangle.setPosition(v);
	m_pRenderWindow->draw(player_triangle);

	//std::cout << "Print player on " << x_pos << " " << y_pos << std::endl;
}

void Renderer::AutomapEnemy()
{
	for (Soldier* a : enemyList) {
		if (!a->isReallyDead()) {
			int w = m_pRenderWindow->getView().getSize().x;
			int h = m_pRenderWindow->getView().getSize().y;

			float x_pos, y_pos;
			RecalculateAutomapInScreen(a->xValue(), x_pos, a->yValue(), y_pos);
			sf::CircleShape enemy_triangle(3.0f, 3);


			enemy_triangle.setFillColor(sf::Color::Red);

			//enemy_triangle.setFillColor(sf::Color::Red);
			sf::Vector2f v = sf::Vector2f(x_pos, y_pos);

			enemy_triangle.setPosition(v);
			m_pRenderWindow->draw(enemy_triangle);

			//std::cout << "Print enemy on " << x_pos << " " << y_pos << std::endl;
		}

	}
	//std::cout << "----------------------" << std::endl;
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
	RenderBSPNodes(m_pMap->getNodesSize() - 1, 0);	
}

void Renderer::RenderBSPNodes(int16_t nodeID, int i)
{
	//Comprobar con la máscara si es un nodo hoja == subsector (el que buscamos)
	if ((int16_t)(nodeID & SUBSECTORIDENTIFIER))
	{		
		//std::cout << "i = " << i << std::endl;
		RenderSubsector((int16_t)(nodeID & (~SUBSECTORIDENTIFIER)));	//hay que volver a hacer el casteo porque si no saca -326XX en vez del ID que debería y explota

		return;
	}

	if (IsPointOnLeftSide(m_pPlayer->GetXPos(), m_pPlayer->GetYPos(), nodeID))
	{
		RenderBSPNodes(m_pMap->getNode(nodeID).LeftChild, i+1);
		RenderBSPNodes(m_pMap->getNode(nodeID).RightChild, i + 1);
		//izquierda y luego derecha, de cerca a lejos (creo que es por eso)
	}
	else
	{
		RenderBSPNodes(m_pMap->getNode(nodeID).RightChild, i + 1);
		RenderBSPNodes(m_pMap->getNode(nodeID).LeftChild, i + 1);
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
	m_pPlayer->setCurrentSubsector(subsectorID);		//Le dice al jugador en qué subsector está
	Subsector &subsector = m_pMap->getSSec(subsectorID);

	for (int i = 0; i < subsector.seg_count; i++)
	{
		Seg &seg = m_pMap->getSeg(subsector.first_segID + i);
		Vertex v = *(seg.vert1);
		Vertex v2 = *(seg.vert2);
		Angle a1, a2, a1fromPlayer, a2fromPlayer;

		if (m_pPlayer->ClipVertexesInFOV(v, v2, a1, a2, a1fromPlayer, a2fromPlayer))
		{
			AddWallInFOV(seg, a1, a2, a1fromPlayer, a2fromPlayer);
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

	if (current.last <= FoundClipWall->last)	//Si empieza y termina "dentro" de lo que ya había
		return;									// no hay que hacer nada

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

sf::Color Renderer::GetCeilingColor(std::string texName)
{
	AssetsManager* am = AssetsManager::getInstance();
	return  m_pDisplayManager->getCurrentPalette().Colors[am->getFlatColor(texName)];
}

sf::Color Renderer::GetWallRenderColor(std::string textName, Texture* walltex)
{
	if (m_WallColor.count(textName))
	{
		return m_WallColor[textName];
	}
	else
	{
		//Texture* tex = AssetsManager::getInstance()->getTexture(textName);
		bool kk = false;
		//No son textures, son flats, funcionan diferente y van a doler si los terminamos metiendo
		sf::Color color = m_pDisplayManager->getCurrentPalette().Colors[walltex->getTexel(rand() % walltex->getWidth(), rand() % walltex->getHeight(), kk)];	//tex->getTexel(tex->getWidth() / 2, tex->getHeight() / 2, kk)
		m_WallColor[textName] = color;
		return color;
	}
	return sf::Color();
}

void Renderer::ClipSolidWallsVertical(Seg& seg, int VertX1, int VertX2, Angle AngleV1, Angle AngleV2)
{
	SegRenderData renderdata {0};

	renderdata.pSeg = &seg;
	renderdata.VertX1OnScreen = VertX1;
	renderdata.VertX2OnScreen = VertX2;
	renderdata.AngleV1 = AngleV1;
	renderdata.AngleV2 = AngleV2;
	renderdata.DistToV1 = m_pPlayer->distanceToEdge(*seg.vert1);
	renderdata.DistToV2 = m_pPlayer->distanceToEdge(*seg.vert2);

	Angle Angle90(90);
	Angle SegToNormalAngle = seg.angle + Angle90;								//rwNormalAngle
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

	//AL RICO INVENT
	
	int vtop = 0, rw_midtexturemid = 0;
	Texture* tex = AssetsManager::getInstance()->getTexture(renderdata.pSeg->pLinedef->sidedef_r->MiddleTexture);
	Texture* texTop = AssetsManager::getInstance()->getTexture(renderdata.pSeg->pLinedef->sidedef_r->UpperTexture);
	Texture* texBot = AssetsManager::getInstance()->getTexture(renderdata.pSeg->pLinedef->sidedef_r->LowerTexture);
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
	else
	{
		if ((renderdata.pSeg->pLinedef->flags & eDONTPEGBOTTOM) != 0)
		{
			vtop = renderdata.pSeg->pRightSector->FloorHeight + tex->getHeight();
			rw_midtexturemid = vtop;
		}
		else
		{
			rw_midtexturemid = renderdata.RSecCeiling;
		}
		rw_midtexturemid += renderdata.pSeg->pLinedef->sidedef_r->YOffset + m_pPlayer->GetZPos();
	}
	
	//Por fin tenemos todos los datos para renderizar un segmento
	//R_RenderSegLoop
	RenderSegment(renderdata, rw_midtexturemid, tex, texTop, texBot);
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

void Renderer::DrawUpperSectionV2(SegRenderData& renderdata, int iXCurrent, int CurrentCeilingEnd, int CurrentFloorStart, Texture* tex, int u, int dc_texturemid, int oldCeilingEnd, int oldFloorStart)
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
			for (int px = CurrentCeilingEnd; px < iUpperHeight; px++)
			{
				float t = ((float)(px - oldCeilingEnd) / (float)(iUpperHeight - oldCeilingEnd));
				float frac = t * (renderdata.pSeg->pRightSector->CeilingHeight - renderdata.pSeg->pLeftSector->CeilingHeight);

				bool transp = false;
				sf::Color color;
				if (transp)
				{
					return;
				}
				else
				{
					color = sf::Color(m_pDisplayManager->getCurrentPalette().Colors[tex->getTexel(u & (tex->getWidth() - 1), (int)frac & 127, transp)]);	//El 127 es cosa de doom, no preguntes
				}
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 0] = color.r;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 1] = color.g;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 2] = color.b;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 3] = color.a;
			}
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
	/*
	sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, CurrentCeilingEnd), color),
			sf::Vertex(sf::Vector2f(iXCurrent, CurrentFloorStart), color)
	};
	m_pRenderWindow->draw(line, 2, sf::Lines);
	*/
	m_CeilingClipHeight[iXCurrent] = renderYSize;
	m_FloorClipHeight[iXCurrent] = -1;
}

void Renderer::DrawMidSectionV2(SegRenderData& renderdata, int iXCurrent, int CurrentCeilingEnd, int CurrentFloorStart, Texture* tex, int u, int dc_texturemid, int oldCeilingEnd, int oldFloorStart)
{
	for (int px = CurrentCeilingEnd; px < CurrentFloorStart; px++)
	{
		float t = ((float)(px - oldCeilingEnd) / (float)(oldFloorStart - oldCeilingEnd));
		float frac = t * (dc_texturemid - renderdata.pSeg->pRightSector->FloorHeight);

		bool transp = false;
		sf::Color color;
		if (transp)
		{
			return;
		}
		else
		{
			color = sf::Color(m_pDisplayManager->getCurrentPalette().Colors[tex->getTexel(u & (tex->getWidth() - 1), (int)frac & 127, transp)]);	//El 127 es cosa de doom, no preguntes
		}
		m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 0] = color.r;
		m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 1] = color.g;
		m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 2] = color.b;
		m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 3] = color.a;
	}
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

void Renderer::DrawLowerSectionV2(SegRenderData& renderdata, int iXCurrent, int CurrentCeilingEnd, int CurrentFloorStart, Texture* tex, int u, int dc_texturemid, int oldCeilingEnd, int oldFloorStart)
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
			for (int px = iLowerHeight; px < CurrentFloorStart; px++)
			{
				float t = ((float)(px - iLowerHeight) / (float)(oldFloorStart - iLowerHeight));
				float frac = t * (renderdata.pSeg->pLeftSector->FloorHeight - renderdata.pSeg->pRightSector->FloorHeight);

				bool transp = false;
				sf::Color color;
				if (transp)
				{
					return;
				}
				else
				{
					color = sf::Color(m_pDisplayManager->getCurrentPalette().Colors[tex->getTexel(u & (tex->getWidth() - 1), (int)frac & 127, transp)]);	//El 127 es cosa de doom, no preguntes
				}
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 0] = color.r;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 1] = color.g;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 2] = color.b;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 3] = color.a;

				m_FloorClipHeight[iXCurrent] = iLowerHeight;
			}
		}
		else
			m_FloorClipHeight[iXCurrent] = CurrentFloorStart + 1;
	}
	else if (renderdata.UpdateFloor)
	{
		m_FloorClipHeight[iXCurrent] = CurrentFloorStart + 1;
	}
}

void Renderer::RenderSegment(SegRenderData& renderdata, int rw_midtexturemid, Texture* texMid, Texture* texTop, Texture* texBot)
{
	int iXCurrent = renderdata.VertX1OnScreen;

	float wallWidth = dist2Points(renderdata.pSeg->vert1->x, renderdata.pSeg->vert1->y, renderdata.pSeg->vert2->x, renderdata.pSeg->vert2->y);
	float x_wall, y_wall, x_aux, y_aux, t;
	float z0 = m_pPlayer->distanceToEdge(*renderdata.pSeg->vert1);
	float z1 = m_pPlayer->distanceToEdge(*renderdata.pSeg->vert2);

	while (iXCurrent <= renderdata.VertX2OnScreen)
	{
		int currentCeilingEnd = renderdata.CeilingEnd;
		int currentFloorStart = renderdata.FloorStart;
		int oldCeilingEnding, oldFloorStart;

		if (!ValidateRange(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, oldCeilingEnding, oldFloorStart))	//Solo hacemos cosas si el rango es válido
		{
			continue;
		}

		// ******************** PATATA ENVENENADA ************************//
		//Sección del medio, se interpola la u de la textura
		x_aux = m_pPlayer->GetXPos() + (m_ScreenXToAngle[iXCurrent] + m_pPlayer->GetAngle()).getCos() * 1;
		y_aux = m_pPlayer->GetYPos() + (m_ScreenXToAngle[iXCurrent] + m_pPlayer->GetAngle()).getSin() * 1;
		intersect(renderdata.pSeg->vert1->x, renderdata.pSeg->vert1->y, renderdata.pSeg->vert2->x, renderdata.pSeg->vert2->y, m_pPlayer->GetXPos(), m_pPlayer->GetYPos(), x_aux, y_aux, x_wall, y_wall);
		t = dist2Points(renderdata.pSeg->vert2->x, renderdata.pSeg->vert2->y, x_wall, y_wall) / wallWidth;
		//El 0 es porque la textura empieza en coord x 0
		float u_alpha = ((1.0 - t) * (0 / z0) + (((float)t * wallWidth) / z1)) / (((1.0 - t) * 1.0f / z0) + (t * 1.0f / z1));
		// ******************** PATATA ENVENENADA ************************//

		sf::Color color_;

		if (renderdata.pSeg->pLeftSector)
		{
			//pintar arriba y abajo
			//techo
			sf::Color color_;
			color_ = GetCeilingColor(renderdata.pSeg->pRightSector->CeilingTexture);
			/*if (texMid != nullptr)
			{
				color_ = GetWallRenderColor(renderdata.pSeg->pRightSector->CeilingTexture, texMid);
			}
			else if (texBot != nullptr)
			{
				color_ = GetWallRenderColor(renderdata.pSeg->pRightSector->CeilingTexture, texBot);
			}
			else
			{
				color_ = GetWallRenderColor(renderdata.pSeg->pRightSector->CeilingTexture, texTop);
			}*/
			for (int px = m_CeilingClipHeight[iXCurrent] + 1; px < currentCeilingEnd; ++px)
			{
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 0] = color_.r;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 1] = color_.g;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 2] = color_.b;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 3] = 255;
			}
			/*sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, currentCeilingEnd), color3),
			sf::Vertex(sf::Vector2f(iXCurrent, m_CeilingClipHeight[iXCurrent]), color3)
			};
			m_pRenderWindow->draw(line, 2, sf::Lines);*/
			//DrawUpperSection(renderdata, iXCurrent, currentCeilingEnd, color);
			if (texTop != nullptr)
			{
				DrawUpperSectionV2(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, texTop, (int)u_alpha, rw_midtexturemid, oldCeilingEnding, oldFloorStart);
			}

			//Suelo
			sf::Color color2;
			color2 = GetCeilingColor(renderdata.pSeg->pRightSector->FloorTexture);
			/*if (texMid != nullptr)
			{
				color2 = GetWallRenderColor(renderdata.pSeg->pRightSector->FloorTexture, texMid);
			}
			else if (texBot != nullptr)
			{
				color2 = GetWallRenderColor(renderdata.pSeg->pRightSector->FloorTexture, texBot);
			}
			else
			{
				color2 = GetWallRenderColor(renderdata.pSeg->pRightSector->FloorTexture, texTop);
			}*/
			for (int px = currentFloorStart; px < m_FloorClipHeight[iXCurrent]; px++)
			{
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 0] = color2.r;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 1] = color2.g;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 2] = color2.b;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 3] = 255;
			}
			/*sf::Vertex line2[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, currentFloorStart), color2),
			sf::Vertex(sf::Vector2f(iXCurrent, m_FloorClipHeight[iXCurrent]), color2)
			};
			m_pRenderWindow->draw(line2, 2, sf::Lines);*/
			//DrawLowerSection(renderdata, iXCurrent, currentFloorStart, color);
			if (texBot != nullptr)
			{
				DrawLowerSectionV2(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, texBot, (int)u_alpha, rw_midtexturemid, oldCeilingEnding, oldFloorStart);
			}
		}
		else
		{
			//pintar el medio de todo
			//techo
			//std::cout << "currentCeilingEnd: " << currentCeilingEnd << std::endl;
			//std::cout << "m_CeilingClipHeight[iXCurrent]: " << m_CeilingClipHeight[iXCurrent] << std::endl;
			//std::cout << "Color: " << (int)color_.r << " " << (int)color_.g << " " << (int)color_.b << std::endl;


			//color_ = GetWallRenderColor(renderdata.pSeg->pRightSector->CeilingTexture, texMid);

			color_ = GetCeilingColor(renderdata.pSeg->pRightSector->CeilingTexture);
			for (int px = m_CeilingClipHeight[iXCurrent] + 1; px < currentCeilingEnd; ++px)
			{
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 0] = color_.r;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 1] = color_.g;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 2] = color_.b;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 3] = 255;
			}

			/*sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, currentCeilingEnd), color3),
			sf::Vertex(sf::Vector2f(iXCurrent, m_CeilingClipHeight[iXCurrent]), color3)
			};
			m_pRenderWindow->draw(line, 2, sf::Lines);*/

			//Suelo
			
			//sf::Color color2 = GetWallRenderColor(renderdata.pSeg->pRightSector->FloorTexture, texMid);

			sf::Color color2 = GetCeilingColor(renderdata.pSeg->pRightSector->FloorTexture);
			for (int px = currentFloorStart; px < m_FloorClipHeight[iXCurrent]; px++)
			{
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 0] = color2.r;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 1] = color2.g;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 2] = color2.b;
				m_pixels[SCREENWIDTH * (px) * 4 + (iXCurrent) * 4 + 3] = 255;
			}
			/*sf::Vertex line2[] = {
			sf::Vertex(sf::Vector2f(iXCurrent, currentFloorStart), color2),
			sf::Vertex(sf::Vector2f(iXCurrent, m_FloorClipHeight[iXCurrent]), color2)
			};
			m_pRenderWindow->draw(line2, 2, sf::Lines);*/
			if (texMid != nullptr)
			{
				DrawMidSectionV2(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, texMid, (int)u_alpha, rw_midtexturemid, oldCeilingEnding, oldFloorStart);
			}
		}
		++iXCurrent;
		renderdata.CeilingEnd += renderdata.CeilingStep;
		renderdata.FloorStart += renderdata.FloorStep;
	}
}


bool Renderer::ValidateRange(SegRenderData& renderdata, int& iXCurrent, int& CurrentCeilingEnd, int& CurrentFloorStart, int& oldCeilingEnd, int& oldFloorStart)
{
	oldCeilingEnd = CurrentCeilingEnd;
	oldFloorStart = CurrentFloorStart;
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
	if (angle > 90)
	{
		angle -= 90;
		i_x = m_iDistancePlayerToScreen - round(tanf(angle.GetValue() * M_PI / 180.0f) * m_halfRenderXSize);
	}
	else //Lado derecho
	{
		angle = 90 - angle.GetValue();
		i_x = round(tanf(angle.GetValue() * M_PI / 180.0f) * m_halfRenderXSize);
		i_x += m_iDistancePlayerToScreen;
	}
	return i_x;
}

float Renderer::GetScaleFactor(int VXScreen, Angle SegToNormalAngle, float DistanceToNormal)
{
	//Cosas de DOOM que no entenderé en la vida
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

