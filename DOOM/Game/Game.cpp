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
    soundLevel = 100;
    m_pPlayer = new Player(id_new_player++);
    actualLevel = 1;
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
                        m_pDoomEngine->releasePlayerInputs();
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
                else if (status == Status::eDEAD) {     //Cuando muere, interfaz para seleccionar siguiente paso (salir o reiniciar)
                    std::cout << "Ha reconocido status dead" << std::endl;
                    if (event.key.code == sf::Keyboard::Escape)     //Preguntar para salir
                    {
                        //Preguntar si realmente desea salir
                        gameState = Status::ePAUSE;
                        m_pDoomEngine->releasePlayerInputs();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        //Reiniciar nivel:
                        resetLevel();
                        gameState = Status::ePLAYING;
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
            case Status::eDEAD:
                m_pDoomEngine->Render();
                m_pPauseMenu->RenderDeathMenu();
            default:
                break;
        }
        m_pWindow->display();
    }
}

void Game::setDeltaTime(const float& dT)
{
    m_deltaTime = dT;
    m_pDoomEngine->setDeltaTime(dT);
}

float Game::getDeltaTime()
{
    return m_deltaTime;
}

void Game::Update()
{
    if (gameState == Status::ePLAYING)
    {
        bool nextLevel = m_pDoomEngine->Update(gameState);
        if (nextLevel && actualLevel == 1) { //Updatea el doomEngine. Si player está en final return true.
            loadLevel2();


        } else if (nextLevel && actualLevel == 2) { //Updatea el doomEngine. Si player está en final return true.
            loadLevel3();
        }
        else if (nextLevel && actualLevel == 3) {
            loadEndGame();
        }

        if (m_pPlayer->getHp() <= 0) {  //Actualiza el estado a muerto para poder reiniciar nivel
            gameState = Status::eDEAD;
            m_pPlayer->setDead(true);
        }
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

void Game::resetLevel() {
    m_pPlayer = new Player(id_new_player++);        //Si no inicias uno nuevo se pierde el sprite de la escopeta porque patata :D
    m_pPlayer->Init(m_pWindow);
    m_pDoomEngine->endProcess();
    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M1", 1);
    actualLevel = 1;
    m_pPauseMenu = new PauseMenu(m_pWindow);
    if (!m_pDoomEngine->Init(m_pWindow, &gameState))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
    }
}

bool Game::Init()
{
    m_pDisplayManager = new DisplayManager();
    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M1", 1);
    m_pWindow = m_pDisplayManager->Init(m_pDoomEngine->GetName());
    m_pPlayer->Init(m_pWindow);
    m_pPauseMenu = new PauseMenu(m_pWindow);
    if (!m_pDoomEngine->Init(m_pWindow, &gameState))
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

int Game::obtenerPorcentajeKills() {
    int total, kills;
    m_pDoomEngine->enemyRecount(total, kills);
    return kills * 100 / total;
}

void Game::loadLevel2() {
    //Change to next lvl
    actualLevel = 2;
    m_pPauseMenu->RenderCarga1(obtenerPorcentajeKills());
    int hp = m_pPlayer->getHp();
    int armor = m_pPlayer->getArmor();
    int ammo = m_pPlayer->getAmmo();
    m_pPlayer = new Player(id_new_player++);        //Si no inicias uno nuevo se pierde el sprite de la escopeta porque patata :D
    m_pPlayer->Init(m_pWindow, hp, armor, ammo);
    m_pDoomEngine->endProcess();
    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M2", 2);
    //m_pPlayer->Init(m_pWindow);
    m_pPauseMenu = new PauseMenu(m_pWindow);
    if (!m_pDoomEngine->Init(m_pWindow, &gameState))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
    }
}


void Game::loadLevel3() {
    //Change to next lvl
    actualLevel = 3;
    int hp = m_pPlayer->getHp();
    int armor = m_pPlayer->getArmor();
    int ammo = m_pPlayer->getAmmo();
    m_pPlayer = new Player(id_new_player++);        //Si no inicias uno nuevo se pierde el sprite de la escopeta porque patata :D
    m_pPlayer->Init(m_pWindow, hp, armor, ammo);
    m_pDoomEngine->endProcess();
    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M3", 3);
    if (!m_pDoomEngine->Init(m_pWindow, &gameState))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
    }
}

void Game::loadEndGame() {
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
                    //std::cout << "Detectada tecla pulsada up" << std::endl;
                    menu.MoveUp();
                    break;

                case sf::Keyboard::Down:
                    shot.play();
                    //std::cout << "Detectada tecla pulsada down" << std::endl;
                    menu.MoveDown();
                    break;

                case sf::Keyboard::Escape:
                    shot.play();
                    //std::cout << "Detectada tecla pulsada ESC" << std::endl;
                    menu.drawIntro(m_pWindow);
                    break;

                case sf::Keyboard::Return:
                    shot.play();
                    //std::cout << "Detectada tecla pulsada Enter" << std::endl;
                    switch (menu.GetPressedItem()) {
                    case 0:     //Entra en Play
                        introMusic.stop();
                        gameState = Status::ePLAYING;
                        m_pPlayer->setVolumenToShoot(this->soundLevel);

                        //Estaria muy bien mover esta parte de código a otro lado:
                        e1m1Music.openFromFile("../../../../assets/Music/E1M1.wav");
                        e1m1Music.setVolume(this->soundLevel);
                        e1m1Music.setLoop(true);
                        e1m1Music.play();
                        return 0;
                        break;

                    case 1:     //Entra en ajustes
                        this->soundLevel = menu.options(m_pWindow, &introMusic, &shot);
                        std::cout << this->soundLevel << std::endl;
                        gameState = Status::eOPTIONS;
                        break;

                    case 2:     //Entra en creditos
                        menu.creditPage(m_pWindow);
                        gameState = Status::eCREDITS;
                        break;

                    case 3:     //Sale del juego
                        std::cout << "Le ha dado a salir Pog" << std::endl;
                        if (menu.confirmarSalir(m_pWindow)) {
                            m_pDoomEngine->Quit();
                            m_pWindow->close();
                        }
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
