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

class Soldier : public Enemy {
public:
	Soldier(int x, int y, Player* player_, Map* map_);
	~Soldier();

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
	void renderEnemy(float playerAngle, sf::RenderWindow* m_pRenderWindow);

protected:
	//Incluidas x, y isAwake
	Player* player;
	EnemyState enemyState;
	Map* map;
	int anguloDeVista;

	bool isVisible;

	sf::SoundBuffer shootBuffer;
	sf::Sound shoot;

	sf::Texture soldierTexture;
	sf::Sprite soldierSprite;
};