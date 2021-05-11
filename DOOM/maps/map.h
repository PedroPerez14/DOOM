/*
* Name: map.h
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
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
	void Init();						//Inicializa las estructuras de datos con punteros a partir de los IDs leídos del WAD
	void addVertex(Vertex& v);			//Añade un vértice [x,y] al mapa
	void addLinedef(WADLinedef& l);		//Añade un linedef al mapa
	void addSidedef(WADSidedef& sd);	//Añade un sidedef al mapa
	void addThing(Thing& th);			//Añade un nuevo thing al mapa
	void addNode(BSP_Node& node);		//Añade un nuevo bsp_node al mapa
	void addSeg(WADSeg& seg);			//Añade un nuevo seg al mapa
	void addSsect(Subsector& ssec);		//Añade un nuevo subsector al mapa
	void addSect(WADSector& sec);		//Añade un nuevo sector al mapa

	Linedef getLinedef(const int& n);	//Devuelve el n-ésimo linedef
	Vertex getVertex(const int& n);		//Devuelve el n-ésimo vertex
	BSP_Node getNode(const int& n);		//Devuelve el n-ésimo BSP_Node
	Subsector getSSec(const int& n);	//Devuelve el n-ésimo subsector
	Seg getSeg(const int& n);			//Devuelve el n-ésimo segmento

	int getLinedefsSize();				//Tamaño del vector
	int getVertexSize();				//Tamaño del vector
	int getThingsSize();				//Tamaño del vector de things
	int getNodesSize();					//Tamaño del vector de nodes
	int getSubsecSize();				//Tamaño del vector de subsectores

	void LoadPlayer();					//Establece la posición inicial del jugador
	bool IsPointOnLeftSide(int XPosition, int YPosition, int nodeID); //Para buscar en el árbol
	float getPlayerSubsecHeight();		//Devuelve la altura del suelo del subsector donde está doomguy

	int getMapIndex();					//Devuelve el índice del mapa
	void setMapIndex(int idx);			//Fija el índice del mapa

	Automap_info getAutomapInfo();		//Devuelve las coordenadas X e Y más grandes y pequeñas del mapa, usadas para automap

	//std::vector<Soldier*> loadEnemy();	//Devuelve la lista de enemigos del nivel cargado
	std::vector<Thing> getThings();

protected:
	int map_index;						//Índice del mapa en los lumps, para acelerar la búsqueda
	std::string name;					//nombre del mapa

	/* Ver map_types.h para entender por qué los tipo Sector y WADSector */

	std::vector<Vertex> map_verts;		//Vértices del mapa
	std::vector<Linedef> map_linedefs;	//Linedefs del mapa
	std::vector<Sidedef> map_sidedefs;	//Sidedefs del mapa
	std::vector<Sector> map_sectors;	//Sectores del mapa
	std::vector<Seg> map_segs;			//Segs del mapa
	std::vector<Thing> map_things;		//Things del mapa
	std::vector<BSP_Node> map_nodes;	//Nodes del bsp-tree asociado al mapa
	std::vector<Subsector> map_subsecs;	//Subsectores del mapa

	std::vector<WADSector>* m_pSectors;	//Sectores leídos del WAD
	std::vector<WADSidedef>* m_pSidedefs;//Sidedefs leídos del WAD
	std::vector<WADLinedef>* m_pLinedefs;//Linedefs leídos del WAD
	std::vector<WADSeg>* m_pSegs;		//Segs leídos del WAD

	void BuildSectors();				//Inicializa map_sectors a partir de m_pSectors
	void BuildSidedefs();				//Inicializa map_sidedefs a partir de m_pSidedefs
	void BuildLinedefs();				//Inicializa map_sectors a partir de m_pLinedefs
	void BuildSegs();					//Inicializa map_sectors a partir de m_pSectors

	Automap_info automap_info;			//Información necesaria para hacer automap correctamente, la solicitará Renderer
	Player* m_pPlayer;					//Referencia al jugador y su personaje
};

#endif