/*
* Name: Enemy.h
* Date: 05/04/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
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
#include "../Game/GameStates.h"

Soldier::Soldier(int x_, int y_, Player* player_, Map* map_, Status* thisStatus, int tipoSonido) : Enemy(x_, y_, "Soldier") {
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
	estadoJuego = thisStatus;

	if (!injuredBuffer.loadFromFile("../../../../assets/Music/SoldierInjured.wav")) {
		std::cout << "Error al cargar audio de herida soldier" << std::endl;
	}
	injuredSound.setBuffer(injuredBuffer);
	switch (tipoSonido){
	case 0:
		if (!awakeBuffer.loadFromFile("../../../../assets/Music/SoldierDespierta1.wav")) {
			std::cout << "Error al cargar audio de awake 1 soldier" << std::endl;
		}
		awakeSound.setBuffer(awakeBuffer);
		if (!deathBuffer.loadFromFile("../../../../assets/Music/SoldierMuere1.wav")) {
			std::cout << "Error al cargar audio de dead 1 soldier" << std::endl;
		}
		deathSound.setBuffer(deathBuffer);
		break;
	case 1:
		if (!awakeBuffer.loadFromFile("../../../../assets/Music/SoldierDespierta2.wav")) {
			std::cout << "Error al cargar audio de awake 1 soldier" << std::endl;
		}
		awakeSound.setBuffer(awakeBuffer);
		if (!deathBuffer.loadFromFile("../../../../assets/Music/SoldierMuere2.wav")) {
			std::cout << "Error al cargar audio de dead 1 soldier" << std::endl;
		}
		deathSound.setBuffer(deathBuffer);
		break;
	case 2:
		if (!awakeBuffer.loadFromFile("../../../../assets/Music/SoldierDespierta3.wav")) {
			std::cout << "Error al cargar audio de awake 1 soldier" << std::endl;
		}
		awakeSound.setBuffer(awakeBuffer);
		if (!deathBuffer.loadFromFile("../../../../assets/Music/SoldierMuere3.wav")) {
			std::cout << "Error al cargar audio de dead 1 soldier" << std::endl;
		}
		deathSound.setBuffer(deathBuffer);
		break;
	default:
		break;
	}

}

void Soldier::changeVolumenes(int soundLevel) {
	injuredSound.setVolume(soundLevel * 0.4);
	awakeSound.setVolume(soundLevel * 0.7);
	deathSound.setVolume(soundLevel * 0.7);
	shoot.setVolume(soundLevel);
}

bool Soldier::operator < (Soldier& s) {
	return (getDistToPlayer() < s.getDistToPlayer());
}

bool Soldier::operator > (Soldier& s) {
	return (getDistToPlayer() > s.getDistToPlayer());
}

bool Soldier::operator <= (Soldier& s) {
	return (getDistToPlayer() <= s.getDistToPlayer());
}

bool Soldier::operator >= (Soldier& s) {
	return (getDistToPlayer() >= s.getDistToPlayer());
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

float Soldier::getDistToPlayer() {
	return sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y));
}

void Soldier::getHitByUser(float anguloDisparo) {
	int dispersion = abs(anguloDisparo - 90);	//Valor entre 0 y 10 que indica dispersion
	if (dispersion < 1) {
		hp = 0;
		isDead = true;
		deathSound.play();
	}
	else if (dispersion < 4) {
		hp = hp - 50;
		if (hp <= 0) {
			hp = 0;
			isDead = true;
			deathSound.play();
		}
		else {
			injuredSound.play();
		}
	}
	else {
		hp = hp - 40;
		if (hp <= 0) {
			hp = 0;
			isDead = true;
			deathSound.play();
		}
		else {
			injuredSound.play();
		}
	}

}

void Soldier::shooting(int numeroAleatorio) {
	if (*estadoJuego == Status::ePLAYING) {
		//std::cout << "Intento de tiro a player: " << numeroAleatorio << std::endl;
		shoot.play();
		if (numeroAleatorio > 40 && isVisible) {
			player->getHitBy("soldado", (numeroAleatorio - 85) / 2);
		}
	}
}

void Soldier::state(){
	int n = 0;
	srand(283 * std::hash<std::thread::id>{}(std::this_thread::get_id()));
	while (true) {
		n++;
		int fullRandom = rand() % 100000;		//Sacar 2 numeros aleatorios de 100 y 1000
		int randomnumber = fullRandom % 100;	//Quedarse con el primer XX del aleatorio
		fullRandom = fullRandom / 100;			//Quedarse con el segundo YY aleatorio
		if (n % 10 == 0 && isVisible) {	// minimo ocada 3 movimientos dispara.
			//shoot
			enemyState = EnemyState::shoot;
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			std::thread dispara(&Soldier::shooting, this, fullRandom % 100);
			dispara.detach();
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
			n = 0;
		}
		else {	//No le toca disparar. Pensar movimiento mejor que dar vueltas cual autista
			if (randomnumber < 21) {
				//Move left
				anguloDeVista = 180;
				enemyState = EnemyState::moveLeft;
				std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom * 2));
			}
			else if (randomnumber < 42) {
				//Move top
				anguloDeVista = 90;
				enemyState = EnemyState::moveTop;
				std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom * 2));
			}
			else if (randomnumber < 63) {
				//Move right
				anguloDeVista = 0;
				enemyState = EnemyState::moveRight;
				std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom * 2));
			}
			else if (randomnumber < 84) {
				//Move down
				anguloDeVista = 270;
				enemyState = EnemyState::moveDown;
				std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom * 2));
			}
			else if (isVisible) {
				//shoot
				enemyState = EnemyState::shoot;
				std::this_thread::sleep_for(std::chrono::milliseconds(400));	//300
				std::thread dispara(&Soldier::shooting, this, fullRandom % 100);
				dispara.detach();
				std::this_thread::sleep_for(std::chrono::milliseconds(800));	//700
				n = 0;
			}
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
		if (sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y)) < 800) {	//900
			isAwake = true;
			std::thread soldierState(&Soldier::state, this);
			soldierState.detach();
			awakeSound.play();
		}
	}
}

bool Soldier::isReallyDead() {
	return isDead;
}

void Soldier::setDead(bool dead_) {
	isDead = dead_;
}

void Soldier::playerMove() {
	if (!isAwake && !isDead) {
		//float patataAux = sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y));
		//std::cout << "COMPROBACION DE DESPIERTO POR PASO " << patataAux << std::endl;
		if (sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y)) < 200) {	//200
			isAwake = true;
			std::thread soldierState(&Soldier::state, this);
			soldierState.detach();
			awakeSound.play();
		}
	}
}

//Calcula la siguiente posicion del enemigo
void Soldier::nextMove(){
	if (isAwake && !isDead) {
		switch (enemyState)
		{
		case EnemyState::moveLeft:
			x = x - 2.0f;
			break;
		case EnemyState::moveRight:
			x = x + 2.0f;
			break;
		case EnemyState::moveTop:
			y = y + 2.0f;
			break;
		case EnemyState::moveDown:
			y = y - 2.0f;
			break;
		default:
			break;
		}
	}
}


void Soldier::renderEnemy(float playerAngle, sf::RenderWindow* m_pRenderWindow) {

	if (!isDead) {
		//3 CALCULOS PARA RENDERIZADO: CONSEGUIR ESCALA, CONSEGUIR EJE X, CONSEGUIR EJE Y:
		int distancia = sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y));	//Calcula la distancia entre el enemigo y el jugador para el tama�o
		float escalado;
		if (distancia > 320) {						//Varias formulas. esta es de distancia 320 a inf
			float diff = 975 - (float)distancia;	//Por cada 75m acercados, aumentamos sprite un 0.1
			escalado = diff / 75 * 0.05;	//0.1
		}
		else if (distancia > 150) {					//Formula de 15 a 320
			float diff = 320 - (float)distancia;	//Por cada 75m acercados, aumentamos sprite un 0.2
			escalado = diff / 75 * 0.1 + 0.5;	//0.2 y 1
		}
		else {	//De 0 a 150
			float diff = 150 - (float)distancia;	//Por cada 75m acercados, aumentamos sprite un 0.4
			escalado = diff / 75 * 0.25 + 0.75;	//0.4 y 1,4
		}

		if (escalado < 0.15) {	//Asignar un m�nimo porque se va de madre sino
			escalado = 0.15;	//0.3
		}
		if (escalado > 1.2) {	//Asignar un m�ximo porque se va de madre sino
			escalado = 1.2;		//2.4
		}
		soldierSprite.setScale(escalado, escalado);
		//std::cout << "a una distancia de " << distancia << " se obtiene escalado de " << escalado << std::endl;


		//CONSEGUIR EL EJE 'X' SEGUN ANGULO DE VISION
		int posRespectoDivisones = (int)playerAngle - 45; // Valor entre 0 y 90, siendo 0 derecha max y 90 izq max
		posRespectoDivisones = abs(90 - posRespectoDivisones); // Alternamos valores, ahora 0 = izq max y 90 = derecha max
		int posicionRespectoPantalla = (posRespectoDivisones * SCREENWIDTH) / 90 - (soldierTexture.getSize().x / 2) * (escalado*1.1);	//Regla de 3 para sacar la posicion con respecto a p�xeles & centrar el sprite en el enemigo

		//CONSEGUIR EL EJE 'Y' SEGUN ALTURA DE PLAYER Y DEL ENEMIGO
		float alturaEnemigo = player->GetZPos() - (map->getEnemySubsecHeight(xValue(), yValue()) + DOOMGUYEYESPOS);	//Cuando es negativa hay que subir y viceversa 
		//std::cout << player->GetZPos() << " - " << map->getEnemySubsecHeight(xValue(), yValue()) << " = " << alturaEnemigo << std::endl;
		int y = SCREENHEIGHT / 2 - (soldierTexture.getSize().y * escalado /2) +(alturaEnemigo * escalado * 1.5) + abs(alturaEnemigo*0.05); //Mitad de pantalla, subiendo(-) el sprite segun su tama�o y luego la altura en la que esta (funciona de forma inversa)
		soldierSprite.setPosition(posicionRespectoPantalla, y);

		//Para finalizar, dibujar en el lugar y escalado adecuado
		m_pRenderWindow->draw(soldierSprite);
	}

	else {
		
	}

}