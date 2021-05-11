/*
* Name: Enemy.h
* Date: 05/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase generica que engloba todos los enemigos del juego
*/

#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "../Engine/DoomEngine.h"
#include "../Player/Player.h"

class Enemy {
	public:
		Enemy();
		Enemy(int x_,int y_, std::string enemyName, Player* player_);
		~Enemy();
		std::string tipeEnemy();

		void setAwake(bool awake);
		bool getAwake();

		virtual void shooting(int numeroRandom) = 0;
		virtual void move() = 0;
		virtual void playerMakeSound() = 0;
		virtual void playerMove() = 0;
		virtual void state() = 0;
		virtual void nextMove() = 0;
		virtual void getHitByUser() = 0;

	//protected:
		bool isAwake;
		bool isDead;
		float x;
		float y;
		int hp;
		std::string nameEnemy;
		Player* player;

};