/*
* Name: HUD.h
* Date: 22/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que engloba todo el HUD del jugador
*/

#pragma once
#include "Hud.h"
#include "../Player/Player.h"
#include "math.h"
#include "../doomdef.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <thread>
#include <chrono>

Hud::Hud(){}

Hud::Hud(sf::RenderWindow* renderWindow, Player* player) {

    //Load the hud texture & sprite
    if (!hudTexture.loadFromFile("../../../../assets/HUD/HUDInferior.png")) {
        std::cout << "ERROR LOAD HUD(hud) TEXTURE" << std::endl;
    }
    hudSprite.setTexture(hudTexture);
    hudSprite.scale((float)SCREENWIDTH / hudTexture.getSize().x, (float)SCREENHEIGHT / hudTexture.getSize().y);


    //Load all the 10 numbers to use it on armor-hp-ammo
    if (!numerosTexture.loadFromFile("../../../../assets/HUD/Numeros.png")) {
        std::cout << "ERROR LOAD NUMEROS(hud) TEXTURE" << std::endl;
    }
    for (int i = 0; i < 10; i++) {
        numerosSprite[i].setTexture(numerosTexture);
    }
    numerosSprite[0].setTextureRect(sf::IntRect(0, 0, 40, 64));
    numerosSprite[1].setTextureRect(sf::IntRect(44, 0, 31, 64));
    numerosSprite[2].setTextureRect(sf::IntRect(80, 0, 41, 64));
    numerosSprite[3].setTextureRect(sf::IntRect(125, 0, 41, 64));
    numerosSprite[4].setTextureRect(sf::IntRect(170, 0, 41, 64));
    numerosSprite[5].setTextureRect(sf::IntRect(216, 0, 41, 64));
    numerosSprite[6].setTextureRect(sf::IntRect(260, 0, 41, 64));
    numerosSprite[7].setTextureRect(sf::IntRect(305, 0, 41, 64));
    numerosSprite[8].setTextureRect(sf::IntRect(350, 0, 41, 64));
    numerosSprite[9].setTextureRect(sf::IntRect(395, 0, 40, 64));
    for (int i = 0; i < 10; i++) {
        numerosSprite[i].scale(2.5, 2.5);
        numerosSprite[i].setPosition(800, (float)SCREENHEIGHT - 233);
    }

    yValue = (float)SCREENHEIGHT - 231;
    m_pPlayer = player;
    m_pRenderWindow = renderWindow;
}

Hud::~Hud() {
}

//Draw player's hp numbers 
void Hud::drawLifePoints() {
    int hp = m_pPlayer->getHp();

    //Se dibuja las centenas
    int valor = hp / 100;
    if (valor != 0) {
        numerosSprite[valor].setPosition(740, yValue);
        m_pRenderWindow->draw(numerosSprite[valor]);
    }

    //Se dibujan las decenas
    valor = hp % 100 / 10;
    numerosSprite[valor].setPosition(840, yValue);
    m_pRenderWindow->draw(numerosSprite[valor]);

    //Se dibujan las unidades
    valor = hp % 10;
    numerosSprite[valor].setPosition(940, yValue);
    m_pRenderWindow->draw(numerosSprite[valor]);
    
}

//Draw player's armor numbers 
void Hud::drawArmorValue() {
    int armor = m_pPlayer->getArmor();

    //Se dibuja las centenas
    int valor = armor / 100;
    if (valor != 0) {
        numerosSprite[valor].setPosition(1375, yValue);
        m_pRenderWindow->draw(numerosSprite[valor]);
    }

    //Se dibujan las decenas
    valor = armor % 100 / 10;
    numerosSprite[valor].setPosition(1475, yValue);
    m_pRenderWindow->draw(numerosSprite[valor]);

    //Se dibujan las unidades
    valor = armor % 10;
    numerosSprite[valor].setPosition(1575, yValue);
    m_pRenderWindow->draw(numerosSprite[valor]);
}

//Draw player's ammo numbers 
void Hud::drawAmmoValue() {
    int ammo = m_pPlayer->getAmmo();

    //Se dibuja las centenas
    int valor = ammo / 100;
    if (valor != 0) {
        numerosSprite[valor].setPosition(1925, yValue);
        m_pRenderWindow->draw(numerosSprite[valor]);
    }

    //Se dibujan las decenas
    valor = ammo % 100 / 10;
    numerosSprite[valor].setPosition(2025, yValue);
    m_pRenderWindow->draw(numerosSprite[valor]);

    //Se dibujan las unidades
    valor = ammo % 10;
    numerosSprite[valor].setPosition(2125, yValue);
    m_pRenderWindow->draw(numerosSprite[valor]);
}

//Draw all the HUD options
void Hud::drawHud() {
    m_pRenderWindow->draw(hudSprite);
    //m_pRenderWindow->draw(numerosSprite[1]);
    drawLifePoints();
    drawArmorValue();
    drawAmmoValue();
}