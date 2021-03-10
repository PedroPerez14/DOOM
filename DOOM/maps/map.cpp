/*
* Name: map.cpp
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
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

Linedef Map::getLinedef(const int& n)
{
	return map_linedefs[n];
}

Vertex Map::getVertex(const int& n)
{
	return map_verts[n];
}

int Map::getLinedefsSize()
{
	return map_linedefs.size();
}

int Map::getVertexSize()
{
	return map_verts.size();
}
