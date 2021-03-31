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
#include "../Player/Player.h"

DoomEngine::DoomEngine(Player* player) : m_isOver(false), rendererWidth(SCREENWIDTH), rendererHeight(SCREENHEIGHT), m_WADLoader(GetWADFileName())
{
    m_pMap = new Map("E1M1", player);
    m_pPlayer = player;
}

DoomEngine::~DoomEngine()
{

}


std::string DoomEngine::GetWADFileName()
{
    //return "C:\\Users\\Pedro\\Desktop\\Universidad\\Cuarto\\Videojuegos\\assets_doom\\DOOM.WAD";    //TODO cambiar más adelante, poner el wad en el proyecto y meterle la ruta relativa
    return "../../../../assets/DOOM.WAD";
}

bool DoomEngine::Init()
{
    m_WADLoader.LoadWAD();
    m_WADLoader.LoadMapData(m_pMap);
    m_pMap->LoadPlayer();               //WIP, inventada
    return true;
}

void DoomEngine::Render(sf::RenderWindow *r_window)
{
    r_window->clear(sf::Color::Black);
    m_pMap->Automap(r_window);
    r_window->display();
}

//TODO de momento lo pongo aquí y luego ya veré qué hago con todo
void DoomEngine::KeyPressed(sf::Event& event)
{
    switch(event.key.code)
    {
        case sf::Keyboard::Right:
            m_pPlayer->RotateRight();
            break;
        case sf::Keyboard::Left:
            m_pPlayer->RotateLeft();
            break;
        default:
            break;
    }

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

