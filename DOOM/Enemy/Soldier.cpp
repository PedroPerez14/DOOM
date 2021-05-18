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

Soldier::Soldier(int x_, int y_, Player* player_, Map* map_) : Enemy(x_, y_, "Soldier") {
	srand(time(NULL));
	isAwake = false;
	hp = 100;
	player = player_;

	if (!shootBuffer.loadFromFile("../../../../assets/Music/zombieShoot.wav")) {
		std::cout << "Error al cargar audio de zombie disparando" << std::endl;
	}
	shoot.setBuffer(shootBuffer);


	if (!soldierTexture.loadFromFile("../../../../assets/Soldier/FrontSoldier.png")) {
		std::cout << "Error al cargar sprite de zombie Frontsoldier" << std::endl;
	}
	soldierSprite.setTexture(soldierTexture);
	isVisible = false;
	enemyState = EnemyState::await;
	anguloDeVista = 0;
	map = map_;
}

float Soldier::xValue() {
	return x;
}

float Soldier::yValue() {
	return y;
}

bool Soldier::getVisible() {
	return isVisible;
}

void Soldier::setVisible(bool visible) {
	isVisible = visible;
}

Soldier::~Soldier() {
}

void Soldier::move(){

}

void Soldier::getHitByUser(float anguloDisparo) {
	int dispersion = abs(anguloDisparo - 90);	//Valor entre 0 y 10 que indica dispersion
	if (dispersion < 1) {
		hp = 0;
		isDead = true;
	}
	else if (dispersion < 4) {
		hp = hp - 50;
		if (hp <= 0) {
			hp = 0;
			isDead = true;
		}
	}
	else {
		hp = hp - 40;
		if (hp <= 0) {
			hp = 0;
			isDead = true;
		}
	}

}

void Soldier::shooting(int numeroAleatorio) {
	//std::cout << "Intento de tiro a player: " << numeroAleatorio << std::endl;
	shoot.play();
	if (numeroAleatorio > 40 && isVisible) {
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
		/*if (n % 10 == 0) {	// minimo ocada 3 movimientos dispara.
			//shoot
			enemyState = EnemyState::shoot;
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			std::thread dispara(&Soldier::shooting, this, fullRandom % 100);
			dispara.detach();
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
			n = 0;
		}
		else*/ if (randomnumber < 25) {
			//Move left
			anguloDeVista = 180;
			enemyState = EnemyState::moveLeft;
			std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom*2));
		}
		else if (randomnumber < 50) {
			//Move top
			anguloDeVista = 90;
			enemyState = EnemyState::moveTop;
			std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom*2));
		}
		else if (randomnumber < 75) {
			//Move right
			anguloDeVista = 0;
			enemyState = EnemyState::moveRight;
			std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom*2));
		}
		else if (randomnumber < 95) {
			//Move down
			anguloDeVista = 270;
			enemyState = EnemyState::moveDown;
			std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom*2));
		}
		else {
			//shoot
			enemyState = EnemyState::shoot;
			std::this_thread::sleep_for(std::chrono::milliseconds(600));	//300
			std::thread dispara(&Soldier::shooting, this, fullRandom%100);
			dispara.detach();
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));	//700
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
		//float patataAux = sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y));
		//std::cout << "COMPROBACION DE DESPIERTO POR SONIDO " << patataAux << std::endl;
		if (sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y)) < 0) {	//900
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
		//float patataAux = sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y));
		//std::cout << "COMPROBACION DE DESPIERTO POR PASO " << patataAux << std::endl;
		if (sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y)) < 0) {	//900	//Valor minimo de alerta por movimiento
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


void Soldier::renderEnemy(float playerAngle, sf::RenderWindow* m_pRenderWindow) {

	if (!isDead) {
		//3 CALCULOS PARA RENDERIZADO: CONSEGUIR ESCALA, CONSEGUIR EJE X, CONSEGUIR EJE Y:
		int distancia = sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y));	//Calcula la distancia entre el enemigo y el jugador para el tamaño
		float escalado;
		if (distancia > 320) {						//Varias formulas. esta es de distancia 320 a inf
			float diff = 975 - (float)distancia;	//Por cada 75m acercados, aumentamos sprite un 0.1
			escalado = diff / 75 * 0.1;
		}
		else if (distancia > 150) {					//Formula de 15 a 320
			float diff = 320 - (float)distancia;	//Por cada 75m acercados, aumentamos sprite un 0.2
			escalado = diff / 75 * 0.2 + 1;
		}
		else {	//De 0 a 150
			float diff = 150 - (float)distancia;	//Por cada 75m acercados, aumentamos sprite un 0.4
			escalado = diff / 75 * 0.4 + 1.4;
		}

		if (escalado < 0.3) {	//Asignar un mínimo porque se va de madre sino
			escalado = 0.3;
		}
		if (escalado > 2.3) {	//Asignar un máximo porque se va de madre sino
			escalado = 2.3;
		}
		soldierSprite.setScale(escalado, escalado);
		std::cout << "a una distancia de " << distancia << " se obtiene escalado de " << escalado << std::endl;


		//CONSEGUIR EL EJE 'X' SEGUN ANGULO DE VISION
		int posRespectoDivisones = (int)playerAngle - 45; // Valor entre 0 y 90, siendo 0 derecha max y 90 izq max
		posRespectoDivisones = abs(90 - posRespectoDivisones); // Alternamos valores, ahora 0 = izq max y 90 = derecha max
		int posicionRespectoPantalla = (posRespectoDivisones * SCREENWIDTH) / 90 - (soldierTexture.getSize().x / 2) * escalado;	//Regla de 3 para sacar la posicion con respecto a píxeles & centrar el sprite en el enemigo

		//CONSEGUIR EL EJE 'Y' SEGUN ALTURA DE PLAYER Y DEL ENEMIGO
		float alturaEnemigo = player->GetZPos() - (map->getEnemySubsecHeight(xValue(), yValue()) + DOOMGUYEYESPOS);
		//std::cout << player->GetZPos() << " - " << map->getEnemySubsecHeight(xValue(), yValue()) << " = " << alturaEnemigo << std::endl;
		int y = SCREENHEIGHT / 2 - (soldierTexture.getSize().y / 2) * escalado + (alturaEnemigo*escalado*2);
		soldierSprite.setPosition(posicionRespectoPantalla, y);

		//Para finalizar, dibujar en el lugar y escalado adecuado
		m_pRenderWindow->draw(soldierSprite);
	}

	else {
		
	}

}