/*
* Name: Player.h
* Date: 11/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que representará al jugador y su personaje dentro del juego.
*/

#pragma once

class Player
{
public:

	Player(int id);		//id is only necessary for multiplayer
	~Player();

	void SetXPos(int x_pos);
	void SetYPos(int y_pos);
	void SetAngle(int theta);

	int GetID();
	int GetXPos();
	int GetYPos();
	int GetAngle();		//Degrees or radians? //TODO

protected:
	int m_PlayerID;
	int m_PlayerXPos;
	int m_PlayerYPos;
	int m_PlayerRotation;
	//Hace falta la z para el futuro?
};