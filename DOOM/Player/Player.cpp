/*
* Name: Player.cpp
* Date: 11/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que representará al jugador y su personaje dentro del juego.
*/

#pragma once
#include "Player.h"
#include "math.h"
#include "../doomdef.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <corecrt_math_defines.h>
#include "iostream"
#include <thread>
#include <chrono>


Player::Player(int id) : m_PlayerID(id), m_PlayerRotation(90.0f), m_PlayerXPos(0), m_PlayerYPos(0), m_FOV(90.0f), m_iRotationSpeed(ROTATIONSPEED)
{
    m_PlayerZPos = DOOMGUYEYESPOS;          //41 en el juego original
    m_iMovementSpeed = DOOMGUYWALKINGSPEED; //TODO provisional, luego habrá que inicializar a 0
    m_frontalThrust = 0.0f;
    m_moveForward = false;
    m_moveBackwards = false;
    m_rotateClockwise = false;
    m_rotateAnticlockwise = false;
    m_isRunning = false;
    m_isShooting = false;
    m_wpnStep = 0;
    firstTimeWpnMovement = true;

    hp = 200;
    armor = 200;
    ammo = 200;
    canShoot = true;
    isDead = false;
}

Player::~Player()
{
}

void Player::Init(sf::RenderWindow* r_Window)
{
    std::cout << "entrando en init" << std::endl;
    //Load shotgun sprite, in this version of the game it´s going to be the only weapon available
    if (!shotgunTexture.loadFromFile("../../../../assets/Weapons/Shotgun.png")) {
        std::cout << "ERROR LOAD SHOTGUN" << std::endl;
    }
    for (int i = 0; i < 4; i++) {
        shotgunSprite[i].setTexture(shotgunTexture);
    }
    shotgunSprite[0].setTextureRect(sf::IntRect(0, 0, 145, 275));
    shotgunSprite[1].setTextureRect(sf::IntRect(145, 0, 218, 275));
    shotgunSprite[2].setTextureRect(sf::IntRect(363, 0, 162, 275));
    shotgunSprite[3].setTextureRect(sf::IntRect(525, 0, 200, 275));

    for (int i = 0; i < 4; i++) {
        shotgunSprite[i].setScale(0.5f * (SCREENWIDTH / 320), 0.5f * (SCREENWIDTH / 320));
        shotgunSprite[i].setPosition((r_Window->getView().getSize().x / 2.0f) - (shotgunSprite[i].getTextureRect().width * shotgunSprite[i].getScale().x / 1.9f), r_Window->getSize().y / 2.0f - (r_Window->getView().getSize().y * 0.28f));
    }
    actualSprite = 0;
    std::cout << "inicio zona carga disparo" << std::endl;

    //Load sounds of player
    if (!shotBuffer.loadFromFile("../../../../assets/Music/shotgunShoot.wav")) {
        std::cout << "Error al cargar audio de tiro en player" << std::endl;
    }
    shotgunShoot.setBuffer(shotBuffer);

    if (!harmedBuffer.loadFromFile("../../../../assets/Music/playerInjured.wav")) {
        std::cout << "Error al cargar audio de pj herido" << std::endl;
    }
    harmed.setBuffer(harmedBuffer);

    if (!deadBuffer.loadFromFile("../../../../assets/Music/playerDead.wav")) {
        std::cout << "Error al cargar audio de pj muerto" << std::endl;
    }
    dead.setBuffer(deadBuffer);
    
}

void Player::setVolumenToShoot(float soundLevel) {
    //std::cout << "inicializado shotgun a volumen:" << soundLevel << std::endl;
    shotgunShoot.setVolume(soundLevel);
    harmed.setVolume(soundLevel);
    dead.setVolume(soundLevel);
}

void Player::SetXPos(float x_pos)
{
    m_PlayerXPos = x_pos;
}

void Player::SetYPos(float y_pos)
{
    m_PlayerYPos = y_pos;
}

void Player::SetZPos(float z_pos)
{
    m_PlayerZPos = z_pos + (float)DOOMGUYEYESPOS;
}

void Player::SetAngle(int theta)
{
    m_PlayerRotation = theta;
}

int Player::GetID()
{
    return m_PlayerID;
}

