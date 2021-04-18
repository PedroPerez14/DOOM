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
#include <corecrt_math_defines.h>


Player::Player(int id) : m_PlayerID(id), m_PlayerRotation(90.0f), m_PlayerXPos(0), m_PlayerYPos(0), m_FOV(90.0f), m_iRotationSpeed(DOOMGUYRUNNINGSPEED)
{
    m_PlayerZPos = DOOMGUYEYESPOS;  //41 en el juego original
    m_iMovementSpeed = 0.0f;
    m_frontalThrust = 0.0f;
    m_moveForward = false;
    m_moveBackwards = false;
    m_rotateClockwise = false;
    m_rotateAnticlockwise = false;
}

Player::~Player()
{
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
    m_PlayerRotation += (m_iRotationSpeed / (float)TARGETFRAMERATE);
}

void Player::RotateRight()
{
    m_PlayerRotation -= (m_iRotationSpeed / (float)TARGETFRAMERATE);
}

//Movimiento del jugador
void Player::Move()
{
    //TODO
    applyThrust();
    clipSpeed();
    getNewPosition();
    applyFriction();
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
