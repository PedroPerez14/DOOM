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

	isVisible = false;
	enemyState = EnemyState::await;
	anguloDeVista = 180;
	map = map_;
	nSprite = 0;
	estadoJuego = thisStatus;
	isDead = false;
	onFireShooting = false;
	

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

	//Load texture 
	if (!soldierTexture.loadFromFile("../../../../assets/Soldier/SoldierSprites.png")) {
		std::cout << "Error al cargar sprite de zombie Frontsoldier" << std::endl;
	}

	//Set texture to sprites
	for (int i = 0; i < 4; i++) {
		soldierLeftSprite[i].setTexture(soldierTexture);
		soldierRightSprite[i].setTexture(soldierTexture);
		soldierTopSprite[i].setTexture(soldierTexture);
		soldierDeadAnimationSprite[i].setTexture(soldierTexture);

	}
	soldierInjuredTopSprite.setTexture(soldierTexture);
	soldierInjuredRightSprite.setTexture(soldierTexture);
	soldierInjuredLeftSprite.setTexture(soldierTexture);
	soldierDeadSprite.setTexture(soldierTexture);
	soldierShootingSprite[0].setTexture(soldierTexture);
	soldierShootingSprite[1].setTexture(soldierTexture);

	//Apply the correct textureRect	
	soldierTopSprite[0].setTextureRect(sf::IntRect(0, 0, 45, 57));
	soldierTopSprite[1].setTextureRect(sf::IntRect(53, 0, 38, 57));	
	soldierTopSprite[2].setTextureRect(sf::IntRect(100, 0, 40, 57));
	soldierTopSprite[3].setTextureRect(sf::IntRect(145, 0, 41, 57));

	soldierRightSprite[0].setTextureRect(sf::IntRect(0, 63, 47, 57));
	soldierRightSprite[1].setTextureRect(sf::IntRect(49, 63, 36, 57));
	soldierRightSprite[2].setTextureRect(sf::IntRect(97, 63, 39, 57));
	soldierRightSprite[3].setTextureRect(sf::IntRect(144, 63, 36, 57));

	soldierLeftSprite[0].setTextureRect(sf::IntRect(0, 126, 43, 57));
	soldierLeftSprite[1].setTextureRect(sf::IntRect(57, 126, 37, 57));	
	soldierLeftSprite[2].setTextureRect(sf::IntRect(100, 126, 38, 57));
	soldierLeftSprite[3].setTextureRect(sf::IntRect(146, 126, 36, 57));

	soldierDeadAnimationSprite[0].setTextureRect(sf::IntRect(353, 0, 40, 56));
	soldierDeadAnimationSprite[1].setTextureRect(sf::IntRect(395, 0, 38, 56));
	soldierDeadAnimationSprite[2].setTextureRect(sf::IntRect(433, 0, 46, 56));
	soldierDeadAnimationSprite[3].setTextureRect(sf::IntRect(481, 0, 49, 56));

	soldierShootingSprite[0].setTextureRect(sf::IntRect(200, 0, 26, 56));
	soldierShootingSprite[1].setTextureRect(sf::IntRect(255, 0, 28, 56));
	soldierInjuredTopSprite.setTextureRect(sf::IntRect(306, 0, 40, 56));
	soldierInjuredRightSprite.setTextureRect(sf::IntRect(306, 64, 44, 56));
	soldierInjuredLeftSprite.setTextureRect(sf::IntRect(318, 128, 45, 56));
	soldierDeadSprite.setTextureRect(sf::IntRect(531, 0, 46, 56));

	std::thread whatSpritePrint(&Soldier::selectNumberSprite, this);
	whatSpritePrint.detach();
}

