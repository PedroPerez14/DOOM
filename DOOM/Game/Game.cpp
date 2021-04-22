/*
* Name: Game.cpp
* Date: 09/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que gestionará la lógica del juego y el gameloop
*/

#pragma once
#include "Game.h" 
#include "../doomdef.h"

#include "Windows.h"    //TODO borrar cuando no me haga falta

Game::Game()
{
    id_new_player = 0;
    m_pPlayer = new Player(id_new_player++);
}

Game::~Game(){}

void Game::ProcessInput(Status status)
{
    sf::Event event;
    while (m_pWindow->pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Resized:
                // update the view to the new size of the window, CREO QUE NO HACE FALTA
                //handleResize();
                break;
        
            case sf::Event::Closed:
                m_pDoomEngine->Quit();
                m_pWindow->close();
                break;

            case sf::Event::KeyPressed:
                if (status == Status::ePLAYING)
                {
                    if (event.key.code == sf::Keyboard::Escape)    //Separamos un input del juego con un input de menús como el de pausa
                    {
                        gameState = Status::ePAUSE;
                    }
                    else
                    {
                        m_pDoomEngine->KeyPressed(event);
                    }
                }
                else if (status == Status::ePAUSE)  //Si aquí dentro tenemos más teclas aparte de reanudar y salir mejor un switch
                {
                    if (event.key.code == sf::Keyboard::Escape)     //Reanudar
                    {
                        gameState = Status::ePLAYING;
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        m_pDoomEngine->Quit();
                        m_pWindow->close();
                    }
                }
                break;

            case sf::Event::KeyReleased:
                if (status == Status::ePLAYING)
                {
                    m_pDoomEngine->KeyReleased(event);
                }
                //Si pulsa esc ingame, pasar a estado pausa (en teoría este código comentado no debería ejecutarse nunca
                /*
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    std::cout << "Cambiar status a pause" << std::endl;
                    gameState = Status::ePAUSE;
                }
                */
                break;
            default:
                break;
        }

    }
}

void Game::Render()
{
    if (!IsOver())
    {
        switch (gameState)
        {
            case Status::eMAINMENU:
                mainMenu();
                break;
            case Status::ePLAYING:
                m_pDoomEngine->Render();
                break;
            case Status::ePAUSE:
                m_pDoomEngine->Render();
                m_pPauseMenu->RenderPauseMenu();
                break;
            default:
                break;
        }
        m_pWindow->display();
    }
}

void Game::Update()
{
    if (gameState == Status::ePLAYING)
    {
        m_pDoomEngine->Update(gameState);
    }
}

void Game::Delay(float elapsedFrameTime)
{
    float timeToWait = (1.0f / (float)TARGETFRAMERATE) - elapsedFrameTime;
    if (timeToWait > 0)
    {
        sf::sleep(sf::seconds(timeToWait));
    }
}

bool Game::IsOver()
{
	return m_pDoomEngine->isOver();
}

bool Game::Init()
{
    m_pDisplayManager = new DisplayManager();
    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager);
    m_pWindow = m_pDisplayManager->Init(m_pDoomEngine->GetName());
    m_pPlayer->Init(m_pWindow);
    m_pPauseMenu = new PauseMenu(m_pWindow);
    if (!m_pDoomEngine->Init(m_pWindow))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
        return false;
    }
	//TODO estaría bien hacer que renderice solo a 320 x 200 aunque la ventana se agrandara pero no tengo mucha idea de cómo hacerlo
    gameState = Status::eMAINMENU;
    return true;
}

