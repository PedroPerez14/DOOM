
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
#include "../maps/map_types.h"
#include "../Game/Game.h"
#include "../Enemy/Soldier.h"
#include "../Player/Player.h"
#include "../PatchesTextures/Patch.h"
#include "../PatchesTextures/AssetsManager.h"
#include <corecrt_math_defines.h>

DoomEngine::DoomEngine(Player* player, DisplayManager* dm) : m_isOver(false), rendererWidth(SCREENWIDTH), rendererHeight(SCREENHEIGHT), showAutomap(false), m_WADLoader(GetWADFileName(), dm)
{
    step = 0;
    m_pMap = new Map("E1M1", player);   //TODO elegir primer nivel de otra forma
    m_pPlayer = player;
    m_pDisplayManager = dm;
}

DoomEngine::~DoomEngine()
{

}


std::string DoomEngine::GetWADFileName()
{
    //return "C:\\Users\\Pedro\\Desktop\\Universidad\\Cuarto\\Videojuegos\\assets_doom\\DOOM.WAD";    //TODO cambiar más adelante, poner el wad en el proyecto y meterle la ruta relativa
    return "../../../../assets/DOOM.WAD";
}

void DoomEngine::setDeltaTime(const float& dT)
{
    m_deltaTime = dT;
}

bool DoomEngine::Init(sf::RenderWindow* r_window)
{
    m_pRenderWindow = r_window;
    m_WADLoader.LoadWAD();
    m_WADLoader.LoadMapData(m_pMap);
    m_pMap->LoadPlayer();               //WIP, inventada
    m_pMap->Init();                     //Inicializamos las estructuras de datos con punteros en vez de IDs para referenciarse entre sí

    std::vector<Thing> map_things = m_pMap->getThings();        //Obtener lista de cosas y obtencion de enemigos (lo siento si lo ponia en map petaba)
    for (auto a : map_things) {
        if (a.Type == 3004 || a.Type == 3001) { //3004 zombieman, 3001 imp, from https://zdoom.org/wiki/Standard_editor_numbers
            Soldier* newEnemigo = new Soldier(a.XPos, a.YPos, m_pPlayer);
            enemyList.push_back(newEnemigo);
            //std::cout << "Enemigo cargado en coordenadas: " << a->xValue() << " " << a->yValue() << std::endl;
        }
    }

    m_pRenderer = new Renderer(r_window);
    m_pRenderer->Init(m_pMap, m_pPlayer, m_pDisplayManager, enemyList);
    return true;
}

void DoomEngine::Render()
{
    m_pRenderer->InitFrame();
    m_pRenderer->Render(showAutomap, m_deltaTime);
    //Borrar luego pls
    /*
    const std::string wasd = "PISGA0";
    AssetsManager* am = AssetsManager::getInstance();
    am->Init(&m_WADLoader, m_pDisplayManager);
    Patch* p = am->GetPatch(wasd);
    uint8_t* pixels = new uint8_t[SCREENWIDTH * SCREENHEIGHT * 4];
    p->Render(pixels, m_pRenderWindow, -p->getXOffset(), -p->getYOffset());
    delete pixels;
    pixels = nullptr;
    */
}

//TODO de momento lo pongo aquí y luego ya veré qué hago con todo
void DoomEngine::KeyPressed(sf::Event& event)
{
    //TODO rehacer con booleanos para procesar mejor todo esto y con deltaTime
    switch (event.key.code)
    {
    case sf::Keyboard::Right:
        //m_pPlayer->RotateRight();
        m_pPlayer->toggleRotateClockwise(true);
        break;
    case sf::Keyboard::Left:
        //m_pPlayer->RotateLeft();
        m_pPlayer->toggleRotateAnticlockwise(true);
        break;
    case sf::Keyboard::Up:
        //m_pPlayer->moveForward();
        m_pPlayer->toggleMoveForward(true);
        break;
    case sf::Keyboard::Down:
        //m_pPlayer->moveBackwards();
        m_pPlayer->toggleMoveBackwards(true);
        break;
    case sf::Keyboard::Tab:
        showAutomap = true;
        break;
    case sf::Keyboard::LShift:
        m_pPlayer->toggleRunning(true);
        break;
    case sf::Keyboard::RControl:
    case sf::Keyboard::LControl:
        if (m_pPlayer->shoot()) {       //Pega un tiro. Return true si realmente lo ha hecho
            for (auto a : enemyList) {
                a->playerMakeSound();
            }
        }
        break;

    default:
        break;
    }
}

void DoomEngine::KeyReleased(sf::Event& event)
{
    switch (event.key.code)
    {
    case sf::Keyboard::Right:
        //m_pPlayer->RotateRight();
        m_pPlayer->toggleRotateClockwise(false);
        break;
    case sf::Keyboard::Left:
        //m_pPlayer->RotateLeft();
        m_pPlayer->toggleRotateAnticlockwise(false);
        break;
    case sf::Keyboard::Up:
        //m_pPlayer->moveForward();
        m_pPlayer->toggleMoveForward(false);
        break;
    case sf::Keyboard::Down:
        //m_pPlayer->moveBackwards();
        m_pPlayer->toggleMoveBackwards(false);
        break;
    case sf::Keyboard::Tab:
        showAutomap = false;
        break;
    case sf::Keyboard::LShift:
        m_pPlayer->toggleRunning(false);
    default:
        break;
    }
}

void DoomEngine::releasePlayerInputs()
{
    showAutomap = false;
    m_pPlayer->toggleRunning(false);
    m_pPlayer->toggleMoveBackwards(false);
    m_pPlayer->toggleMoveForward(false);
    m_pPlayer->toggleRotateAnticlockwise(false);
    m_pPlayer->toggleRotateClockwise(false); 
}

void DoomEngine::Quit()
{
    m_isOver = true;
    //TODO algo más?
}

void DoomEngine::Update(Status status)
{
    if (status == Status::ePLAYING)
    {
        if (!m_pPlayer->checkDead())    //TODO esto sería mejor moverlo a otro sitio, como a Player.cpp, pero de momento y probablemente para siempre, se queda aquí
        {
            float baseHeight = m_pMap->getPlayerSubsecHeight();
            float offsetHeight = 0.0f;
            if (m_pPlayer->isMoving() && !m_pPlayer->isRunning())
            {
                offsetHeight = sinf(((M_PI * 2.0f) / 40.0f) * step) * 10.0f / (m_deltaTime * (float)TARGETFRAMERATE);
                step = (step + 1) % 40;
            }
            if (m_pPlayer->isRunning() && m_pPlayer->isMoving())
            {
                offsetHeight = sinf(((M_PI * 2.0f) / 40.0f) * step) * 15.0f / (m_deltaTime * (float)TARGETFRAMERATE);
                step = (step + 1) % 40;
            }
            m_pPlayer->SetZPos(baseHeight + offsetHeight); //Think() sería mejor nombre
        }
        
        //Mover al jugador aqui
        //Calcular colisiones //TODO
        //Si hay colisiones, reposicionar
        m_pPlayer->Move(m_deltaTime);

        //Pensar y mover/atacar la IA de los enemigos
        //Para cada enemigo, ejecutar playerMove para ver si se despierta.
        for (auto a : enemyList) {
            a->playerMove();
        }

        //Para cada enemigo, ejecutar su funcion "nextMove" y asi actualiza su X e Y.
        for (auto a : enemyList) {
            a->nextMove();
        }
    }
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

float DoomEngine::getDeltaTime()
{
    return m_deltaTime;
}

std::string DoomEngine::GetName()
{
    return "DOOM";
}

