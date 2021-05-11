/*
* Name: Enemy.h
* Date: 05/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase enemigo: Soldado
*/

#pragma once
#include "Enemy.h"
#include "Soldier.h"
#include "math.h"
#include "../doomdef.h"
#include <stdlib.h>     /* srand, rand, abs */
#include <iostream>
#include <thread>
#include <chrono>
#include "EnemyStates.h"
#include "../Player/Player.h"

Soldier::Soldier(int x_, int y_, Player* player_) : Enemy(x_, y_, "Soldier") {
	srand(time(NULL));
	isAwake = false;
	hp = 100;
	player = player_;

	if (!shootBuffer.loadFromFile("../../../../assets/Music/zombieShoot.wav")) {
		std::cout << "Error al cargar audio de zombie disparando" << std::endl;
	}
	shoot.setBuffer(shootBuffer);
}

float Soldier::xValue() {
	return x;
}

float Soldier::yValue() {
	return y;
}

Soldier::~Soldier() {
}

void Soldier::move(){

}

void Soldier::getHitByUser() {

}

void Soldier::shooting(int numeroAleatorio) {
	std::cout << "Intento de tiro a player: " << numeroAleatorio << std::endl;
	shoot.play();
	if (numeroAleatorio > 40) {
		player->getHitBy("soldado", (numeroAleatorio-85)/2);
	}
}

void Soldier::state(){
	int n = 0;
	while (true) {
		n++;
		int fullRandom = rand() % 100000;		//Sacar 2 numeros aleatorios de 100 y 1000
		int randomnumber = fullRandom % 100;	//Quedarse con el primer XX del aleatorio
		fullRandom = fullRandom / 100;			//Quedarse con el segundo YY aleatorio
		if (n % 3 == 0) {	// minimo ocada 3 movimientos dispara.
			//shoot
			enemyState = EnemyState::shoot;
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			std::thread dispara(&Soldier::shooting, this, fullRandom % 100);
			dispara.detach();
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
			n = 0;
		}
		else if (randomnumber < 20) {
			//Move left
			anguloDeVista = 180;
			enemyState = EnemyState::moveLeft;
			std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom));
		}
		else if (randomnumber < 40) {
			//Move top
			anguloDeVista = 90;
			enemyState = EnemyState::moveTop;
			std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom));
		}
		else if (randomnumber < 60) {
			//Move right
			anguloDeVista = 0;
			enemyState = EnemyState::moveRight;
			std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom));
		}
		else if (randomnumber < 80) {
			//Move down
			anguloDeVista = 270;
			enemyState = EnemyState::moveDown;
			std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom));
		}
		else {
			//shoot
			enemyState = EnemyState::shoot;
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			std::thread dispara(&Soldier::shooting, this, fullRandom%100);
			dispara.detach();
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
			n = 0;
		}

		if (isDead) {
			enemyState = EnemyState::await;
			break;
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(200));	//Seguro de que se quede en await quietecito.
	enemyState = EnemyState::await;
}

void Soldier::playerMakeSound(){
	if (!isAwake && !isDead) {
		if (abs(player->GetXPos()) - abs(x) + abs(player->GetYPos()) - abs(y) > 700) {		//Valor minimo de alerta por sonido
			isAwake = true;
			std::thread dispara(&Soldier::state, this);
			dispara.detach();
			//Iniciar proceso de cambio de sprite
			std::cout << "Enemigo despertado por disparo" << std::endl;
		}
	}
}

void Soldier::playerMove() {
	if (!isAwake && !isDead) {
		if (abs(player->GetXPos()) - abs(x) + abs(player->GetYPos()) - abs(y) > 100) {		//Valor minimo de alerta por movimiento
			isAwake = true;
			std::thread dispara(&Soldier::state, this);
			dispara.detach();

			std::cout << "Enemigo despertado por cercania" << std::endl;
			//Iniciar proceso de cambio de sprite
		}
	}
}

//Calcula la siguiente posicion del enemigo
void Soldier::nextMove(){
	if (isAwake && !isDead) {
		switch (enemyState)
		{
		case EnemyState::moveLeft:
			x = x - 1.0f;
			break;
		case EnemyState::moveRight:
			x = x + 1.0f;
			break;
		case EnemyState::moveTop:
			y = y + 1.0f;
			break;
		case EnemyState::moveDown:
			y = y - 1.0f;
			break;
		default:
			break;
		}
	}
}
