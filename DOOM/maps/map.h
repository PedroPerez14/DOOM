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

class Map
{
public:
	Map(std::string n);
	~Map();

	std::string GetName();	//Devuelve el nombre del mapa
	void addVertex(Vertex& v);	//Añade un vértice [x,y] al mapa
	void addLinedef(Linedef& l);	//Añade un linedef al mapa

	Linedef getLinedef(const int& n);	//Devuelve el n-ésimo linedef
	Vertex getVertex(const int& n);		//Devuelve el n-ésimo vertex

	int getLinedefsSize();				//Tamaño del vector
	int getVertexSize();				//Tamaño del vector

protected:
	std::string name;		//nombre del mapa
	std::vector<Vertex> map_verts;	//Vértices del mapa
	std::vector<Linedef> map_linedefs;	//Linedefs del mapa

};

#endif