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
    m_pDoomEngine = new DoomEngine(m_pPlayer);
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
            // update the view to the new size of the window
            handleResize();
            break;
        
        case sf::Event::Closed:
            m_pDoomEngine->Quit();
            m_pWindow->close();
            break;

        case sf::Event::KeyPressed:
            if (status == Status::ePLAYING)
            {
                m_pDoomEngine->KeyPressed(event);
            }
            break;

        case sf::Event::KeyReleased:
            if (status == Status::ePLAYING)
            {
                m_pDoomEngine->KeyReleased(event);
            }
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
            default:
                break;
        }
    }
}

void Game::Update()
{

}

void Game::Delay()
{
    Sleep(250); //TODO ajustar a lo que necesite, puede que no haga falta delay y me sirva con sf::Clock??
}

bool Game::IsOver()
{
	return m_pDoomEngine->isOver();
}

bool Game::Init()
{
	m_pWindow = new sf::RenderWindow(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), m_pDoomEngine->GetName());
	if (m_pWindow == nullptr)
	{
		std::cerr << "SFML could not create a window to play DOOM in. Going back to hell..." << std::endl;
		return false;
	}
    
    if (!m_pDoomEngine->Init(m_pWindow))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
        return false;
    }
	//TODO estaría bien hacer que renderice solo a 320 x 200 aunque la ventana se agrandara pero no tengo mucha idea de cómo hacerlo
    gameState = Status::eMAINMENU;
    return true;
}

void Game::handleResize()
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
    //::RenderWindow window(sf::VideoMode(600, 450), "DoomMenu");

    //Load songs of menu
    sf::SoundBuffer shotBuffer;
    sf::Sound shot;
    if (!shotBuffer.loadFromFile("../../../../assets/MainMenu/shot.wav"))
        std::cout << "Error al cargar audio de tiro en mainMenu" << std::endl;
    shot.setBuffer(shotBuffer);

    sf::Music introMusic;
    if (!introMusic.openFromFile("../../../../assets/MainMenu/MainMenuMusic.wav"))
        std::cout << "Error al cargar music en mainMenu" << std::endl;

    introMusic.play();
    introMusic.setLoop(true);
    //Create the menu himself
    Menu menu((float)m_pWindow->getView().getSize().x, (float)m_pWindow->getView().getSize().y);
    menu.drawIntro(m_pWindow);

    //Start the music and the loop on the menu:


    while (m_pWindow->isOpen()) {
        sf::Event event;

        while (m_pWindow->pollEvent(event)) {
            switch (event.type) {
            case sf::Event::KeyReleased:
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
                        std::cout << "TODO Entra en DOOM" << std::endl;
                        introMusic.stop();
                        gameState = Status::ePLAYING;
                        return 0;
                        break;

                    case 1:     //Entra en ajustes
                        std::cout << "TODO Entra en ajustes" << std::endl;
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
