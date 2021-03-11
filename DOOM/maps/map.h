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

	Linedef getLinedef(const int& n);	//Devuelve el n-ésimo linedef
	Vertex getVertex(const int& n);		//Devuelve el n-ésimo vertex

	int getLinedefsSize();				//Tamaño del vector
	int getVertexSize();				//Tamaño del vector
	int getThingsSize();				//Tamaño del vector de things

	void Automap(sf::RenderWindow* r_window);//Dibuja el mapa 2D en pantalla
	void LoadPlayer();					//Establece la posición inicial del jugador

	int getMapIndex();					//Devuelve el índice del mapa
	void setMapIndex(int idx);			//Fija el índice del mapa

protected:
	int map_index;						//Índice del mapa en los lumps, para acelerar la búsqueda
	std::string name;					//nombre del mapa
	std::vector<Vertex> map_verts;		//Vértices del mapa
	std::vector<Linedef> map_linedefs;	//Linedefs del mapa
	std::vector<Thing> map_things;		//Things del mapa
	Automap_info automap_info;			//Información necesaria para hacer automap correctamente
	Player* m_pPlayer;					//Referencia al jugador y su personaje

	void AutomapPlayer(sf::RenderWindow* r_window);		//Auxiliar a automap, pinta al jugador
	void AutomapWalls(sf::RenderWindow* r_window);		//Auxiliar a automap, pinta el mapeado

};

#endif