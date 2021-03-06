
/*
* Name: DoomEngine.cpp
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: V?ctor Mart?nez Lascorz (738845)
* Author: Pedro Jos? P?rez garc?a (756642)
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
#include "../Misc/Geometry.h"
#include "../PatchesTextures/Patch.h"
#include "../PatchesTextures/AssetsManager.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include "math.h"
#include <thread>
#include <corecrt_math_defines.h>
#include <random>

DoomEngine::DoomEngine(Player* player, DisplayManager* dm, std::string level, int actualLevel_) : m_isOver(false), rendererWidth(SCREENWIDTH), rendererHeight(SCREENHEIGHT), showAutomap(false), m_WADLoader(GetWADFileName(), dm)
{
    step = 0;
    m_pMap = new Map(level, player);
    m_pPlayer = player;
    actualLevel = actualLevel_;
    m_pDisplayManager = dm;
}

DoomEngine::~DoomEngine()
{

}


std::string DoomEngine::GetWADFileName()
{
    return "assets/DOOM.WAD";
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
    m_pMap->Init();                     //Inicializamos las estructuras de datos con punteros en vez de IDs para referenciarse entre s?

    return true;
}

bool DoomEngine::InitEnemy(Status* gameState, int dificultad){
    std::vector<Thing> map_things = m_pMap->getThings();        //Obtener lista de cosas y obtencion de enemigos (lo siento si lo ponia en map petaba)
    srand(time(NULL));
    if (dificultad == 0) {      //I'm too young to die
        for (auto a : map_things) {
            if ((a.Type == eFORMERHUMANTROOPER || a.Type == eIMP) && (a.Flags & 0x0001)) {
                int aleatorio = rand();
                aleatorio = aleatorio + a.XPos + a.YPos;
                Soldier* newEnemigo = new Soldier(a.XPos, a.YPos, m_pPlayer, m_pMap, gameState, aleatorio % 5);
                enemyList.push_back(newEnemigo);
            }
        }
    }
    else if (dificultad == 1) { //Hurt me more
        for (auto a : map_things) {
            if ((a.Type == eFORMERHUMANTROOPER || a.Type == eIMP) && (a.Flags & 0x0002)) {
                int aleatorio = rand();
                aleatorio = aleatorio + a.XPos + a.YPos;
                Soldier* newEnemigo = new Soldier(a.XPos, a.YPos, m_pPlayer, m_pMap, gameState, aleatorio % 5);
                enemyList.push_back(newEnemigo);
            }
        }
    }
    else {      //Ultra violence & Nightmare
        for (auto a : map_things) {
            if ((a.Type == eFORMERHUMANTROOPER || a.Type == eIMP) && (a.Flags & 0x0004)) {
                int aleatorio = rand();
                aleatorio = aleatorio + a.XPos + a.YPos;
                Soldier* newEnemigo = new Soldier(a.XPos, a.YPos, m_pPlayer, m_pMap, gameState, aleatorio % 5);
                enemyList.push_back(newEnemigo);
            }
        }
    }


    AssetsManager::getInstance()->Init(&m_WADLoader, m_pDisplayManager);

    m_pRenderer = new Renderer(m_pRenderWindow);
    m_pRenderer->Init(m_pMap, m_pPlayer, m_pDisplayManager, enemyList);

    return true;
}

void DoomEngine::initVolumenes(int soundLevel_) {
    for (auto a : enemyList) {
        a->changeVolumenes(soundLevel_);
    }
}

void DoomEngine::Render()
{
    m_pRenderer->InitFrame();
    m_pRenderer->Render(showAutomap, m_deltaTime);
   
}

void DoomEngine::KeyPressed(sf::Event& event)
{
    switch (event.key.code)
    {
    case sf::Keyboard::Right:
        m_pPlayer->toggleRotateClockwise(true);
        break;
    case sf::Keyboard::Left:
        m_pPlayer->toggleRotateAnticlockwise(true);
        break;
    case sf::Keyboard::Up:
        m_pPlayer->toggleMoveForward(true);
        break;
    case sf::Keyboard::Down:
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
            for (auto a : enemyList) {  //Comprobamos quien recibe da?o
                Angle a1, a1fromPlayer;
                Vertex v;
                v.x = a->xValue();
                v.y = a->yValue();
                if (a->getVisible() && !a->isReallyDead()) {  //Si a es visible
                    if (m_pPlayer->ClipOneVertexInFOV(v, a1, a1fromPlayer)) {
                        float dist = sqrt((m_pPlayer->GetXPos() - a->xValue()) * (m_pPlayer->GetXPos() - a->xValue()) + (m_pPlayer->GetYPos() - a->yValue()) * (m_pPlayer->GetYPos() - a->yValue()));
                        if (dist < 200) {
                            if (a1fromPlayer > 78 && a1fromPlayer < 102) { //Si estas cerca, mas angulo de aceptar
                                float anguloAjsutado = 90 - (abs(90 - a1fromPlayer.GetValue()) * 7 / 12);   //Formula para adaptar los angulos al b?sico
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
                                float anguloAjsutado = 90 - (abs(90 - a1fromPlayer.GetValue()) * 7 / 4);   //Formula para adaptar los angulos al b?sico
                                a->getHitByUser(anguloAjsutado);
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;

    case sf::Keyboard::P:
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
        m_pPlayer->toggleRotateClockwise(false);
        break;
    case sf::Keyboard::Left:
        m_pPlayer->toggleRotateAnticlockwise(false);
        break;
    case sf::Keyboard::Up:
        m_pPlayer->toggleMoveForward(false);
        break;
    case sf::Keyboard::Down:
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
}

bool compareDistEnemies(Soldier* a, Soldier* b) {
    return (a->getDistToPlayer() < b->getDistToPlayer());
}

bool DoomEngine::Update(Status status)
{
    if (status == Status::ePLAYING)
    {
        //Mover al jugador aqui
        //Calcular colisiones
        //Si hay colisiones, reposicionar
        int16_t old_subsecID = 0, new_subsecID = 0;
        float old_x = m_pPlayer->GetXPos();
        float old_y = m_pPlayer->GetYPos();
        float old_z = m_pMap->getPlayerSubsecHeight(old_subsecID);
        m_pPlayer->Move(m_deltaTime);
        float new_x = m_pPlayer->GetXPos();
        float new_y = m_pPlayer->GetYPos();
        float new_z = m_pMap->getPlayerSubsecHeight(new_subsecID);

        int segCollided;
        if (collisionDetect((int16_t)old_subsecID, old_x, old_y, segCollided))
        {
            m_pPlayer->SetXPos(old_x);
            m_pPlayer->SetYPos(old_y);
            m_pPlayer->SetZPos(old_z);

        }
        else if (collisionDetect((int16_t)new_subsecID, new_x, new_y, segCollided))
        {
            m_pPlayer->SetXPos(old_x);
            m_pPlayer->SetYPos(old_y);
            m_pPlayer->SetZPos(old_z);
        }


        if (!m_pPlayer->checkDead())    //TODO esto ser?a mejor moverlo a otro sitio, como a Player.cpp, pero de momento y probablemente para siempre, se queda aqu?
        {   
            int16_t subsecID;
            float baseHeight = m_pMap->getPlayerSubsecHeight(subsecID);
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
            m_pPlayer->SetZPos(baseHeight + offsetHeight); //Think() ser?a mejor nombre
        }

        //Pensar y mover/atacar la IA de los enemigos
        Vertex vPlayer; //Se inicializa para no buscarlo en cada iteracion
        vPlayer.x = m_pPlayer->GetXPos();
        vPlayer.y = m_pPlayer->GetYPos();
        for (auto a : enemyList) {
            //Para cada enemigo, ejecutar playerMove para ver si se despierta.
            a->playerMove();

            //Para cada enemigo, ejecutar su funcion "nextMove" y asi actualiza su X e Y.
            float old_x = a->xValue();
            float old_y = a->yValue();
            int16_t old_enemy_subsector;
            m_pMap->getEnemySubsecHeight(old_x, old_y, old_enemy_subsector);
            a->nextMove(m_deltaTime);
            float new_x = a->xValue();
            float new_y = a->yValue();
            int16_t new_enemy_subsector;
            m_pMap->getEnemySubsecHeight(new_x, new_y, new_enemy_subsector);
            if (collisionDetect(new_enemy_subsector, new_x, new_y, segCollided) || collisionDetect(old_enemy_subsector, old_x, old_y, segCollided))
            {
                a->setPosition(old_x, old_y);
            }

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

        if (actualLevel == 1 && vPlayer.x < 2970 && vPlayer.x > 2905 && vPlayer.y < -4750 && vPlayer.y > -4795) {
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

//Termina todos los procesos (enemigos) antes de su destrucci?n y cambio de nivel
void DoomEngine::killEverything() {
    for (auto a : enemyList) {
        a->setDead(true);
    }
}

std::vector<Soldier*> DoomEngine::getEnemyList() {
    return enemyList;
}

//Me da error si incluyo el geometry.h y estoy hasta los huevos, asi que ahora se llama intersection en vez de intersec y apa?ao
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
        bool isAReallyWall = ((aux & (1 << 0)) && !(aux & (1 << 3))) || (aux2 == 1) || (aux2 > 25 && aux2 < 29) || (aux2 > 31 && aux2 < 35) || (aux2 > 133 && aux2 < 138) || aux2 == 99;     //Primer bit0 controla paredes utiles. El segudo operador quita puertas etc...
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

//Llamar despu?s de haber intentado mover al personaje!
bool DoomEngine::collisionDetect(int16_t currentEntitySubsector, float x, float y, int& segCollided)
{
    bool col_detected = false;

    Subsector& subsector = m_pMap->getSSec((int)currentEntitySubsector);
    float inter_x, inter_y;

    for (int i = 0; (i < subsector.seg_count); i++)
    {
        Seg& seg = m_pMap->getSeg(subsector.first_segID + i);
        Vertex v = *(seg.vert1);
        Vertex v2 = *(seg.vert2);
        Angle rayAngle;

        for (int j = 0; j < 32; j++)
        {
            rayAngle = Angle((360.0f / 32.0f) * j + m_pPlayer->GetAngle().GetValue());
            if (intersect(v.x, v.y, v2.x, v2.y, x, y, x + rayAngle.getCos(), y + rayAngle.getSin(), inter_x, inter_y))
            {
                float distToFarthestVertex = std::max(dist2Points(v.x, v.y, inter_x, inter_y), dist2Points(v2.x, v2.y, inter_x, inter_y));
                float wallLength = dist2Points(v.x, v.y, v2.x, v2.y);
                if (dist2Points(x, y, inter_x, inter_y) <= COLLISIONDISTANCE && distToFarthestVertex <= wallLength)
                {
                    if (seg.pLinedef->flags & (1 << 0))
                    {
                        segCollided = i;
                        return true;
                    }
                }
            }
        }
    }
    segCollided = -1;
    return col_detected;
}
