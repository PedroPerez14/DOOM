/*
* Name: Player.h
* Date: 11/03/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase que representar� al jugador y su personaje dentro del juego.
*/

#pragma once
#include "../maps/map_types.h"
#include "../Misc/Angle.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "SFML/Graphics.hpp"

class Player
{
public:

	Player(int id);		//id is only necessary for multiplayer
	~Player();

	void SetXPos(int x_pos);
	void SetYPos(int y_pos);
	void SetZPos(float z_pos);
	void SetAngle(int theta);

	int GetID();
	int GetXPos();
	int GetYPos();
	float GetZPos();
	Angle GetAngle();

	Angle AngleToVertex(Vertex& vertex);
	bool ClipVertexesInFOV(Vertex& V1, Vertex& V2, Angle& V1Angle, Angle& V2Angle, Angle& V1AngleFromPlayer, Angle& V2AngleFromPlayer);

	void RotateLeft();	//WIP, debug
	void RotateRight();	//WIP
	void moveForward();
	void moveBackwards();
	void moveUpwards();
	void moveDownwards();

	float getFOV();		//Ret. fov value
	float distanceToEdge(Vertex& V);	//Distancia a un v�rtice

	void shoot();
	void timerauxiliar();
	void renderPlayer(sf::RenderWindow* m_pRenderWindow);

protected:
	int m_PlayerID;
	int m_PlayerXPos;
	int m_PlayerYPos;
	float m_PlayerZPos;
	Angle m_PlayerRotation;
	float m_FOV;			//FOV del jugador, se inicializar� por defecto a 90 grados
	float m_iRotationSpeed;
	float m_iMovementSpeed;	
	//Hace falta la z para el futuro? 

	//Estadisticas del player
	int hp;
	int armor;
	int ammo;
	bool canShoot;

	//Textura y sprites de animacion de arma
	sf::Texture shotgunTexture;
	sf::Sprite shotgunSprite[4];
	int actualSprite;
};