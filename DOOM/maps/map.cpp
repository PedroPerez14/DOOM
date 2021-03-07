/*
* Name: map.cpp
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Mapas de DOOM.
*/

#include "map.h"

Map::Map(std::string n) : name(n)
{}

Map::~Map()
{}

std::string Map::GetName()
{
	return name;
}

void Map::addVertex(Vertex& v)
{
	map_verts.push_back(v);
}

void Map::addLinedef(Linedef& l)
{
	map_linedefs.push_back(l);
}
