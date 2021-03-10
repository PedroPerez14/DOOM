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

class Map
{
public:
	Map(std::string n);
	~Map();

	std::string GetName();	//Devuelve el nombre del mapa
	void addVertex(Vertex& v);	//A�ade un v�rtice [x,y] al mapa
	void addLinedef(Linedef& l);	//A�ade un linedef al mapa

	Linedef getLinedef(const int& n);	//Devuelve el n-�simo linedef
	Vertex getVertex(const int& n);		//Devuelve el n-�simo vertex

	int getLinedefsSize();				//Tama�o del vector
	int getVertexSize();				//Tama�o del vector

protected:
	std::string name;		//nombre del mapa
	std::vector<Vertex> map_verts;	//V�rtices del mapa
	std::vector<Linedef> map_linedefs;	//Linedefs del mapa

};

#endif