/*
* Name: Enemy.h
* Date: 05/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase generica que engloba todos los enemigos del juego
*/

#pragma once
#include "Enemy.h"
#include "math.h"
#include "../doomdef.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <corecrt_math_defines.h>
#include <iostream>
#include <thread>
#include <chrono>

Enemy::Enemy() {}

Enemy::Enemy(int x_, int y_, std::string enemyName, Player* player_) {
	nameEnemy = enemyName;
	player = player_;
	x = x_;
	y = y_;
	isAwake = false;
	isDead = false;
}

Enemy::~Enemy() {
}

std::string Enemy::tipeEnemy() {
	return nameEnemy;
}

void Enemy::setAwake(bool awake) {
	isAwake = awake;
}

bool Enemy::getAwake() {
	return isAwake;
}