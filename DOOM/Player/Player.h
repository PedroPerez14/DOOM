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
#include <SFML/Audio.hpp>

class Player
{
public:

	Player(int id);		//id is only necessary for multiplayer
	~Player();

	void Init(sf::RenderWindow* r_Window);
	void setVolumenToShoot(float soundLevel);

	void SetXPos(float x_pos);
	void SetYPos(float y_pos);
	void SetZPos(float z_pos);
	void SetAngle(int theta);

	int GetID();
	float GetXPos();
	float GetYPos();
	float GetZPos();
	Angle GetAngle();

	Angle AngleToVertex(Vertex& vertex);
	bool ClipVertexesInFOV(Vertex& V1, Vertex& V2, Angle& V1Angle, Angle& V2Angle, Angle& V1AngleFromPlayer, Angle& V2AngleFromPlayer);

	void Move(const float& deltaTime);

	// Puede que estas funciones dejen de hacerme falta //
	void RotateLeft(const float& deltaTime);	//WIP, debug
	void RotateRight(const float& deltaTime);	//WIP
	void moveForward(const float& deltaTime);
	void moveBackwards(const float& deltaTime);
	void moveUpwards();
	void moveDownwards();
	// ----------------------------------------------- //


	void toggleRunning(bool running);
	void toggleMoveForward(bool move);
	void toggleMoveBackwards(bool move);
	void toggleRotateClockwise(bool rotate);
	void toggleRotateAnticlockwise(bool rotate);

	bool isRunning();
	bool isMoving();

	float getFOV();		//Ret. fov value
	float distanceToEdge(Vertex& V);	//Distancia a un v�rtice

	bool shoot();
	void getHitBy(std::string enemigo, int randomNumber);
	bool checkDead();
	void timerauxiliar();
	void renderPlayer(sf::RenderWindow* m_pRenderWindow, const float& deltaTime);

	//Poder obtener estadisticas para el hud
	int getAmmo();
	int getArmor();
	int getHp();

protected:
	int m_PlayerID;
	float m_PlayerXPos;
	float m_PlayerYPos;
	float m_PlayerZPos;
	Angle m_PlayerRotation;
	float m_FOV;			//FOV del jugador, se inicializar� por defecto a 90 grados

	float m_frontalThrust;	//Aceleraci�n, como no hay movimiento hacia los lados como tal, solo necesito una variable
	float m_iRotationSpeed;
	float m_iMovementSpeed;

	bool m_moveForward;
	bool m_moveBackwards;
	bool m_rotateClockwise;
	bool m_rotateAnticlockwise;
	bool m_isRunning;
	bool m_isShooting;

	//Estadisticas del player
	int hp;
	int armor;
	int ammo;
	bool canShoot;
	bool isDead;

	//Textura y sprites de animacion de arma
	sf::Texture shotgunTexture;
	sf::Sprite shotgunSprite[4];
	sf::SoundBuffer shotBuffer;
	sf::Sound shotgunShoot;	
	sf::SoundBuffer harmedBuffer;
	sf::Sound harmed;
	sf::SoundBuffer deadBuffer;
	sf::Sound dead;
	int actualSprite;
	int m_wpnStep;				//Se usa para calcular el movimiento del sprite del arma al andar
	bool firstTimeWpnMovement;	//Lo mismo
};