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
#include <corecrt_math_defines.h>


Player::Player(int id) : m_PlayerID(id), m_PlayerRotation(90.0f), m_PlayerXPos(), m_PlayerYPos(), m_FOV(90.0f), m_iRotationSpeed(5.0f)
{
}

Player::~Player()
{
}

void Player::SetXPos(int x_pos)
{
	m_PlayerXPos = x_pos;
}

void Player::SetYPos(int y_pos)
{
	m_PlayerYPos = y_pos;
}

void Player::SetAngle(int theta)
{
	m_PlayerRotation = theta;
}

int Player::GetID()
{
	return m_PlayerID;
}

int Player::GetXPos()
{
	return m_PlayerXPos;
}

int Player::GetYPos()
{
	return m_PlayerYPos;
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

bool Player::ClipVertexesInFOV(Vertex& V1, Vertex& V2, Angle& V1Angle, Angle& V2Angle)
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
    V1Angle = V1Angle - m_PlayerRotation;
    V2Angle = V2Angle - m_PlayerRotation;
    Angle HalfFOV = m_FOV / 2;
    // Clipear V1 entre 0 y 90
    Angle V1Moved = V1Angle + HalfFOV;
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
        V1Angle = HalfFOV;
    }
    //Si V2 está fuera del FOV, clipeamos
    Angle V2Moved = HalfFOV - V2Angle;
    if (V2Moved > m_FOV)
    {
        V2Angle = -HalfFOV;
    }
    V1Angle += 90;
    V2Angle += 90;
    return true;
}

void Player::RotateLeft()
{
    m_PlayerRotation += (0.1875f * m_iRotationSpeed);
}

void Player::RotateRight()
{
    m_PlayerRotation -= (0.1875f * m_iRotationSpeed);
}

float Player::getFOV()
{
    return m_FOV;
}