float Player::GetXPos()
{
    return m_PlayerXPos;
}

float Player::GetYPos()
{
    return m_PlayerYPos;
}

float Player::GetZPos()
{
    return m_PlayerZPos;
}

Angle Player::GetAngle()
{
    return m_PlayerRotation;
}

Angle Player::AngleToVertex(Vertex& vertex)
{
    float Vdx = vertex.x - m_PlayerXPos;
    float Vdy = vertex.y - m_PlayerYPos;

    Angle VertexAngle(atan2f(Vdy, Vdx) * 180.0f / (float)M_PI);

    return VertexAngle;
}

bool Player::ClipVertexesInFOV(Vertex& V1, Vertex& V2, Angle& V1Angle, Angle& V2Angle, Angle& V1AngleFromPlayer, Angle& V2AngleFromPlayer)
{
    //Para encajar los vértices en el FOV del jugador y no tener en cuenta lo que no se deba renderizar
    V1Angle = AngleToVertex(V1);
    V2Angle = AngleToVertex(V2);
    Angle V1ToV2Span = V1Angle - V2Angle;
    if (V1ToV2Span >= 180)
    {
        return false;   //Si la línea no mira al jugador no tenemos que hacer nada
    }
    // Operamos en el primer cuadrante para simplificar todo
    //  no entendí muy bien estas operaciones pero tengo fe en ellas
    V1AngleFromPlayer = V1Angle - m_PlayerRotation;
    V2AngleFromPlayer = V2Angle - m_PlayerRotation;
    Angle HalfFOV = m_FOV / 2;
    // Clipear V1 entre 0 y 90
    Angle V1Moved = V1AngleFromPlayer + HalfFOV;
    if (V1Moved > m_FOV)
    {
        //Si V1 está fuera del FOV (por la izq)
        // Ver si el otro vértice está también fuera del FOV
        Angle V1MovedAngle = V1Moved - m_FOV;
        if (V1MovedAngle >= V1ToV2Span)
        {
            return false;
        }
        // Hay que clipear V1
        V1AngleFromPlayer = HalfFOV;
    }
    //Si V2 está fuera del FOV, clipeamos
    Angle V2Moved = HalfFOV - V2AngleFromPlayer;
    if (V2Moved > m_FOV)
    {
        V2AngleFromPlayer = -HalfFOV;
    }
    V1AngleFromPlayer += m_FOV;
    V2AngleFromPlayer += m_FOV;
    return true;
}

void Player::RotateLeft()
{
    if (!isDead) {
        m_PlayerRotation += (m_iRotationSpeed / (float)TARGETFRAMERATE);
    }
    
}

void Player::RotateRight()
{
    if (!isDead) {
        m_PlayerRotation -= (m_iRotationSpeed / (float)TARGETFRAMERATE);
    }
}

//Movimiento del jugador
void Player::Move()
{
    if (!isDead) {
        //Código provisional
        if (m_moveForward)
        {
            moveForward();
        }

        if (m_moveBackwards)
        {
            moveBackwards();
        }

        if (m_rotateClockwise)
        {
            RotateRight();
        }

        if (m_rotateAnticlockwise)
        {
            RotateLeft();
        }

        //TODO
        //applyThrust();
        //clipSpeed();
        //getNewPosition();
        //applyFriction();
    }

}

void Player::moveForward()
{
    m_PlayerXPos += m_PlayerRotation.getCos() * m_iMovementSpeed / (float)TARGETFRAMERATE;
    m_PlayerYPos += m_PlayerRotation.getSin() * m_iMovementSpeed / (float)TARGETFRAMERATE;
}

void Player::moveBackwards()
{
    m_PlayerXPos -= m_PlayerRotation.getCos() * m_iMovementSpeed / (float)TARGETFRAMERATE;
    m_PlayerYPos -= m_PlayerRotation.getSin() * m_iMovementSpeed / (float)TARGETFRAMERATE;
}

void Player::toggleRunning(bool running)
{
    if (running)
    {
        m_iMovementSpeed = DOOMGUYRUNNINGSPEED;
    }
    else
    {
        m_iMovementSpeed = DOOMGUYWALKINGSPEED;
    }
    m_isRunning = running;
}

