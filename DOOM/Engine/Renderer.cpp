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
#include "../PatchesTextures/Texture.h"
#include "../PatchesTextures/AssetsManager.h"

Renderer::Renderer(sf::RenderWindow* r_window) : m_pMap(NULL), m_pPlayer(NULL)
{
	automapScaleFactor = 2.0f * (2560.0f / SCREENWIDTH);	//Este valor da resultados decentes al hacer automap
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
	AutomapEnemy();
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

	//std::cout << "Print player on " << x_pos << " " << y_pos << std::endl;
}

void Renderer::AutomapEnemy()
{
	for (Soldier* a : enemyList) {
		int w = m_pRenderWindow->getView().getSize().x;
		int h = m_pRenderWindow->getView().getSize().y;

		float x_pos, y_pos;
		RecalculateAutomapInScreen(a->xValue(), x_pos, a->yValue(), y_pos);
		sf::CircleShape enemy_triangle(5.0f, 4);
		enemy_triangle.setFillColor(sf::Color::Red);
		sf::Vector2f v = sf::Vector2f(x_pos, y_pos);

		enemy_triangle.setPosition(v);
		m_pRenderWindow->draw(enemy_triangle);

		//std::cout << "Print enemy on " << x_pos << " " << y_pos << std::endl;
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
	//ClipSolidWallsVertical(seg, VertX1, VertX2, a1, a2);

	if (seg.pLeftSector != nullptr)
	{
		ClipSolidWallsVertical(seg, VertX1, VertX2, a1, a2);
	}
	else
	{
		/*
		Linedef* line = seg.pLinedef;
		Sidedef* side = line->sidedef_r;
		Sector* frontSector = seg.pRightSector;

		float worldFrontZ1 = frontSector->CeilingHeight - m_pPlayer->GetZPos();
		float worldFrontZ2 = frontSector->FloorHeight - m_pPlayer->GetZPos();

		Texture* wallTexture = AssetsManager::getInstance()->getTexture(side->MiddleTexture);
		unsigned int wallWidthMask = wallTexture->getWidth() - 1;
		float middleTextureAlt;
		if ((line->flags & eDONTPEGBOTTOM) != 0)
		{
			float vTop = (float)frontSector->FloorHeight + (float)wallTexture->getHeight();
			middleTextureAlt = vTop - m_pPlayer->GetZPos();
		}
		else
		{
			middleTextureAlt = worldFrontZ1;
		}
		middleTextureAlt += side->YOffset;	//Probar también con XOffset

		// Calcular factor de escala para los vértices izquierdo y derecho de las paredes
		Angle rwNormalAngle = seg.angle + Angle(90.0f);
		Angle offsetAngle = rwNormalAngle - m_pPlayer->AngleToVertex(*seg.vert1);		//a1 = rwAngle1???? no creo, probar con m_pPlayer->AngleToVertex(*seg->vert1)
		if (offsetAngle > Angle(90.0f))
		{
			offsetAngle = Angle(90.0f);
		}
		Angle distAngle = Angle(90.0f) - offsetAngle;
		float hypotenuse = m_pPlayer->distanceToEdge(*seg.vert1);
		float rwDistance = hypotenuse * distAngle.getSin();
		float rwScale = GetScaleFactor(VertX1, rwNormalAngle, rwDistance);
		float scale1 = rwScale;
		float scale2, rwScaleStep;
		if (VertX2 > VertX1)
		{
			scale2 = GetScaleFactor(VertX2, rwNormalAngle, rwDistance);
			rwScaleStep = (scale2 - rwScale) / (VertX2 - VertX1);
		}
		else
		{
			scale2 = scale1;
			rwScaleStep = 0.0f;
		}

		//Determinar alineación horizontal de las texturas
		Angle textureOffsetAngle = rwNormalAngle - m_pPlayer->AngleToVertex(*seg.vert1);	//a1 = rwAngle1???? no creo, probar con m_pPlayer->AngleToVertex(*renderdata.pSeg->vert1)
		if (textureOffsetAngle > Angle(180.0f))
		{
			textureOffsetAngle = -textureOffsetAngle;
		}
		if (textureOffsetAngle > Angle(90.0f))
		{
			textureOffsetAngle = Angle(90.0f);
		}

		float rwOffset = hypotenuse * textureOffsetAngle.getSin();
		if (rwNormalAngle - m_pPlayer->AngleToVertex(*seg.vert1) < Angle(180.0f))		//a1 = rwAngle1???? no creo, probar con m_pPlayer->AngleToVertex(*renderdata.pSeg->vert1)
		{
			rwOffset = -rwOffset;
		}
		rwOffset += seg.offset + side->XOffset;
		Angle rwCenterAngle = Angle(90.0f) + m_pPlayer->GetAngle() - rwNormalAngle;

		//Determinar Y1, Y2 de pantalla donde dibujar
		float wallY1Frac = m_halfRenderYSize - worldFrontZ1 * rwScale;
		float wallY1Step = -(rwScaleStep * worldFrontZ1);
		float wallY2Frac = m_halfRenderYSize - worldFrontZ2 * rwScale;
		float wallY2Step = -(rwScaleStep * worldFrontZ2);

		// . . . ya lo haré luego xd

		//Renderizar
		for (int x = VertX1; x <= VertX2; x++)
		{
			int drawWallY1 = (int)wallY1Frac;
			int drawWallY2 = (int)wallY2Frac;
			//if drawceiling . . . 

			//if drawwall
			if (true)
			{
				int wy1 = std::max(drawWallY1, m_CeilingClipHeight[x] + 1);
				int wy2 = std::min(drawWallY2, m_FloorClipHeight[x] - 1);

				Angle angle = rwCenterAngle + m_ScreenXToAngle[x];
				angle = Angle((int)angle.GetValue() & 0x7FFFFFFF);

				float texturecolumn = floor((rwOffset - angle.getTan() * rwDistance));
				uint8_t* source = wallTexture->getColumn((unsigned int)texturecolumn & wallWidthMask);
				float invScale = (int)(0xffffffffu / (unsigned int)(rwScale + 1));
				//DrawColumn(source[0], x, wy1, wy2, invScale, middleTextureAlt);

				if (wy2 - wy1 < 0)
					continue;
				//pos1 y pos2

				float fracStep = invScale;
				float frac = middleTextureAlt + (wy1 - m_halfRenderYSize) * fracStep;

				for (int px = wy1; px < wy2; px++)
				{
					bool booleano;
					sf::Color color = sf::Color(m_pDisplayManager->getCurrentPalette().Colors[source[(int)frac & 127]]); // & 127?
					sf::Vertex point(sf::Vector2f(x, px), color);
					m_pRenderWindow->draw(&point, 1, sf::Points);
					frac += fracStep;
				}
				//m_CeilingClipHeight[x] = renderYSize;
				//m_FloorClipHeight[x] = -1;
			}
			rwScale += rwScaleStep;
			wallY1Frac += wallY1Step;
			wallY2Frac += wallY2Step;
		}*/
		ClipSolidWallsVertical(seg, VertX1, VertX2, a1, a2);
	}

}

sf::Color Renderer::GetWallRenderColor(std::string textName)
{
	if (m_WallColor.count(textName))
	{
		return m_WallColor[textName];
	}
	else
	{
		sf::Color color = m_pDisplayManager->getCurrentPalette().Colors[rand() % 256];
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
	renderdata.DistToV2 = m_pPlayer->distanceToEdge(*seg.vert2);

	Angle Angle90(90);
	Angle SegToNormalAngle = seg.angle + Angle90;								//rwNormalAngle
	Angle NomalToV1Angle = SegToNormalAngle.GetValue() - AngleV1.GetValue();	//
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
	
	int vtop = 0, rw_midtexturemid = 0, rw_offset = 0;
	Angle rw_centerangle = Angle(0.0f);
	Texture* tex = AssetsManager::getInstance()->getTexture(renderdata.pSeg->pLinedef->sidedef_r->MiddleTexture);
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
		rw_midtexturemid += renderdata.pSeg->offset + m_pPlayer->GetZPos();
		Angle offsetangle = SegToNormalAngle - m_pPlayer->AngleToVertex(*renderdata.pSeg->vert1); // rwnormalangle - rwAngle1
		if (offsetangle > Angle(180.0f))
		{
			offsetangle = -offsetangle;
		}
		if (offsetangle > 90.0f)
		{
			offsetangle = Angle(90.0f);
		}

		rw_offset = renderdata.DistToV1 * offsetangle.getSin();
		if ((SegToNormalAngle - (SegToNormalAngle - renderdata.AngleV1)) < Angle(180.0f))
		{
			rw_offset = -rw_offset;
		}
		rw_offset = rw_offset + renderdata.pSeg->pLinedef->sidedef_r->XOffset + renderdata.pSeg->offset;
		rw_centerangle = Angle(90) + m_pPlayer->GetAngle() - SegToNormalAngle;
	}

	//Por fin tenemos todos los datos para renderizar un segmento
	//R_RenderSegLoop
	
	RenderSegment(renderdata, rw_offset, rw_centerangle, rw_midtexturemid, tex); // vtop?
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
	float frac, fracstep;
	
	//fracstep = 1.0f / renderdata.V1ScaleFactor; 
	//frac = dc_texturemid - m_pPlayer->GetZPos() + (CurrentCeilingEnd - m_halfRenderYSize) * fracstep;

	for (int px = CurrentCeilingEnd; px < CurrentFloorStart; px++)
	{
		float t = ((float)(px - oldCeilingEnd) / (float)(oldFloorStart - oldCeilingEnd));
		float frac = t * (renderdata.pSeg->pRightSector->CeilingHeight - renderdata.pSeg->pRightSector->FloorHeight);

		bool transp = false;
		sf::Color color;
		if (transp)
		{
			return;
		}
		else
		{
			color = sf::Color(m_pDisplayManager->getCurrentPalette().Colors[tex->getTexel(u & (tex->getWidth() - 1), (int)frac & 127, transp)]);
		}
		sf::Vertex point(sf::Vector2f(iXCurrent, px), color);
		m_pRenderWindow->draw(&point, 1, sf::Points);
		//frac += fracstep;
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

void Renderer::RenderSegment(SegRenderData& renderdata, float rw_offset, Angle rw_centerangle, int rw_midtexturemid, Texture* tex) // int vtop?
{
	sf::Color color;
	color = SelectColor(*(renderdata.pSeg));
	int iXCurrent = renderdata.VertX1OnScreen;
	Angle angle;
	int texturecolumn;
	while (iXCurrent <= renderdata.VertX2OnScreen)
	{
		int currentCeilingEnd = renderdata.CeilingEnd;
		int currentFloorStart = renderdata.FloorStart;
		int oldCeilingEnding, oldFloorStart;

		if (!ValidateRange(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, oldCeilingEnding, oldFloorStart))	//Solo hacemos cosas si el rango es válido
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

			//INVENT INSIDE (medio old)
			angle = Angle(rw_centerangle.GetValue() + m_ScreenXToAngle[iXCurrent].GetValue());
			texturecolumn = (int)(rw_offset + ((float)angle.getTan() * renderdata.DistToNormal));
			//int dc_yl = currentCeilingEnd;
			//int dc_yh = currentFloorStart;
			//int dc_texturemid = rw_midtexturemid;
			//DrawMidSectionV2(renderdata, iXCurrent, std::max(currentCeilingEnd, m_CeilingClipHeight[iXCurrent]), std::min(currentFloorStart, m_FloorClipHeight[iXCurrent]), tex, texturecolumn, dc_texturemid);
			//DrawMidSection(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, color);

			
			//OLD
			
			//Angle test11, test12, test21, t est22;
			//m_pPlayer->ClipVertexesInFOV(*(renderdata.pSeg->vert1), *(renderdata.pSeg->vert2), test11, test12, test21, test22);
			Angle test1 = m_pPlayer->AngleToVertex(*(renderdata.pSeg->vert1)) + Angle(Angle(m_pPlayer->getFOV()) - m_pPlayer->GetAngle());
			Angle test2 = m_pPlayer->AngleToVertex(*(renderdata.pSeg->vert2)) + Angle(Angle(m_pPlayer->getFOV()) - m_pPlayer->GetAngle());

			int V1XScreen_texlerp = AngleToScreen(test1);		// AngleToScreen(renderdata.AngleV1 + m_pPlayer->getFOV() - m_pPlayer->GetAngle() - m_ScreenXToAngle[iXCurrent]);
			int V2XScreen_texlerp = AngleToScreen(test2);		// AngleToScreen(renderdata.AngleV2 + m_pPlayer->getFOV() - m_pPlayer->GetAngle() - m_ScreenXToAngle[iXCurrent]);

			if (V1XScreen_texlerp != V2XScreen_texlerp)
			{
				float wallWidth = sqrtf(pow(renderdata.pSeg->vert2->x - renderdata.pSeg->vert1->x, 2) + pow(renderdata.pSeg->vert2->y - renderdata.pSeg->vert1->y, 2));
				Angle SkewAngle = renderdata.AngleV1 - m_ScreenXToAngle[iXCurrent];
				float offset = renderdata.AngleV1.getTan() / renderdata.DistToNormal + (renderdata.pSeg->angle + Angle(90.0f) - 0).getTan();
				//float offset = sqrtf(pow(renderdata.pSeg->vert2->x - renderdata.pSeg->vert1->x, 2) + pow(renderdata.pSeg->vert2->y - renderdata.pSeg->vert1->y, 2));
				float t = ((float)(iXCurrent - V1XScreen_texlerp) / (float)(V2XScreen_texlerp - V1XScreen_texlerp));
				//float t = ((float)(iXCurrent - renderdata.VertX1OnScreen) / (float)(renderdata.VertX2OnScreen - renderdata.VertX1OnScreen));
				//El 0 es porque la textura empieza en coord x 0
				///NOOOOO: -0.00474777 -1.11299 520 -19700 520 639 57.9585 359.084        
				float z0 = m_pPlayer->distanceToEdge(*renderdata.pSeg->vert1);// renderdata.DistToV1;	//
				float z1 = m_pPlayer->distanceToEdge(*renderdata.pSeg->vert2);// renderdata.DistToV2;	//
				float u_alpha = ((1.0 - t) * (0 / z0) + (((float)t * wallWidth) / z1)) / (((1.0 - t) * 1.0f / z0) + (t * 1.0f / z1));
				if (u_alpha < 0)
				{
					//std::cerr << "NOOOOO: " << t << " " << u_alpha << " " << V1XScreen_texlerp << " " << V2XScreen_texlerp << " " << renderdata.VertX1OnScreen << " " << renderdata.VertX2OnScreen << " " << test21.GetValue() << " " << test22.GetValue() << std::endl;
				}
				DrawMidSectionV2(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, tex, (int)u_alpha, rw_midtexturemid, oldCeilingEnding, oldFloorStart);
			}
			
			//DrawMidSection(renderdata, iXCurrent, currentCeilingEnd, currentFloorStart, color);
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

