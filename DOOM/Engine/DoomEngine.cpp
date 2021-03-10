/*
* Name: DoomEngine.cpp
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Motor del juego DOOM.
*/

#pragma once
#include "SFML/Graphics.hpp"
#include "DoomEngine.h"
#include "../doomdef.h"
#include "../maps/map.h"

DoomEngine::DoomEngine() : m_isOver(false), rendererWidth(SCREENWIDTH), rendererHeight(SCREENHEIGHT), m_WADLoader(GetWADFileName())
{
    m_pMap = new Map("E1M1");
}

DoomEngine::~DoomEngine()
{

}


std::string DoomEngine::GetWADFileName()
{
    return "C:\\Users\\Pedro\\Desktop\\Universidad\\Cuarto\\Videojuegos\\assets_doom\\DOOM.WAD";    //TODO cambiar más adelante, poner el wad en el proyecto y meterle la ruta relativa
}

bool DoomEngine::Init()
{
    m_WADLoader.LoadWAD();
    m_WADLoader.LoadMapData(*m_pMap);
    return true;
}

void DoomEngine::Render(sf::RenderWindow *r_window)
{
    int maxX, minX, maxY, minY, scaleFactor;
    
    maxX = m_pMap->getVertex(0).x;
    minX = m_pMap->getVertex(0).x;
    maxY = m_pMap->getVertex(0).y;
    minY = m_pMap->getVertex(0).y;

    for (int i = 1; i < m_pMap->getVertexSize(); i++)
    {
        Vertex v = m_pMap->getVertex(i);
        if (v.x > maxX)
            maxX = v.x;
        if (v.y > maxY)
            maxY = v.y;
        if (v.x < minX)
            minX = v.x;
        if (v.y < minY)
            minY = v.y;
    }

    r_window->clear(sf::Color::Green);

    for (int i = 0; i < m_pMap->getLinedefsSize(); i++)
    {
        Vertex v = m_pMap->getVertex(m_pMap->getLinedef(i).vert1);
        Vertex v2 = m_pMap->getVertex(m_pMap->getLinedef(i).vert2);
        sf::Vertex vertex[] = { sf::Vector2f((v.x - minX) / 15, r_window->getSize().y - 1 - (v.y - minY) / 15), sf::Vector2f((v2.x - minX) / 15, r_window->getSize().y - 1 - (v2.y - minY) / 15) };

        //sf::Vector2f v2i = (sf::Vector2f)r_window->mapCoordsToPixel(vertex[0].position, r_window->getView());
        //sf::Vector2f v2i = r_window->mapPixelToCoords((sf::Vector2i)vertex[0].position, r_window->getView());
        //vertex[0].position = v2i;
        //v2i = r_window->mapPixelToCoords((sf::Vector2i)vertex[1].position, r_window->getView());
        //vertex[1].position = v2i;

        r_window->draw(vertex, 2, sf::Lines);
    }
    r_window->display();
}

//TODO MORRALLA

void DoomEngine::KeyPressed(sf::Event& event)
{

}

void DoomEngine::KeyReleased(sf::Event& event)
{

}

void DoomEngine::Quit()
{
    m_isOver = true;
    //TODO
}

void DoomEngine::Update()
{

}

bool DoomEngine::isOver()
{
    return m_isOver;
}

int DoomEngine::GetRendererWidth()
{
    return rendererWidth;
}

int DoomEngine::GetRendererHeight()
{
    return rendererHeight;
}

int DoomEngine::GetTimePerFrame()
{
    return 0;
}

std::string DoomEngine::GetName()
{
    return "DOOM";
}

