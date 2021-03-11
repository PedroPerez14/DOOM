/*
* Name: Player.cpp
* Date: 11/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que representará al jugador y su personaje dentro del juego.
*/

#pragma once
#include "Player.h"


Player::Player(int id) : m_PlayerID(id)
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

int Player::GetAngle()
{
	return m_PlayerRotation;
}