/*
* Name: Player.cpp
* Date: 11/03/2021 (DD/MM/YYYY)
* Author: V?ctor Mart?nez Lascorz (738845)
* Author: Pedro Jos? P?rez garc?a (756642)
* Coms: Clase que representar? al jugador y su personaje dentro del juego.
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
    m_iMovementSpeed = DOOMGUYWALKINGSPEED;
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

void Player::Init(sf::RenderWindow* r_Window, bool isOnNightmare_)
{
    isOnNightmare = isOnNightmare_;
    //Load shotgun sprite, in this version of the game it?s going to be the only weapon available
    if (!shotgunTexture.loadFromFile("assets/Weapons/Shotgun.png")) {
        std::cout << "Error LOAD SHOTGUN" << std::endl;
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
        shotgunSprite[i].setPosition((SCREENWIDTH / 2.0f) - (shotgunSprite[i].getTextureRect().width * shotgunSprite[i].getScale().x / 1.9f), SCREENHEIGHT / 2.0f - (r_Window->getView().getSize().y * 0.30f));// 0.28f));
    }
    actualSprite = 0;
    
    //Load sounds of player
    if (!shotBuffer.loadFromFile("assets/Music/shotgunShoot.wav")) {
        std::cout << "Error al cargar audio de tiro en player" << std::endl;
    }
    shotgunShoot.setBuffer(shotBuffer);

    if (!harmedBuffer.loadFromFile("assets/Music/playerInjured.wav")) {
        std::cout << "Error al cargar audio de pj herido" << std::endl;
    }
    harmed.setBuffer(harmedBuffer);

    if (!deadBuffer.loadFromFile("assets/Music/playerDead.wav")) {
        std::cout << "Error al cargar audio de pj muerto" << std::endl;
    }
    dead.setBuffer(deadBuffer);

    invencible = false;
}

void Player::Init(sf::RenderWindow* r_Window, int hp_, int armor_, int ammo_, bool isOnNightmare_)
{
    hp = hp_;
    armor = armor_;
    ammo = ammo_;
    isOnNightmare = isOnNightmare_;
    //Load shotgun sprite, in this version of the game it?s going to be the only weapon available
    if (!shotgunTexture.loadFromFile("assets/Weapons/Shotgun.png")) {
        std::cout << "Error LOAD SHOTGUN" << std::endl;
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
        shotgunSprite[i].setPosition((SCREENWIDTH / 2.0f) - (shotgunSprite[i].getTextureRect().width * shotgunSprite[i].getScale().x / 1.9f), SCREENHEIGHT / 2.0f - (r_Window->getView().getSize().y * 0.28f));
    }
     actualSprite = 0;
    
    //Load sounds of player
    if (!shotBuffer.loadFromFile("assets/Music/shotgunShoot.wav")) {
        std::cout << "Error al cargar audio de tiro en player" << std::endl;
    }
    shotgunShoot.setBuffer(shotBuffer);

    if (!harmedBuffer.loadFromFile("assets/Music/playerInjured.wav")) {
        std::cout << "Error al cargar audio de pj herido" << std::endl;
    }
    harmed.setBuffer(harmedBuffer);

    if (!deadBuffer.loadFromFile("assets/Music/playerDead.wav")) {
        std::cout << "Error al cargar audio de pj muerto" << std::endl;
    }
    dead.setBuffer(deadBuffer);
    
    invencible = false;
}

void Player::setVolumenToShoot(float soundLevel) {
    shotgunShoot.setVolume(soundLevel);
    harmed.setVolume(soundLevel*2);
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
void Player::setNightmare(bool nightmare_) {
    isOnNightmare = nightmare_;
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
    //Para encajar los v?rtices en el FOV del jugador y no tener en cuenta lo que no se deba renderizar
    V1Angle = AngleToVertex(V1);
    V2Angle = AngleToVertex(V2);
    Angle V1ToV2Span = V1Angle - V2Angle;
    if (V1ToV2Span >= 180)
    {
        return false;   //Si la l?nea no mira al jugador no tenemos que hacer nada
    }
    // Operamos en el primer cuadrante para simplificar todo
    V1AngleFromPlayer = V1Angle - m_PlayerRotation;
    V2AngleFromPlayer = V2Angle - m_PlayerRotation;
    Angle HalfFOV = m_FOV / 2;
    // Clipear V1 entre 0 y 90
    Angle V1Moved = V1AngleFromPlayer + HalfFOV;
    if (V1Moved > m_FOV)
    {
        //Si V1 est? fuera del FOV (por la izq)
        // Ver si el otro v?rtice est? tambi?n fuera del FOV
        Angle V1MovedAngle = V1Moved - m_FOV;
        if (V1MovedAngle >= V1ToV2Span)
        {
            return false;
        }
        // Hay que clipear V1
        V1AngleFromPlayer = HalfFOV;
    }
    //Si V2 est? fuera del FOV, clipeamos
    Angle V2Moved = HalfFOV - V2AngleFromPlayer;
    if (V2Moved > m_FOV)
    {
        V2AngleFromPlayer = -HalfFOV;
    }
    V1AngleFromPlayer += m_FOV;
    V2AngleFromPlayer += m_FOV;
    return true;
}


bool Player::ClipOneVertexInFOV(Vertex& V1, Angle& V1Angle, Angle& V1AngleFromPlayer)
{
    //Para encajar los v?rtices en el FOV del jugador y no tener en cuenta lo que no se deba renderizar
    V1Angle = AngleToVertex(V1);

    // Operamos en el primer cuadrante para simplificar todo
    V1AngleFromPlayer = V1Angle - m_PlayerRotation;
    Angle HalfFOV = m_FOV / 2;
    // Clipear V1 entre 0 y 90
    Angle V1Moved = V1AngleFromPlayer + HalfFOV;

    //Si esta fuera del FOV no printeamos una mierda
    if (V1Moved > m_FOV)
    {
        return false;
    }
    V1AngleFromPlayer += m_FOV;
    return true;
}


void Player::RotateLeft(const float& deltaTime)
{
    if (!isDead) {
        m_PlayerRotation += (m_iRotationSpeed * deltaTime);
    }
    
}

void Player::RotateRight(const float& deltaTime)
{
    if (!isDead) {
        m_PlayerRotation -= (m_iRotationSpeed * deltaTime);
    }
}

//Movimiento del jugador
void Player::Move(const float& deltaTime)
{
    if (!isDead) {
        if (m_moveForward)
        {
            moveForward(deltaTime);
        }

        if (m_moveBackwards)
        {
            moveBackwards(deltaTime);
        }

        if (m_rotateClockwise)
        {
            RotateRight(deltaTime);
        }

        if (m_rotateAnticlockwise)
        {
            RotateLeft(deltaTime);
        }


    }

}

void Player::moveForward(const float& deltaTime)
{
    m_PlayerXPos += m_PlayerRotation.getCos() * m_iMovementSpeed * deltaTime;
    m_PlayerYPos += m_PlayerRotation.getSin() * m_iMovementSpeed * deltaTime;
}

void Player::moveBackwards(const float& deltaTime)
{
    m_PlayerXPos -= m_PlayerRotation.getCos() * m_iMovementSpeed * deltaTime;
    m_PlayerYPos -= m_PlayerRotation.getSin() * m_iMovementSpeed * deltaTime;
}

void Player::toggleRunning(bool running)
{
    if (running)
    {
        m_iMovementSpeed = DOOMGUYRUNNINGSPEED;
        m_iRotationSpeed = ROTATIONSPEED * 1.5f;
    }
    else
    {
        m_iMovementSpeed = DOOMGUYWALKINGSPEED;
        m_iRotationSpeed = ROTATIONSPEED;
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

int Player::getCurrentSubsector()
{
    return m_currentSubsector;
}

void Player::setCurrentSubsector(int subsector_id)
{
    m_currentSubsector = subsector_id;
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
        return false;
    }
}

void Player::setDead(bool dead_) {
    if (dead_) {
        isDead = true;
        hp = 0;
        armor = 0;
    }
}

void Player::getHitBy(std::string enemigo, int randomNumber) {
    if (!isDead && !invencible) {
        if (enemigo == "soldado") {
            int damageDeal = 30 + randomNumber; //Escopeta de feria que depende del RNG como todo el juego
            if (isOnNightmare) { damageDeal *= 2; }
            if (damageDeal < 8) { damageDeal = 8; }
            int armorDamage = 3*damageDeal / 4;
            int hpDamage = damageDeal - armorDamage;
            if (armor - armorDamage >= 0) {
                armor = armor - armorDamage;
                hp = hp - hpDamage;
                if (hp <= 0) {
                    hp = 0;
                    isDead = true;
                    dead.play();
                }
                else {
                    harmed.play();
                }
            }
            else {
                hp = hp - damageDeal + armor;
                armor = 0;
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
}

//Renderiza el arma del jugador acorde con su estado
void Player::renderPlayer(sf::RenderWindow* m_pRenderWindow, const float& deltaTime) {
    //Mover el sprite de la escopeta si no estamos en la animaci?n de disparar
    if (!isDead) {
        if (!m_isShooting)
        {
            if (isMoving())
            {
                for (int i = 0; i < 4; i++)
                {
                    sf::Vector2f pos = shotgunSprite[i].getPosition();
                    float x_displ = sinf(((2.0f * M_PI) / 100.0f) * m_wpnStep) * 2.0f / (deltaTime * (float)TARGETFRAMERATE);
                    if ((x_displ < 0.0f && firstTimeWpnMovement) || !firstTimeWpnMovement)
                    {
                        if (firstTimeWpnMovement)
                        {
                            firstTimeWpnMovement = false;
                        }
                        x_displ = x_displ * 2.0f;
                    }
                    shotgunSprite[i].setPosition(pos.x + (x_displ * SCREENWIDTH / 2560.0f), pos.y + (sinf(((2.0f * M_PI) / 50.0f) * m_wpnStep) * 3.5f * SCREENWIDTH / 2560.0f) / (deltaTime * (float)TARGETFRAMERATE));
                    //Esto a lo mejor si cambiamos el l?mite de FPS explota
                }
                m_wpnStep = m_wpnStep + 1 % 100;
            }
        }
        m_pRenderWindow->draw(shotgunSprite[actualSprite]); //Pintar en pantalla
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

//TODO QUITAR DESPUES DE LA BETA
void Player::toogleInvencible() {
    invencible = !invencible;
    if (invencible) {
        std::cout << "haha, godmode on ! get rekt ! " << std::endl;
    }
    else
    {
        std::cout << "Godmode desactivado" << std::endl;
    }
}