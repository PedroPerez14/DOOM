/*
* Name: Enemy.h
* Date: 05/04/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase enemigo: Soldado
*/

#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "../Engine/DoomEngine.h"
#include "Enemy.h"
#include "EnemyStates.h"

class Soldier : Enemy {
public:
	Soldier(int x, int y, Player* player_);
	~Soldier();

	float xValue();
	float yValue();
	void shooting(int numeroRandom) override;
	void move() override;
	void state() override;
	void playerMakeSound() override;
	void playerMove() override;
	void nextMove() override;
	void getHitByUser() override;

protected:
	//Incluidas x, y isAwake
	EnemyState enemyState;
	int anguloDeVista;
};