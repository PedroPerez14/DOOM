
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
#include <string>
#include <corecrt_math_defines.h>

DoomEngine::DoomEngine(Player* player, DisplayManager* dm, std::string level, int actualLevel_) : m_isOver(false), rendererWidth(SCREENWIDTH), rendererHeight(SCREENHEIGHT), showAutomap(false), m_WADLoader(GetWADFileName(), dm)
{
    step = 0;
    m_pMap = new Map(level, player);   //TODO elegir primer nivel de otra forma
    m_pPlayer = player;
    actualLevel = actualLevel_;
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

bool DoomEngine::Init(sf::RenderWindow* r_window, Status* gameState)
{
    m_pRenderWindow = r_window;
    m_WADLoader.LoadWAD();
    m_WADLoader.LoadMapData(m_pMap);
    m_pMap->LoadPlayer();               //WIP, inventada
    m_pMap->Init();                     //Inicializamos las estructuras de datos con punteros en vez de IDs para referenciarse entre sí

    std::vector<Thing> map_things = m_pMap->getThings();        //Obtener lista de cosas y obtencion de enemigos (lo siento si lo ponia en map petaba)
    for (auto a : map_things) {
        if ((a.Type == eFORMERHUMANTROOPER || a.Type == eIMP) ){// && a.XPos == 2272) {
            Soldier* newEnemigo = new Soldier(a.XPos, a.YPos, m_pPlayer, m_pMap, gameState);
            enemyList.push_back(newEnemigo);
            //std::cout << "Enemigo cargado en coordenadas: " << a->xValue() << " " << a->yValue() << std::endl;
        }
    }
    std::cout << "Lista de enemigos de " << enemyList.size() << std::endl;

    AssetsManager::getInstance()->Init(&m_WADLoader, m_pDisplayManager);

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
    const std::string wasd = "W94_1";
    AssetsManager* am = AssetsManager::getInstance();
    //am->Init(&m_WADLoader, m_pDisplayManager);
    Patch* p = am->GetPatch(wasd);
    uint8_t* pixels = new uint8_t[SCREENWIDTH * SCREENHEIGHT * 4];
    p->Render(pixels, m_pRenderWindow, 50, 50);
    delete pixels;
    pixels = nullptr;
    */

    //Renderizar textura de prueba SKY1
    /*
    Texture* pTex = AssetsManager::getInstance()->getTexture("EXITDOOR");
    uint8_t* buffer = new uint8_t[SCREENWIDTH * SCREENHEIGHT * 4];
    for (int i = 0; i < SCREENWIDTH * SCREENHEIGHT * 4; i++)
    {
        buffer[i] = 0;
    }
    sf::Texture texture;
    texture.create(SCREENWIDTH, SCREENHEIGHT);
    pTex->Render(buffer, 10, 10);
    texture.update(buffer);
    sf::Sprite sprite(texture);
    m_pRenderWindow->draw(sprite);
    delete buffer;
    buffer = nullptr;
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
            for (auto a : enemyList) {  //Comprobamos quien despierta
                a->playerMakeSound();
            }
            for (auto a : enemyList) {  //Comprobamos quien recibe daño
                Angle a1, a1fromPlayer;
                Vertex v;
                v.x = a->xValue();
                v.y = a->yValue();
                if (a->getVisible() && !a->isReallyDead()) {  //Si a es visible
                    if (m_pPlayer->ClipOneVertexInFOV(v, a1, a1fromPlayer)) {
                        float dist = sqrt((m_pPlayer->GetXPos() - a->xValue()) * (m_pPlayer->GetXPos() - a->xValue()) + (m_pPlayer->GetYPos() - a->yValue()) * (m_pPlayer->GetYPos() - a->yValue()));
                        if (dist < 200) {
                            if (a1fromPlayer > 78 && a1fromPlayer < 102) { //Si estas cerca, mas angulo de aceptar
                                float anguloAjsutado = 90 - (abs(90 - a1fromPlayer.GetValue()) * 7 / 12);   //Formula para adaptar los angulos al básico
                                a->getHitByUser(anguloAjsutado);
                                break;
                            }
                        }
                        else if (dist < 450) {
                            if (a1fromPlayer > 83 && a1fromPlayer < 97) {
                                a->getHitByUser(a1fromPlayer.GetValue());
                                break;
                            }
                        }
                        else {
                            if (a1fromPlayer > 86 && a1fromPlayer < 94) {   //Si estas lejos, menos angulo de aceptar
                                float anguloAjsutado = 90 - (abs(90 - a1fromPlayer.GetValue()) * 7 / 4);   //Formula para adaptar los angulos al básico
                                a->getHitByUser(anguloAjsutado);
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;

    case sf::Keyboard::Z:
        m_pPlayer->toogleInvencible();
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

bool compareDistEnemies(Soldier* a, Soldier* b) {
    return (a->getDistToPlayer() < b->getDistToPlayer());
}

bool DoomEngine::Update(Status status)
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
        Vertex vPlayer; //Se inicializa para no buscarlo en cada iteracion
        vPlayer.x = m_pPlayer->GetXPos();
        vPlayer.y = m_pPlayer->GetYPos();
        for (auto a : enemyList) {
            //Para cada enemigo, ejecutar playerMove para ver si se despierta.
            a->playerMove();

            //Para cada enemigo, ejecutar su funcion "nextMove" y asi actualiza su X e Y.
            a->nextMove();

            //Con los movimientos resultantes, actualizar si son visibles o no para el jugador (paredes a mitad de rayo)
            if (testIfVisible(a, &vPlayer)) {
                a->setVisible(true);
            }
            else {
                a->setVisible(false);
            }
        }

        //Ordenamos la lista de enemigos (comodidad para disparos/render)
        std::sort(enemyList.begin(), enemyList.end(), compareDistEnemies);

        if (actualLevel == 1 && vPlayer.x < 2950 && vPlayer.x > 2918 && vPlayer.y < -4756 && vPlayer.y > -4788) {
            actualLevel = 2;
            return true;
        }
        if (actualLevel == 2 && vPlayer.x < -261 && vPlayer.x > -314 && vPlayer.y < 2363 && vPlayer.y > 2319) {
            actualLevel = 3;
            return true;
        }
        if (actualLevel == 3 && vPlayer.x < 741 && vPlayer.x > 675 && vPlayer.y < -1697 && vPlayer.y > -1754) {
            actualLevel = 3;
            return true;
        }
    }
    return false;
}

//Termina todos los procesos (enemigos) antes de su destrucción y cambio de nivel
void DoomEngine::endProcess() {
    for (auto a : enemyList) {
        a->setDead(true);
    }
}

//Me da error si incluyo el geometry.h y estoy hasta los huevos, asi que ahora se llama intersection en vez de intersec y apañao
bool intersection(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3, const float& x4, const float& y4, float& Px, float& Py)
{
    float denominador = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denominador == 0)
    {
        return false;
    }
    else
    {
        float x1y2_y1x2 = (x1 * y2 - y1 * x2);
        float x3y4_y3x4 = (x3 * y4 - y3 * x4);
        Px = (x1y2_y1x2 * (x3 - x4) - (x1 - x2) * x3y4_y3x4) / denominador;
        Py = (x1y2_y1x2 * (y3 - y4) - (y1 - y2) * x3y4_y3x4) / denominador;
        return true;
    }
}

bool DoomEngine::testIfVisible(Soldier* enemigo, Vertex* v3) {
    for (int i = 0; i < m_pMap->getLinedefsSize(); i++)
    {
        uint16_t aux = m_pMap->getLinedef(i).flags;
        uint16_t aux2 = m_pMap->getLinedef(i).line_type;
        bool isAReallyWall = (aux & (1 << 0)) || (aux2 & (1 << 0));     //Primer bit0 controla paredes utiles. El segudo operador quita puertas etc...
        if (isAReallyWall){
            Vertex v = *(m_pMap->getLinedef(i).vert1);
            Vertex v2 = *(m_pMap->getLinedef(i).vert2);
            Vertex v4;
            v4.x = enemigo->xValue();
            v4.y = enemigo->yValue();
            float x_pos, y_pos;
            if (intersection(v.x, v.y, v2.x, v2.y, v3->x, v3->y, v4.x, v4.y, x_pos, y_pos)) {  //Las rectas intersectan. Comprobar que en un punto del segmento
                if (((x_pos >= v3->x && x_pos <= v4.x) || (x_pos >= v4.x && x_pos <= v3->x)) && ((y_pos >= v3->y && y_pos <= v4.y) || (y_pos >= v4.y && y_pos <= v3->y))) { //Si es en parte del segmento, return false porque ya no son visibles
                    if (((x_pos >= v.x && x_pos <= v2.x) || (x_pos >= v2.x && x_pos <= v.x)) && ((y_pos >= v.y && y_pos <= v2.y) || (y_pos >= v2.y && y_pos <= v.y))) { //Si es en parte del segmento, return false porque ya no son visibles
                    //    std::cout << "El enemigo " << v4.x << " " << v4.y << " y el user " << v3->x << v3->y << " golpea en " << x_pos << y_pos;
                    //    std::cout << " pared de golpe = " << v.x << " " << v.y << " y " << v2.x << " " << v2.y << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    return true;
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

void DoomEngine::enemyRecount(int& total, int& killed) {
    total = enemyList.size();
    killed = 0;
    for (auto a : enemyList) {
        if (a->isReallyDead()) {
            killed++;
        }
    }
}