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

class Map
{
public:
	Map(std::string n, Player *player);
	~Map();

	std::string GetName();				//Devuelve el nombre del mapa
	void addVertex(Vertex& v);			//Añade un vértice [x,y] al mapa
	void addLinedef(Linedef& l);		//Añade un linedef al mapa
	void addThing(Thing& th);			//Añade un nuevo thing al mapa
	void addNode(BSP_Node& node);		//Añade un nuevo bsp_node al mapa
	void addSeg(Seg& seg);				//Añade un nuevo seg al mapa
	void addSsect(Subsector& ssec);		//Añade un nuevo subsector al mapa

	Linedef getLinedef(const int& n);	//Devuelve el n-ésimo linedef
	Vertex getVertex(const int& n);		//Devuelve el n-ésimo vertex

	int getLinedefsSize();				//Tamaño del vector
	int getVertexSize();				//Tamaño del vector
	int getThingsSize();				//Tamaño del vector de things
	int getNodesSize();					//Tamaño del vector de nodes

	void Automap(sf::RenderWindow* r_window);//Dibuja el mapa 2D en pantalla
	void LoadPlayer();					//Establece la posición inicial del jugador

	int getMapIndex();					//Devuelve el índice del mapa
	void setMapIndex(int idx);			//Fija el índice del mapa

	void RenderBSPNodes(sf::RenderWindow* r_window);				//Renderiza el nodo BSP donde esté el jugador

protected:
	int map_index;						//Índice del mapa en los lumps, para acelerar la búsqueda
	std::string name;					//nombre del mapa
	std::vector<Vertex> map_verts;		//Vértices del mapa
	std::vector<Linedef> map_linedefs;	//Linedefs del mapa
	std::vector<Thing> map_things;		//Things del mapa
	std::vector<BSP_Node> map_nodes;	//Nodes del bsp-tree asociado al mapa
	std::vector<Seg> map_segs;			//Segs del mapa
	std::vector<Subsector> map_subsecs;	//Subsectores del mapa
	Automap_info automap_info;			//Información necesaria para hacer automap correctamente //TODO mover a renderer
	Player* m_pPlayer;					//Referencia al jugador y su personaje

	//------------------------------------------------------//
	/* Funciones WIP para ir renderizando cosas en pantalla */
	//------------------------------------------------------//
					// mover a Renderer //

	void AutomapPlayer(sf::RenderWindow* r_window);		//Auxiliar a automap, pinta al jugador
	void AutomapWalls(sf::RenderWindow* r_window);		//Auxiliar a automap, pinta el mapeado
	void AutomapNodes(sf::RenderWindow* r_window);		//Auxiliar a automap, pinta los nodos (WIP)

	void convertToScreenCords(const float& Xin, float& Xout, const float& Yin, float& Yout, 
		const float& currentwidth, const float& currentheight, const float& width, const float& height);
	//Auxiliar, convierte de coordenadas de mapa a pantalla, para hacer automap
	//TODO habrá que rehacer todo esto bien con las views de sfml en algún momento (a no ser que spaghetti)

	bool IsPointOnLeftSide(int XPosition, int YPosition, int iNodeID);	//Para recorrer el árbol de nodos BSP
	
	void RenderBSPNodes(int16_t nodeID, sf::RenderWindow* r_window);	//Auxiliar a RenderBSPNodes(), encuentra al jugador y renderiza su nodo
	void RenderSubsector(int subsectorID, sf::RenderWindow* r_window);	//Auxiliar a RenderBSPNodes(int nodeID), renderiza ssec hoja
};

#endif