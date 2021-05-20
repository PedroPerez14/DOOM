/*
* Name: PauseMenu.cpp
* Date: 16/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que gestiona la lógica y los elementos del menú de pausa in-game.
*/

#pragma once

#include "PauseMenu.h"
#include "../doomdef.h"
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

PauseMenu::PauseMenu(sf::RenderWindow* m_pRenderWindow) : m_pRenderWindow(m_pRenderWindow)
{
    //Se cargan en memoria las imágenes que van a hacer falta posteriormente

    if (!PauseTextTexture.loadFromFile("../../../../assets/PauseMenu/TextoPausa.png")) {
        std::cout << "Error on load LetraMenu texture (Pausemenu.cpp)" << std::endl;
    }
    PauseTextSprite.setTexture(PauseTextTexture);
    PauseTextSprite.scale((float)SCREENWIDTH * 0.75f / PauseTextTexture.getSize().x, (float)SCREENHEIGHT * 0.2 / PauseTextTexture.getSize().y);
    PauseTextSprite.setPosition((SCREENWIDTH / 2.0f) - PauseTextTexture.getSize().x * PauseTextSprite.getScale().x / 2.0f + 3.0f, SCREENHEIGHT / 2.7f);

    if (!DeathTextTexture.loadFromFile("../../../../assets/PauseMenu/TextoMuerte.png")) {
        std::cout << "Error on load texto muerte texture (Pausemenu.cpp)" << std::endl;
    }
    DeathTextSprite.setTexture(DeathTextTexture);
    DeathTextSprite.scale((float)SCREENWIDTH * 0.75f / DeathTextTexture.getSize().x, (float)SCREENHEIGHT * 0.35 / DeathTextTexture.getSize().y);
    DeathTextSprite.setPosition((SCREENWIDTH / 2.0f) - DeathTextTexture.getSize().x * DeathTextSprite.getScale().x / 2.0f + 3.0f, SCREENHEIGHT / 3.2f);





    if (!fondoTexture.loadFromFile("../../../../assets/LoadingScreens/E1Intermission.png")) {
        std::cout << "Error on load fondo hangar texture (Pausemenu.cpp)" << std::endl;
    }
    fondoSprite.setTexture(fondoTexture);
    fondoSprite.scale(0.5f, 0.5f);

    if (!interNuclearTexture.loadFromFile("../../../../assets/LoadingScreens/IntermissionNuclearPlant.png")) {
        std::cout << "Error on load fondo hangar texture (Pausemenu.cpp)" << std::endl;
    }
    interNuclearSprite.setTexture(interNuclearTexture);
    interNuclearSprite.scale(0.5f, 0.4166f);        //Regla de tres, 620x480 pasado a 320x200





    if (!Hangar.loadFromFile("../../../../assets/LoadingScreens/Hangar.png")) {
        std::cout << "Error on load fondo hangar texture (Pausemenu.cpp)" << std::endl;
    }
    HangarSprite.setTexture(Hangar);
    HangarSprite.scale(0.35f, 0.35f);
    HangarSprite.setPosition(SCREENWIDTH / 2 - Hangar.getSize().x * HangarSprite.getScale().x / 2, 8);

    if (!finalizado.loadFromFile("../../../../assets/LoadingScreens/Finalizado.png")) {
        std::cout << "Error on load fondo hangar texture (Pausemenu.cpp)" << std::endl;
    }
    finalizadoSprite.setTexture(finalizado);
    finalizadoSprite.scale(0.35f, 0.35f);
    finalizadoSprite.setPosition(SCREENWIDTH / 2 - finalizado.getSize().x * finalizadoSprite.getScale().x / 2, 8 + 5 + Hangar.getSize().y * HangarSprite.getScale().y);

    if (!bajas.loadFromFile("../../../../assets/LoadingScreens/Bajas.png")) {
        std::cout << "Error on load fondo hangar texture (Pausemenu.cpp)" << std::endl;
    }
    bajasSprite.setTexture(bajas);
    bajasSprite.scale(0.35f, 0.35f);
    bajasSprite.setPosition(SCREENWIDTH / 10 , finalizadoSprite.getPosition().y + finalizado.getSize().y * finalizadoSprite.getScale().y + 20);
    std::cout << bajasSprite.getPosition().x << " " << bajasSprite.getPosition().y << std::endl;

    if (!porcentaje.loadFromFile("../../../../assets/LoadingScreens/porciento.png")) {
        std::cout << "Error on load % (Pausemenu.cpp)" << std::endl;
    }
    porcentajeSprite.setTexture(porcentaje);
    porcentajeSprite.scale(0.35f, 0.35f);
    porcentajeSprite.setPosition(SCREENWIDTH - SCREENWIDTH / 10, bajasSprite.getPosition().y);
    std::cout << bajasSprite.getPosition().x << " " << bajasSprite.getPosition().y << std::endl;

    if (!cargaCompletaTexture.loadFromFile("../../../../assets/LoadingScreens/CargaCompleta.png")) {
        std::cout << "Error on load fondo cargaCompleta (Pausemenu.cpp)" << std::endl;
    }
    cargaCompletaSprite.setTexture(cargaCompletaTexture);
    cargaCompletaSprite.scale(0.22f, 0.22f);
    cargaCompletaSprite.setPosition(SCREENWIDTH /2 - cargaCompletaTexture.getSize().x * cargaCompletaSprite.getScale().x/2, SCREENHEIGHT - SCREENHEIGHT / 8);

    if (!numerosTexture.loadFromFile("../../../../assets/LoadingScreens/numeros.png")) {
        std::cout << "Error on load fondo cargaCompleta (Pausemenu.cpp)" << std::endl;
    }
    for (int i = 0; i < 10; i++) {
        numerosSprite[i].setTexture(numerosTexture);
    }
    numerosSprite[1].setTextureRect(sf::IntRect(0, 0, 21, 42));
    numerosSprite[2].setTextureRect(sf::IntRect(22, 0, 27, 42));
    numerosSprite[3].setTextureRect(sf::IntRect(50, 0, 27, 42));
    numerosSprite[4].setTextureRect(sf::IntRect(78, 0, 27, 42));
    numerosSprite[5].setTextureRect(sf::IntRect(106, 0, 27, 42));
    numerosSprite[6].setTextureRect(sf::IntRect(133, 0, 27, 42));
    numerosSprite[7].setTextureRect(sf::IntRect(161, 0, 28, 42));
    numerosSprite[8].setTextureRect(sf::IntRect(190, 0, 27, 42));
    numerosSprite[9].setTextureRect(sf::IntRect(218, 0, 27, 42));
    numerosSprite[0].setTextureRect(sf::IntRect(246, 0, 26, 42));
    for (int i = 0; i < 10; i++) {
        numerosSprite[i].scale(0.35f, 0.35f);
    }
}

