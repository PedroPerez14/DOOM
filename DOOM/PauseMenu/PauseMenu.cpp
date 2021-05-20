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
    interNuclearSprite.scale(0.5f, 0.42f);

    if (!interToxicTexture.loadFromFile("../../../../assets/LoadingScreens/IntermissionToxic.png")) {
        std::cout << "Error on load fondo hangar texture (Pausemenu.cpp)" << std::endl;
    }
    interToxicSprite.setTexture(interToxicTexture);
    interToxicSprite.scale(0.5f, 0.42f);





    if (!Hangar.loadFromFile("../../../../assets/LoadingScreens/Hangar.png")) {
        std::cout << "Error on load fondo hangar texture (Pausemenu.cpp)" << std::endl;
    }
    HangarSprite.setTexture(Hangar);
    HangarSprite.scale(0.35f, 0.35f);
    HangarSprite.setPosition(SCREENWIDTH / 2 - Hangar.getSize().x * HangarSprite.getScale().x / 2, 8);

    if (!Nuclear.loadFromFile("../../../../assets/LoadingScreens/Nuclear.png")) {
        std::cout << "Error on load fondo Nuclear texture (Pausemenu.cpp)" << std::endl;
    }
    NuclearSprite.setTexture(Nuclear);
    NuclearSprite.scale(0.35f, 0.35f);
    NuclearSprite.setPosition(SCREENWIDTH / 2 - Nuclear.getSize().x * NuclearSprite.getScale().x / 2, 8);

    if (!Toxic.loadFromFile("../../../../assets/LoadingScreens/Toxic.png")) {
        std::cout << "Error on load fondo Toxic texture (Pausemenu.cpp)" << std::endl;
    }
    ToxicSprite.setTexture(Toxic);
    ToxicSprite.scale(0.35f, 0.35f);
    ToxicSprite.setPosition(SCREENWIDTH / 2 - Toxic.getSize().x * ToxicSprite.getScale().x / 2, 8);

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

    if (!porcentaje.loadFromFile("../../../../assets/LoadingScreens/porciento.png")) {
        std::cout << "Error on load % (Pausemenu.cpp)" << std::endl;
    }
    porcentajeSprite.setTexture(porcentaje);
    porcentajeSprite.scale(0.35f, 0.35f);
    porcentajeSprite.setPosition(SCREENWIDTH - SCREENWIDTH / 10, bajasSprite.getPosition().y);

    if (!enhorabuena.loadFromFile("../../../../assets/LoadingScreens/Enhorabuena.png")) {
        std::cout << "Error on load % (Pausemenu.cpp)" << std::endl;
    }
    enhorabuenaSprite.setTexture(enhorabuena);
    enhorabuenaSprite.scale(0.35f, 0.35f);
    enhorabuenaSprite.setPosition(SCREENWIDTH/2 - enhorabuena.getSize().x * enhorabuenaSprite.getScale().x/2, 20);


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

    if (!endTextTexture.loadFromFile("../../../../assets/LoadingScreens/EndScreen.png")) {
        std::cout << "Error on load fondo Toxic texture (Pausemenu.cpp)" << std::endl;
    }
    endTextSprite.setTexture(endTextTexture);
    endTextSprite.scale(0.3f, 0.3f);
    endTextSprite.setPosition( SCREENWIDTH/2 - endTextTexture.getSize().x* endTextSprite.getScale().x/2, 70 );


    if (!endBackgroundTexture.loadFromFile("../../../../assets/LoadingScreens/EndBackground.jpg")) {
        std::cout << "Error on load fondo Toxic texture (Pausemenu.cpp)" << std::endl;
    }
    endBackgroundSprite.setTexture(endBackgroundTexture);
    endBackgroundSprite.scale(0.4f, 0.4f);


    //ENTRANDO ZONA DE MUSICAS Y EFECTOS:

    if (!reloadBuffer.loadFromFile("../../../../assets/Music/recarga.wav")) {
        std::cout << "Error al cargar audio de recarga en pausemenu" << std::endl;
    }
    reload.setBuffer(reloadBuffer);
}

PauseMenu::~PauseMenu()
{

}

void PauseMenu::setVolumenes(float soundLevel) {
    reload.setVolume(soundLevel*1.5);
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

//Transition de lo que haya en pantalla a fondoSprite
void PauseMenu::renderSmeltEffect() {
    sf::Image imagenActual = m_pRenderWindow->capture();

    sf::Texture imagenTexture;
    imagenTexture.loadFromImage(imagenActual);
    std::cout << imagenTexture.getSize().x << " " << imagenTexture.getSize().y << std::endl;

    sf::Sprite cortesSmelt[320];
    for (int i = 0; i < 320; i++) {
        cortesSmelt[i].setTexture(imagenTexture);
        cortesSmelt[i].setTextureRect(sf::IntRect(1 * i * 4, 0, 4, 800));
        cortesSmelt[i].setPosition(i, 0);
        cortesSmelt[i].scale(0.25, 0.25);
    }

    srand(time(NULL));
    int randomVal = (rand() % 100);
    cortesSmelt[0].move(0, randomVal);
    for (int i = 1; i < 320; i++) {
        randomVal = ((randomVal + (rand() % 10 - 10)) % 20);
        cortesSmelt[i].move(0, randomVal);
    }

    for (int i = 0; i < 130; i++) {

        for (int i = 0; i < 320; i++) {
            cortesSmelt[i].move(0, 2 + (rand() % 3));		//Edita esto para cambiar lo que baja por render
        }

        m_pRenderWindow->clear(sf::Color::Black);
        m_pRenderWindow->draw(fondoSprite);
        for (int i = 0; i < 320; i++) {
            m_pRenderWindow->draw(cortesSmelt[i]);
        }
        m_pRenderWindow->display();

        std::this_thread::sleep_for(std::chrono::milliseconds(9));		//Edita esto para cambiar la velocidad de render
    }

}

void PauseMenu::RenderCarga1(int porcentaje) {

    renderSmeltEffect();

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

    reload.play();

    m_pRenderWindow->clear();
    m_pRenderWindow->draw(interNuclearSprite);
    m_pRenderWindow->display();
        
}

void PauseMenu::RenderCarga2(int porcentaje) {

    renderSmeltEffect();

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
        m_pRenderWindow->draw(NuclearSprite);
        m_pRenderWindow->draw(finalizadoSprite);
        m_pRenderWindow->draw(bajasSprite);
        m_pRenderWindow->draw(porcentajeSprite);
        renderPorcentajeKills(porcentaje);
        m_pRenderWindow->draw(cargaCompletaSprite);
        m_pRenderWindow->display();
    }

    reload.play();

    m_pRenderWindow->clear();
    m_pRenderWindow->draw(interToxicSprite);
    m_pRenderWindow->display();
}


void PauseMenu::RenderCarga3(int porcentaje) {

    renderSmeltEffect();

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
        m_pRenderWindow->draw(ToxicSprite);
        m_pRenderWindow->draw(finalizadoSprite);
        m_pRenderWindow->draw(bajasSprite);
        m_pRenderWindow->draw(porcentajeSprite);
        renderPorcentajeKills(porcentaje);
        m_pRenderWindow->draw(cargaCompletaSprite);
        m_pRenderWindow->display();
    }

    reload.play();

}


void PauseMenu::RenderEnd() {
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
        m_pRenderWindow->clear(sf::Color::Black);
        m_pRenderWindow->draw(endBackgroundSprite);
        m_pRenderWindow->draw(endTextSprite);
        m_pRenderWindow->draw(enhorabuenaSprite);
        m_pRenderWindow->display();
    }
}