/*
* Name: map.cpp
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Mapas de DOOM.
*/

#include "map.h"
#include "../doomdef.h"

Map::Map(std::string n) : name(n), automap_info({ INT16_MIN, INT16_MAX, INT16_MIN, INT16_MAX })
{}

Map::~Map()
{}

std::string Map::GetName()
{
	return name;
}

void Map::addVertex(Vertex& v)
{
    if (v.x > automap_info.maxX)
        automap_info.maxX = v.x;
    if (v.y > automap_info.maxY)
        automap_info.maxY = v.y;
    if (v.x < automap_info.minX)
        automap_info.minX = v.x;
    if (v.y < automap_info.minY)
        automap_info.minY = v.y;

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

void Map::Automap(sf::RenderWindow* r_window)
{
    const int scaleFactor = 15;    //TODO está bien aquí?
    int w = r_window->getSize().x;
    int h = r_window->getSize().y;

    for (int i = 0; i < getLinedefsSize(); i++)
    {
        //Magia, tú solo disfruta
        Vertex v = getVertex(getLinedef(i).vert1);
        Vertex v2 = getVertex(getLinedef(i).vert2);
        sf::Vertex vertex[] = { sf::Vector2f(3 + (v.x - automap_info.minX) / ceilf(scaleFactor * (float)SCREENWIDTH / (float)w),
                                            h - 1 - (v.y - automap_info.minY) / ceilf(scaleFactor * (float)SCREENHEIGHT / (float)h)),
                                sf::Vector2f(3 + (v2.x - automap_info.minX) / ceilf(scaleFactor * (float)SCREENWIDTH / (float)w),
                                            h - 1 - (v2.y - automap_info.minY) / ceilf(scaleFactor * (float)SCREENHEIGHT / (float)h))
        };  //El 3+... y el h-1-... son porque la pantalla va de 0 a h-1, no de 1 a h. El 3 no es un 1 por arreglar imprecisiones pequeñas
        r_window->draw(vertex, 2, sf::Lines);
    }
}