PauseMenu::~PauseMenu()
{

}

void PauseMenu::RenderPauseMenu()
{
    //A no ser que queramos meter más cosas esto ya estaría
    m_pRenderWindow->draw(PauseTextSprite);
}

void PauseMenu::RenderDeathMenu() {
    m_pRenderWindow->draw(DeathTextSprite);
}

void PauseMenu::renderPorcentajeKills(int porcentaje) {

    int unidades = porcentaje % 10;        
    int centenas = porcentaje / 100;
    int decenas = porcentaje % 100 / 10;

    numerosSprite[unidades].setPosition(porcentajeSprite.getPosition().x - numerosSprite[unidades].getTextureRect().width * numerosSprite[unidades].getScale().x - 1
                                        , porcentajeSprite.getPosition().y);      //Unidades Siempre
    m_pRenderWindow->draw(numerosSprite[unidades]);

   
    if (decenas != 0 || centenas != 0) {        //Renderiza decenas
        numerosSprite[decenas].setPosition(numerosSprite[unidades].getPosition().x - numerosSprite[decenas].getTextureRect().width * numerosSprite[decenas].getScale().x - 1
            , porcentajeSprite.getPosition().y);      //Unidades Siempre
        m_pRenderWindow->draw(numerosSprite[decenas]);
    }

    
    if (centenas != 0) {                        //Renderiza centenas
        numerosSprite[centenas].setPosition(numerosSprite[decenas].getPosition().x - numerosSprite[centenas].getTextureRect().width * numerosSprite[centenas].getScale().x - 1
            , porcentajeSprite.getPosition().y);      //Unidades Siempre
        m_pRenderWindow->draw(numerosSprite[centenas]);
    }
}

void PauseMenu::RenderCarga1(int porcentaje) {

    sf::Event event;
    bool skipIntro = false;
    while (m_pRenderWindow->pollEvent(event) || !skipIntro) {
        switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Enter)
            {
                skipIntro = true;
            }
            break;

        case sf::Event::Closed:
            m_pRenderWindow->close();
            break;

        default:
            break;

        }
        m_pRenderWindow->draw(fondoSprite);
        m_pRenderWindow->draw(HangarSprite);
        m_pRenderWindow->draw(finalizadoSprite);
        m_pRenderWindow->draw(bajasSprite);
        m_pRenderWindow->draw(porcentajeSprite);
        renderPorcentajeKills(porcentaje);
        m_pRenderWindow->draw(cargaCompletaSprite);
        m_pRenderWindow->display();
    }

    m_pRenderWindow->clear();

    m_pRenderWindow->draw(interNuclearSprite);
    m_pRenderWindow->display();
        
}

void PauseMenu::RenderCarga2(int porcentaje) {
}