Soldier::~Soldier() {
	/*
	soldierTexture.~Texture();
	soldierSprite.~Sprite();
	soldierLeftSprite[0].~Sprite();
	soldierRightSprite[0].~Sprite();
	soldierTopSprite[0].~Sprite();
	soldierLeftSprite[1].~Sprite();
	soldierRightSprite[1].~Sprite();
	soldierTopSprite[1].~Sprite();
	soldierLeftSprite[2].~Sprite();
	soldierRightSprite[2].~Sprite();
	soldierTopSprite[2].~Sprite();
	soldierLeftSprite[3].~Sprite();
	soldierRightSprite[3].~Sprite();
	soldierTopSprite[3].~Sprite();
	soldierShootingSprite[0].~Sprite();
	soldierShootingSprite[1].~Sprite();
	soldierInjuredTopSprite.~Sprite();
	soldierInjuredLeftSprite.~Sprite();
	soldierInjuredRightSprite.~Sprite();
	soldierDeadAnimationSprite[0].~Sprite();
	soldierDeadAnimationSprite[1].~Sprite();
	soldierDeadAnimationSprite[2].~Sprite();
	soldierDeadAnimationSprite[3].~Sprite();
	soldierDeadSprite.~Sprite();

	*/
	
}

void Soldier::move() {

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
	alreadyInjured = true;
}

void Soldier::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Soldier::shooting(int numeroAleatorio) {
	if (*estadoJuego == Status::ePLAYING) {
		//std::cout << "Intento de tiro a player: " << numeroAleatorio << std::endl;
		shoot.play();
		if (isVisible) {
			if (numeroAleatorio > 70 && player->isRunning()) {	//Si va corriendo, mas % de fallar
				player->getHitBy("soldado", (numeroAleatorio - 85) / 2);
			}
			else if (numeroAleatorio > 40) {
				player->getHitBy("soldado", (numeroAleatorio - 85) / 2);
			}
		}
	}
}

void Soldier::state(){
	int n = 0;
	srand(283 * std::hash<std::thread::id>{}(std::this_thread::get_id()));
	while (!isDead) {
		if (*estadoJuego == Status::ePLAYING) {
			n++;
			int fullRandom = rand() % 100000;		//Sacar 2 numeros aleatorios de 100 y 1000
			int randomnumber = fullRandom % 100;	//Quedarse con el primer XX del aleatorio
			fullRandom = fullRandom / 100;			//Quedarse con el segundo YY aleatorio
			if (n % 5 == 0 && isVisible) {	// minimo ocada 3 movimientos dispara.
				//shoot
				enemyState = EnemyState::shoot;
				std::this_thread::sleep_for(std::chrono::milliseconds(400));

				onFireShooting = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				std::thread dispara(&Soldier::shooting, this, fullRandom % 100);
				dispara.detach();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				onFireShooting = false;
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				n = 0;
			}
			else {	//No le toca disparar. Pensar movimiento mejor que dar vueltas cual autista
				if (randomnumber < 21) {
					//Move left
					anguloDeVista = 180;
					enemyState = EnemyState::moveLeft;
					std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom * 3));
				}
				else if (randomnumber < 42) {
					//Move top
					anguloDeVista = 90;
					enemyState = EnemyState::moveTop;
					std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom * 3));
				}
				else if (randomnumber < 63) {
					//Move right
					anguloDeVista = 0;
					enemyState = EnemyState::moveRight;
					std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom * 3));
				}
				else if (randomnumber < 84) {
					//Move down
					anguloDeVista = 270;
					enemyState = EnemyState::moveDown;
					std::this_thread::sleep_for(std::chrono::milliseconds(fullRandom * 3));
				}
				else if (isVisible) {
					//shoot
					enemyState = EnemyState::shoot;
					std::this_thread::sleep_for(std::chrono::milliseconds(400));

					onFireShooting = true;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					std::thread dispara(&Soldier::shooting, this, fullRandom % 100);
					dispara.detach();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));

					onFireShooting = false;
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					n = 0;
				}
			}

			if (isDead) {
				enemyState = EnemyState::dead;
				break;
			}
		}	//fin if ePAUSE
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));	//Seguro de que se quede en await quietecito.
	enemyState = EnemyState::dead;
}

