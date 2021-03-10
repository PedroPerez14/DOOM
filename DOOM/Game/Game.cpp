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

Game::Game()
{
    m_pDoomEngine = new DoomEngine();
}

Game::~Game(){}

void Game::ProcessInput()
{
    sf::Event event;
    sf::Vector2u v;
    int w, h;
    while (m_pWindow->pollEvent(event))
    {
        sf::FloatRect visibleArea;
        switch(event.type)
        {
        case sf::Event::Resized:
            // update the view to the new size of the window
            
            w = m_pWindow->getSize().x;
            h = m_pWindow->getSize().y;

            if (w * (SCREENHEIGHT / (float)SCREENWIDTH) > h)
            {
                v = sf::Vector2u(m_pWindow->getSize().x, m_pWindow->getSize().x * (SCREENHEIGHT / (float)SCREENWIDTH));
                m_pWindow->setSize(v);

                //visibleArea = sf::FloatRect(0, 0, SCREENWIDTH, SCREENHEIGHT);
                //m_pWindow->setView();
                sf::View v = m_pWindow->getView();
                v.setSize({
                                        static_cast<float>(event.size.width),
                                        static_cast<float>(event.size.height)
                    });
                v.setCenter({   //TODO arreglar
                                        static_cast<float>(event.size.width) / 2.0f,
                                        static_cast<float>(event.size.height) / 2.0f
                    });
                m_pWindow->setView(v);
            }
            else
            {
                v = sf::Vector2u(m_pWindow->getSize().y * (SCREENWIDTH / (float)SCREENHEIGHT), m_pWindow->getSize().y);
                m_pWindow->setSize(v);

                //visibleArea = sf::FloatRect(0, 0, SCREENWIDTH, SCREENHEIGHT);
                //m_pWindow->setView(sf::View(visibleArea));
            }
            break;
        
        case sf::Event::Closed:
            m_pDoomEngine->Quit();
            m_pWindow->close();
            break;

        case sf::Event::KeyPressed:
            m_pDoomEngine->KeyPressed(event);
            break;

        case sf::Event::KeyReleased:
            m_pDoomEngine->KeyReleased(event);
            break;

        default:
            break;
        }

    }
}

void Game::Render()
{
    m_pDoomEngine->Render(m_pWindow);
}

void Game::Update()
{

}

void Game::Delay()
{

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
    
    if (!m_pDoomEngine->Init())
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
        return false;
    }
	//TODO cosas para que renderice a 320x200 aunque la ventana sea más grande

    return true;
}
