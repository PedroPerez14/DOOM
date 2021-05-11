/*
* Name: map.h
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Mapas de DOOM.
*/

#pragma once

#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include "map_types.h"
#include "../Player/Player.h"
#include <SFML/Graphics/RenderWindow.hpp>
//#include "../Enemy/Enemy.h"

class Map
{
public:
	Map(std::string n, Player *player);
	~Map();

	std::string GetName();				//Devuelve el nombre del mapa
	void Init();						//Inicializa las estructuras de datos con punteros a partir de los IDs le�dos del WAD
	void addVertex(Vertex& v);			//A�ade un v�rtice [x,y] al mapa
	void addLinedef(WADLinedef& l);		//A�ade un linedef al mapa
	void addSidedef(WADSidedef& sd);	//A�ade un sidedef al mapa
	void addThing(Thing& th);			//A�ade un nuevo thing al mapa
	void addNode(BSP_Node& node);		//A�ade un nuevo bsp_node al mapa
	void addSeg(WADSeg& seg);			//A�ade un nuevo seg al mapa
	void addSsect(Subsector& ssec);		//A�ade un nuevo subsector al mapa
	void addSect(WADSector& sec);		//A�ade un nuevo sector al mapa

	Linedef getLinedef(const int& n);	//Devuelve el n-�simo linedef
	Vertex getVertex(const int& n);		//Devuelve el n-�simo vertex
	BSP_Node getNode(const int& n);		//Devuelve el n-�simo BSP_Node
	Subsector getSSec(const int& n);	//Devuelve el n-�simo subsector
	Seg getSeg(const int& n);			//Devuelve el n-�simo segmento

	int getLinedefsSize();				//Tama�o del vector
	int getVertexSize();				//Tama�o del vector
	int getThingsSize();				//Tama�o del vector de things
	int getNodesSize();					//Tama�o del vector de nodes
	int getSubsecSize();				//Tama�o del vector de subsectores

	void LoadPlayer();					//Establece la posici�n inicial del jugador
	bool IsPointOnLeftSide(int XPosition, int YPosition, int nodeID); //Para buscar en el �rbol
	float getPlayerSubsecHeight();		//Devuelve la altura del suelo del subsector donde est� doomguy

	int getMapIndex();					//Devuelve el �ndice del mapa
	void setMapIndex(int idx);			//Fija el �ndice del mapa

	Automap_info getAutomapInfo();		//Devuelve las coordenadas X e Y m�s grandes y peque�as del mapa, usadas para automap

	//std::vector<Soldier*> loadEnemy();	//Devuelve la lista de enemigos del nivel cargado
	std::vector<Thing> getThings();

protected:
	int map_index;						//�ndice del mapa en los lumps, para acelerar la b�squeda
	std::string name;					//nombre del mapa

	/* Ver map_types.h para entender por qu� los tipo Sector y WADSector */

	std::vector<Vertex> map_verts;		//V�rtices del mapa
	std::vector<Linedef> map_linedefs;	//Linedefs del mapa
	std::vector<Sidedef> map_sidedefs;	//Sidedefs del mapa
	std::vector<Sector> map_sectors;	//Sectores del mapa
	std::vector<Seg> map_segs;			//Segs del mapa
	std::vector<Thing> map_things;		//Things del mapa
	std::vector<BSP_Node> map_nodes;	//Nodes del bsp-tree asociado al mapa
	std::vector<Subsector> map_subsecs;	//Subsectores del mapa

	std::vector<WADSector>* m_pSectors;	//Sectores le�dos del WAD
	std::vector<WADSidedef>* m_pSidedefs;//Sidedefs le�dos del WAD
	std::vector<WADLinedef>* m_pLinedefs;//Linedefs le�dos del WAD
	std::vector<WADSeg>* m_pSegs;		//Segs le�dos del WAD

	void BuildSectors();				//Inicializa map_sectors a partir de m_pSectors
	void BuildSidedefs();				//Inicializa map_sidedefs a partir de m_pSidedefs
	void BuildLinedefs();				//Inicializa map_sectors a partir de m_pLinedefs
	void BuildSegs();					//Inicializa map_sectors a partir de m_pSectors

	Automap_info automap_info;			//Informaci�n necesaria para hacer automap correctamente, la solicitar� Renderer
	Player* m_pPlayer;					//Referencia al jugador y su personaje
};

#endif