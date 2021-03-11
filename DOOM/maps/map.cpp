/*
* Name: map.cpp
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Mapas de DOOM.
*/

#include "map.h"
#include "../doomdef.h"
#include "SFML/Graphics.hpp"

Map::Map(std::string n, Player* player) : name(n), automap_info({ INT16_MIN, INT16_MAX, INT16_MIN, INT16_MAX }), m_pPlayer(player), map_index(-1)
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

void Map::addThing(Thing& th)
{
    map_things.push_back(th);
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

int Map::getThingsSize()
{
    return map_things.size();
}

void Map::Automap(sf::RenderWindow* r_window)
{
    AutomapPlayer(r_window);
    AutomapWalls(r_window);
}

//WIP, me la he inventado. habrá que modificarla mucho en el futuro, o incluso borrarla
void Map::LoadPlayer()
{
    for (int i = 0; i < getThingsSize(); i++)
    {
        if (map_things[i].Type == ePLAYER1START)
        {
            std::cout << "Encontrado spawn del jugador en X= " << map_things[i].XPos << ",Y= " << map_things[i].YPos << std::endl;
            m_pPlayer->SetXPos(map_things[i].XPos);
            m_pPlayer->SetYPos(map_things[i].YPos);
            m_pPlayer->SetAngle(map_things[i].Angle);
        }
    }
}

void Map::AutomapPlayer(sf::RenderWindow* r_window)
{
    int w = r_window->getSize().x;
    int h = r_window->getSize().y;
    sf::CircleShape player_triangle(ceilf((3.0f * (float)w / (float)h) / ((float)SCREENWIDTH / (float)SCREENHEIGHT)), 3);   //Variar tamaño al escalar la pantalla
    player_triangle.setFillColor(sf::Color::Green);
    sf::Vector2f v = sf::Vector2f(3.0f + ((float)m_pPlayer->GetXPos() - (float)automap_info.minX) / ceilf(15.0f * (float)SCREENWIDTH / (float)w),
        (float)h - 1.0f - ((float)m_pPlayer->GetYPos() - (float)automap_info.minY) / ceilf(15.0f * (float)SCREENHEIGHT / (float)h));
    player_triangle.setPosition(v);
    r_window->draw(player_triangle);
}

void Map::AutomapWalls(sf::RenderWindow* r_window)
{
    const float scaleFactor = 15.0f;    //TODO está bien aquí?
    int w = r_window->getSize().x;
    int h = r_window->getSize().y;

    for (int i = 0; i < getLinedefsSize(); i++)
    {
        //Magia, tú solo disfruta
        Vertex v = getVertex(getLinedef(i).vert1);
        Vertex v2 = getVertex(getLinedef(i).vert2);
        sf::Vertex vertex[] = { sf::Vector2f(3.0f + ((float)v.x - (float)automap_info.minX) / ceilf(scaleFactor * (float)SCREENWIDTH / (float)w),
                                            (float)h - 1.0f - ((float)v.y - (float)automap_info.minY) / ceilf(scaleFactor * (float)SCREENHEIGHT / (float)h)),
                                sf::Vector2f(3.0f + ((float)v2.x - (float)automap_info.minX) / ceilf(scaleFactor * (float)SCREENWIDTH / (float)w),
                                            (float)h - 1.0f - ((float)v2.y - (float)automap_info.minY) / ceilf(scaleFactor * (float)SCREENHEIGHT / (float)h))
        };  //El 3+... y el h-1-... son porque la pantalla va de 0 a h-1, no de 1 a h. El 3 no es un 1 por arreglar imprecisiones pequeñas
        r_window->draw(vertex, 2, sf::Lines);
    }
}

int Map::getMapIndex()
{
    return map_index;
}

void Map::setMapIndex(int idx)
{
    map_index = idx;
}