void Game::handleResize()   //Creo que no me hace falta
{
    int w = m_pWindow->getSize().x;
    int h = m_pWindow->getSize().y;

    int desktop_height = sf::VideoMode::getDesktopMode().height;
    int desktop_width = sf::VideoMode::getDesktopMode().width;

    float ratio = (SCREENWIDTH / (float)SCREENHEIGHT);
    float inv_ratio = 1 / ratio;
    const float _FACTOR = 1.0f; //Para no considerar TODA la pantalla, pero sí una buena parte de ella (?)

    sf::Vector2u v;
    sf::FloatRect visibleArea;

    if (w * inv_ratio > h)
    {
        v = sf::Vector2u(w, round(w * inv_ratio));
        if (v.x <= desktop_width * _FACTOR && v.y <= desktop_height * _FACTOR)
        {
            m_pWindow->setSize(v);

            //visibleArea = sf::FloatRect(0, 0, v.x, v.x * inv_ratio);
            visibleArea = sf::FloatRect(0, 0, SCREENWIDTH, SCREENHEIGHT);
            m_pWindow->setView(sf::View(visibleArea));
        }
        else
        {
            v = sf::Vector2u(desktop_height * _FACTOR * ratio, desktop_height * _FACTOR);
            //visibleArea = sf::FloatRect(0, 0, desktop_height * _FACTOR * ratio, desktop_height * _FACTOR);
            visibleArea = sf::FloatRect(0, 0, SCREENWIDTH, SCREENHEIGHT);
            m_pWindow->setSize(v);
            m_pWindow->setView(sf::View(visibleArea));
        }
    }
    else if (h * ratio > w)
    {
        v = sf::Vector2u(h * ratio, h);
        if (v.x <= desktop_width * _FACTOR && v.y <= desktop_height * _FACTOR)
        {
            m_pWindow->setSize(v);

            //visibleArea = sf::FloatRect(0, 0, v.y * ratio, v.y);
            visibleArea = sf::FloatRect(0, 0, SCREENWIDTH, SCREENHEIGHT);
            m_pWindow->setView(sf::View(visibleArea));
        }
        else
        {
            v = sf::Vector2u(desktop_height * _FACTOR * ratio, desktop_height * _FACTOR);
            //visibleArea = sf::FloatRect(0, 0, desktop_height * _FACTOR * ratio, desktop_height * _FACTOR);
            visibleArea = sf::FloatRect(0, 0, SCREENWIDTH, SCREENHEIGHT);
            m_pWindow->setSize(v);
            m_pWindow->setView(sf::View(visibleArea));
        }
    }
}

Status Game::getStatus()
{
    return gameState;
}

int Game::mainMenu()
{
    //Create the window to display

    //Load songs of menu
    sf::SoundBuffer shotBuffer;
    sf::Sound shot;
    if (!shotBuffer.loadFromFile("../../../../assets/MainMenu/shot.wav"))
        std::cout << "Error al cargar audio de tiro en mainMenu" << std::endl;
    shot.setBuffer(shotBuffer);

    sf::Music introMusic;
    if (!introMusic.openFromFile("../../../../assets/MainMenu/MainMenuMusic.wav"))
        std::cout << "Error al cargar music en mainMenu" << std::endl;

    //Start the music and the loop on the menu:
    introMusic.play();
    introMusic.setLoop(true);
    //Create the menu itself
    Menu menu((float)m_pWindow->getView().getSize().x, (float)m_pWindow->getView().getSize().y, m_pDoomEngine);
    menu.drawIntro(m_pWindow);
    double soundLevel = 100;
    

    while (m_pWindow->isOpen()) {
        sf::Event event;

        while (m_pWindow->pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Resized:
                //handleResize();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                //TODO rehacer esto
                case sf::Keyboard::Up:
                    shot.play();
                    std::cout << "Detectada tecla pulsada up" << std::endl;
                    menu.MoveUp();
                    break;

                case sf::Keyboard::Down:
                    shot.play();
                    std::cout << "Detectada tecla pulsada down" << std::endl;
                    menu.MoveDown();
                    break;

                case sf::Keyboard::Escape:
                    shot.play();
                    std::cout << "Detectada tecla pulsada ESC" << std::endl;
                    menu.drawIntro(m_pWindow);
                    break;

                case sf::Keyboard::Return:
                    shot.play();
                    std::cout << "Detectada tecla pulsada Enter" << std::endl;
                    switch (menu.GetPressedItem()) {
                    case 0:     //Entra en Play
                        introMusic.stop();
                        gameState = Status::ePLAYING;
                        return 0;
                        break;

                    case 1:     //Entra en ajustes
                        soundLevel = menu.options(m_pWindow, &introMusic, &shot);
                        std::cout << soundLevel << std::endl;
                        gameState = Status::eOPTIONS;
                        break;

                    case 2:     //Entra en creditos
                        menu.creditPage(m_pWindow);
                        gameState = Status::eCREDITS;
                        break;

                    case 3:     //Sale del juego
                        m_pDoomEngine->Quit();
                        m_pWindow->close();
                        break;

                    default:    //Ha dado a una quinta opcion (?)
                        std::cout << "MENU FATAL ERROR: CHOOSED A NO-NUMBERED OPTION" << std::endl;
                        m_pDoomEngine->Quit();
                        m_pWindow->close();
                        return -1;
                        break;
                    }
                    break;
                }
                break;

            case sf::Event::Closed:
                m_pWindow->close();
                m_pDoomEngine->Quit();
                break;
            }
        }
        //std::cout << "Limpia a negro y duerme" << std::endl;
        m_pWindow->clear(sf::Color(0, 0, 0));
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));

        //std::cout << "Dibuja" << std::endl;
        menu.draw(m_pWindow);
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));

        //std::cout << "Display" << std::endl;
        m_pWindow->display();
    }

    return 0;
}