void Soldier::playerMakeSound(){
	if (!isAwake && !isDead) {
		//float patataAux = sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y));
		//std::cout << "COMPROBACION DE DESPIERTO POR SONIDO " << patataAux << std::endl;
		if (sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y)) < 900) {	//900
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

//Proceso ASINCRONO que va cambiando los sprites de andar. Se genera automaticamente
void Soldier::selectNumberSprite() {
	while (!isDead) {
		if (alreadyInjured) {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			alreadyInjured = false;
		}
		else if (enemyState != EnemyState::shoot) {
			nSprite++;
			nSprite = nSprite % 4;
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
		}
	}
	nSprite = 0;
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	nSprite = 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	nSprite = 2;
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	nSprite = 3;
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	nSprite = -1;
}


void Soldier::renderEnemy(float playerAngle, sf::RenderWindow* m_pRenderWindow) {

	int spriteAUsar = nSprite;	//Puede ser que se modifique a mitad de busqueda y explote
	sf::Sprite* soldierSprite;

	if (!isDead) {
		if (enemyState == EnemyState::shoot) {
			if (onFireShooting) {
				soldierSprite = &soldierShootingSprite[1];
			}
			else {
				soldierSprite = &soldierShootingSprite[0];
			}
		}
		else {
			int playerAngle = player->GetAngle().GetValue();
			int anguloVista = anguloDeVista - playerAngle;
			//std::cout << "anguloVista inicial " << anguloVista << " :";
			if (anguloVista < 0) {
				anguloVista = 360 - abs(anguloVista);
			}

			//std::cout <<"con anguloDeVista "  << anguloDeVista  << " y angulo de player " << playerAngle << " resultado de "<< anguloVista << std::endl;
			if (anguloVista < 225 && anguloVista > 135) {		//Si son contrarios se miran de frente
				if (alreadyInjured) {
					soldierSprite = &soldierInjuredTopSprite;
				}
				else {
					soldierSprite = &soldierTopSprite[spriteAUsar];
				}
			}

			else if (anguloVista >= 225 && anguloVista <= 360) {	// Left
				if (alreadyInjured) {
					soldierSprite = &soldierInjuredLeftSprite;
				}
				else {
					soldierSprite = &soldierLeftSprite[spriteAUsar];
				}
			}

			else {
				if (alreadyInjured) {
					soldierSprite = &soldierInjuredRightSprite;
				}
				else {
					soldierSprite = &soldierRightSprite[spriteAUsar];
				}
			}
		}
	
	}
	else {
		if (spriteAUsar != -1) {
			soldierSprite = &soldierDeadAnimationSprite[spriteAUsar];
		}
		else {
			soldierSprite = &soldierDeadSprite;
		}
	}
	//std::cout << "Obteniendo sprite dimensiones " << Soldiersprite.getTextureRect().top << " " << Soldiersprite.getTextureRect().left << " " << Soldiersprite.getTextureRect().height << " "<< Soldiersprite.getTextureRect().width << std::endl;
	//m_pRenderWindow->draw(*soldierSprite);

	//A partir de aqui se usa Soldiersprite, que es el sprite a usar actualmente

	//3 CALCULOS PARA RENDERIZADO: CONSEGUIR ESCALA, CONSEGUIR EJE X, CONSEGUIR EJE Y:
	int distancia = sqrt((player->GetXPos() - x) * (player->GetXPos() - x) + (player->GetYPos() - y) * (player->GetYPos() - y));	//Calcula la distancia entre el enemigo y el jugador para el tamaño
	float escalado;
	bool reducirY = false;
	if (distancia > 800) {
		float diff = (float)distancia - 800;	//Por cada 75m acercados, aumentamos sprite un 0.1
		escalado = 0.3 - diff / 300 * 0.1;	// diff < 300, con lo cual valores entre 0.2 y 0.3
	}	
	else if (distancia > 600) {
		float diff = (float)distancia - 600;	//Por cada 75m acercados, aumentamos sprite un 0.1
		escalado = 0.5 - diff / 200 * 0.2 ;	// diff < 200, con lo cual valores entre 0.3 y 0.5
	}	
	else if (distancia > 380) {
		float diff = (float)distancia - 380;	//Por cada 75m acercados, aumentamos sprite un 0.1
		escalado = 0.9 - diff / 220 * 0.4;	// diff < 220, con lo cual valores entre 0.5 y 0.9
	}	
	else if (distancia > 150) {
		float diff =(float)distancia - 150;	//Por cada 75m acercados, aumentamos sprite un 0.1
		escalado = 1.5 - diff / 230 * 0.6;	// diff < 300, con lo cual valores entre 0.2 y 0.3
	}	
	else if (distancia > 60 ){
		float diff =(float)distancia - 60;	//Por cada 75m acercados, aumentamos sprite un 0.1
		escalado = 2.3 - diff / 90 * 0.5;	// diff < 300, con lo cual valores entre 0.2 y 0.3
	}
	else {
		float diff = (float)distancia;	//Por cada 75m acercados, aumentamos sprite un 0.1
		escalado = 3.5 - diff / 60 * 1.2;	// diff < 300, con lo cual valores entre 0.2 y 0.3
		//std::cout << "Reducir Y activado" << std::endl;
		reducirY = true;
	}

	if (escalado < 0.2) {	//Asignar un mínimo porque se va de madre sino
		escalado = 0.2;	
	}
	if (escalado > 3.5) {	//Asignar un máximo porque se va de madre sino
		escalado = 3.5;
	}
	//escalado = 2;
	soldierSprite->setScale(escalado, escalado);
	//std::cout << "a una distancia de " << distancia << " se obtiene escalado de " << escalado << std::endl;


	//CONSEGUIR EL EJE 'X' SEGUN ANGULO DE VISION
	float posRespectoDivisones = playerAngle - 45; // Valor entre 0 y 90, siendo 0 derecha max y 90 izq max
	posRespectoDivisones = abs(90 - posRespectoDivisones); // Alternamos valores, ahora 0 = izq max y 90 = derecha max
	float posicionRespectoPantalla = (posRespectoDivisones * SCREENWIDTH) / 90 - (soldierSprite->getTextureRect().width * escalado / 2);	//Regla de 3 para sacar la posicion con respecto a píxeles & centrar el sprite en el enemigo

	
	//CONSEGUIR EL EJE 'Y' SEGUN ALTURA DE PLAYER Y DEL ENEMIGO
	int16_t kk;
	float alturaEnemigo = player->GetZPos() - (map->getEnemySubsecHeight(xValue(), yValue(), kk) + DOOMGUYEYESPOS);	//Cuando es negativa hay que subir y viceversa 
	//std::cout << player->GetZPos() << " - " << map->getEnemySubsecHeight(xValue(), yValue()) << " = " << alturaEnemigo << std::endl;
	float y = SCREENHEIGHT / 2 - (soldierSprite->getTextureRect().height * escalado / 2) + ((double)alturaEnemigo * escalado / 1.5) + abs(alturaEnemigo*0.05 * escalado);// +abs(alturaEnemigo * 0.1); //Mitad de pantalla, subiendo(-) el sprite segun su tamaño y luego la altura en la que esta (funciona de forma inversa)
	if (reducirY) { y = y + (60 - distancia); }	//Si se acerca, se ve menos pies y mas cabeza (logic)
	
	soldierSprite->setPosition(posicionRespectoPantalla, y);

	//Para finalizar, dibujar en el lugar y escalado adecuado
	m_pRenderWindow->draw(*soldierSprite);



}