void Player::toggleMoveForward(bool move)
{
    m_moveForward = move;
}

void Player::toggleMoveBackwards(bool move)
{
    m_moveBackwards = move;
}

void Player::toggleRotateClockwise(bool rotate)
{
    m_rotateClockwise = rotate;
}

void Player::toggleRotateAnticlockwise(bool rotate)
{
    m_rotateAnticlockwise = rotate;
}

void Player::moveUpwards()
{
    m_PlayerZPos += 1.0f;
}

void Player::moveDownwards()
{
    m_PlayerZPos -= 1.0f;
}


float Player::getFOV()
{
    return m_FOV;
}

float Player::distanceToEdge(Vertex& V)
{
    return sqrtf(powf((float)(m_PlayerXPos - (int)V.x), 2.0f) + powf((float)(m_PlayerYPos - (int)V.y), 2.0f));
}

bool Player::isRunning()
{
    return m_isRunning;
}

bool Player::isMoving()
{
    //Una u otra pero no las dos
    return (m_moveForward || m_moveBackwards) && !(m_moveForward && m_moveBackwards);
}

//Analiza todas las interacciones cuando el jugador dispara la escopeta
bool Player::shoot() {
    if (canShoot && ammo > 0 && !isDead) {
        shotgunShoot.play();
        m_isShooting = true;
        canShoot = false;
        ammo--;
        std::thread asdas(&Player::timerauxiliar, this);
        asdas.detach();
        return true;
    }
    else {
        //std::cout << "Disparando sin recargar" << std::endl;
        return false;
    }
}

void Player::getHitBy(std::string enemigo, int randomNumber) {
    
    if (enemigo == "soldado") {
        int damageDeal = 30 + randomNumber; //30 +-8
        if (armor - damageDeal >= 0) {
            armor = armor - damageDeal;
            harmed.play();
        }
        else {
            damageDeal = damageDeal - armor;
            armor = 0;
            hp = hp - damageDeal;
            if (hp <= 0) {
                hp = 0;
                isDead = true;
                dead.play();
            }
            else {
                harmed.play();
            }
        }
    }
}

bool Player::checkDead() {
    return isDead;
}

//Inicia un contador para que el sprite del arma avance por las 4 fases
void Player::timerauxiliar() {
    actualSprite = 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    actualSprite = 2;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    actualSprite = 3;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    actualSprite = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    m_isShooting = false;
    canShoot = true;
    std::cout << "Vuelvo del thread" << std::endl;
}

//Renderiza el arma del jugador acorde con su estado
void Player::renderPlayer(sf::RenderWindow* m_pRenderWindow) {
    //Mover el sprite de la escopeta si no estamos en la animación de disparar
    //std::cout << "angulo = " << m_PlayerRotation.GetValue() << ", x = "<< GetXPos() <<", y = " << GetYPos() <<std::endl;
    if (!isDead) {
        if (!m_isShooting)
        {
            if (isMoving())
            {
                for (int i = 0; i < 4; i++)
                {
                    sf::Vector2f pos = shotgunSprite[i].getPosition();
                    float x_displ = sin(((2.0f * M_PI) / 100.0f) * m_wpnStep) * 2.0f;
                    if ((x_displ < 0.0f && firstTimeWpnMovement) || !firstTimeWpnMovement)
                    {
                        if (firstTimeWpnMovement)
                        {
                            firstTimeWpnMovement = false;
                        }
                        x_displ = x_displ * 2.0f;
                    }
                    shotgunSprite[i].setPosition(pos.x + (x_displ * SCREENWIDTH / 2560.0f), pos.y + (sin(((2.0f * M_PI) / 50.0f) * m_wpnStep) * 3.5f * SCREENWIDTH / 2560.0f));
                }
                m_wpnStep = m_wpnStep + 1 % 100;
            }
        }
        m_pRenderWindow->draw(shotgunSprite[actualSprite]); //Pintar en pantalla
    }
    else {
        //TODO bajar la z hasta el suelo y cancelar el movimiento arriba/abajo
    }
    
}

int Player::getAmmo() {
    return ammo;
}

int Player::getArmor() {
    return armor;
}

int Player::getHp() {
    return hp;
}