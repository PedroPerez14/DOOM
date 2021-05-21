/*
* Name: Enemy.h
* Date: 05/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase enemigo: Soldado
*/

#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "Enemy.h"
#include "EnemyStates.h"
#include "../Player/Player.h"
#include "../maps/map.h"
#include "../Game/GameStates.h"

class Soldier : public Enemy {
public:
	Soldier(int x, int y, Player* player_, Map* map_, Status* thisStatus, int tipoSonido);
	~Soldier();
	void changeVolumenes(int soundLevel);

	float xValue();
	float yValue();

	bool getVisible();
	void setVisible(bool visible);

	virtual void shooting(int numeroRandom) override;
	virtual void move() override;
	virtual void state() override;
	virtual void playerMakeSound() override;
	virtual void playerMove() override;
	virtual void nextMove() override;
	virtual void getHitByUser(float anguloDisparo) override;
	virtual void setPosition(float x, float y);

	float getDistToPlayer();
	void renderEnemy(float playerAngle, sf::RenderWindow* m_pRenderWindow);
	bool isReallyDead();
	void setDead(bool dead_);

	bool operator < (Soldier& s);
	bool operator > (Soldier& s);
	bool operator <= (Soldier& s);
	bool operator >= (Soldier& s);

protected:
	//Incluidas x, y isAwake
	Player* player;
	EnemyState enemyState;
	Map* map;
	Status* estadoJuego;
	int anguloDeVista;

	bool isVisible;

	sf::SoundBuffer shootBuffer;
	sf::Sound shoot;

	sf::Texture soldierTexture;
	sf::Sprite soldierSprite;

	sf::SoundBuffer awakeBuffer;
	sf::Sound awakeSound;
	sf::SoundBuffer deathBuffer;
	sf::Sound deathSound;
	sf::SoundBuffer injuredBuffer;
	sf::Sound injuredSound;
};