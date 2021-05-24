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
#include <thread>

#include "Windows.h"

Game::Game()
{
    id_new_player = 0;
    soundLevel = 100;
    m_pPlayer = new Player(id_new_player++);
    actualLevel = 1;

    if (!introMusic.openFromFile("assets/MainMenu/MainMenuMusic.wav"))
        std::cout << "Error al cargar music en mainMenu" << std::endl;
    introMusic.setLoop(true);
    isOnNightmare = false;
}

Game::~Game(){}

void Game::ProcessInput(Status status)
{
    sf::Event event;
    while (m_pWindow->pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Resized:    //Hay que capturarlo porque sino pum y explota
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

void deleteLastsSoldiers(std::vector<Soldier*> lista) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    for (auto a : lista) {
        delete a;
    }
}

void Game::resetLevel() {
    m_pDoomEngine->killEverything();

    std::vector<Soldier*> listaAux = m_pDoomEngine->getEnemyList();
    std::thread eliminacionFuturaEnemigos(deleteLastsSoldiers, listaAux);
    eliminacionFuturaEnemigos.detach();
    sf::Clock deltaClock;
    sf::Time t1 = deltaClock.restart();

    e1m1Music.stop();
    e1m2Music.stop();
    e1m3Music.stop();

    sf::Time elapsed = deltaClock.getElapsedTime();
    float timeToWait = 6.0f - elapsed.asSeconds();
    if (timeToWait > 0)
    {
        sf::sleep(sf::seconds(timeToWait));
    }

    m_pPlayer = new Player(id_new_player++);        //Si no inicias uno nuevo se pierde el sprite de la escopeta porque patata :D
    m_pPlayer->Init(m_pWindow, isOnNightmare);
    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M1", 1);
    actualLevel = 1;
    if (!m_pDoomEngine->Init(m_pWindow))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
    }
    m_pDoomEngine->InitEnemy(&gameState, dificultad);
    m_pDoomEngine->initVolumenes(soundLevel);
    e1m1Music.play();
}

bool Game::Init()
{
    m_pDisplayManager = new DisplayManager();
    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M1", 1);
    m_pWindow = m_pDisplayManager->Init(m_pDoomEngine->GetName());
    m_pPlayer->Init(m_pWindow, false);
    m_pPauseMenu = new PauseMenu(m_pWindow);
    if (!m_pDoomEngine->Init(m_pWindow))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
        return false;
    }
    
	//TODO estaría bien hacer que renderice solo a 320 x 200 : No hace falta, asi esta guay
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

            visibleArea = sf::FloatRect(0, 0, SCREENWIDTH, SCREENHEIGHT);
            m_pWindow->setView(sf::View(visibleArea));
        }
        else
        {
            v = sf::Vector2u(desktop_height * _FACTOR * ratio, desktop_height * _FACTOR);
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

            visibleArea = sf::FloatRect(0, 0, SCREENWIDTH, SCREENHEIGHT);
            m_pWindow->setView(sf::View(visibleArea));
        }
        else
        {
            v = sf::Vector2u(desktop_height * _FACTOR * ratio, desktop_height * _FACTOR);
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
    //Calcula las estadisticas e inicia un proceso para eliminar entidades enemigo
    int porcentaje = obtenerPorcentajeKills();
    m_pDoomEngine->killEverything();

    std::vector<Soldier*> listaAux = m_pDoomEngine->getEnemyList();
    std::thread eliminacionFuturaEnemigos(deleteLastsSoldiers, listaAux);
    eliminacionFuturaEnemigos.detach();
    sf::Clock deltaClock;
    sf::Time t1= deltaClock.restart();

    //Se muestra todo y se empieza a cargar el nivel
    e1m1Music.stop();
    intermissionMusic.play();
    actualLevel = 2;
    m_pPauseMenu->RenderCarga1(porcentaje);

    int hp = m_pPlayer->getHp();
    int armor = m_pPlayer->getArmor();
    int ammo = m_pPlayer->getAmmo();

    if (!isOnNightmare) { armor += 125; hp += 50;  ammo += 30; }

    if (armor > 200) armor = 200;
    if (ammo > 200) ammo = 200;
    if (hp > 200) hp = 200;

    //Si aun no ha terminado el proceso de limpieza de enemigos, se espera
    sf::Time elapsed = deltaClock.getElapsedTime();
    float timeToWait = 6.0f - elapsed.asSeconds();
    if (timeToWait > 0)
    {
        sf::sleep(sf::seconds(timeToWait));
    }
    delete m_pPlayer;
    delete m_pDoomEngine;

    m_pPlayer = new Player(id_new_player++);        //Si no inicias uno nuevo se pierde el sprite de la escopeta porque patata :D
    m_pPlayer->Init(m_pWindow, hp, armor, ammo, isOnNightmare);

    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M2", 2);
    if (!m_pDoomEngine->Init(m_pWindow))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
    }
    m_pDoomEngine->InitEnemy(&gameState, dificultad);
    m_pDoomEngine->initVolumenes(soundLevel);

    intermissionMusic.stop();
    e1m2Music.play();
}


void Game::loadLevel3() {
    //Change to next lvl. Los mismos pasos que en la funcion de arriba.
    int porcentajek = obtenerPorcentajeKills();
    m_pDoomEngine->killEverything();

    std::vector<Soldier*> listaAux = m_pDoomEngine->getEnemyList();
    std::thread eliminacionFuturaEnemigos(deleteLastsSoldiers, listaAux);
    eliminacionFuturaEnemigos.detach();
    sf::Clock deltaClock;
    sf::Time t1 = deltaClock.restart();

    e1m2Music.stop();
    intermissionMusic.play();
    actualLevel = 3;
    m_pPauseMenu->RenderCarga2(porcentajek);
    int hp = m_pPlayer->getHp();
    int armor = m_pPlayer->getArmor();    
    int ammo = m_pPlayer->getAmmo();

    if (!isOnNightmare) { armor += 125; hp += 50;  ammo += 30; }

    if (armor > 200) armor = 200;
    if (ammo > 200) ammo = 200;
    if (hp > 200) hp = 200;

    sf::Time elapsed = deltaClock.getElapsedTime();
    float timeToWait = 6.0f - elapsed.asSeconds();
    if (timeToWait > 0)
    {
        sf::sleep(sf::seconds(timeToWait));
    }

    delete m_pPlayer;
    delete m_pDoomEngine;

    m_pPlayer = new Player(id_new_player++);        //Si no inicias uno nuevo se pierde el sprite de la escopeta porque patata :D
    m_pPlayer->Init(m_pWindow, hp, armor, ammo, isOnNightmare);

    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M3", 3);
    if (!m_pDoomEngine->Init(m_pWindow))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
    }
    m_pDoomEngine->InitEnemy(&gameState, dificultad);
    m_pDoomEngine->initVolumenes(soundLevel);

    intermissionMusic.stop();
    e1m3Music.play();
}

void Game::loadEndGame() {
    //Change to next lvl Lo mismo, funcion de arriba solo que esta vez se inicia el 1 mientras pantalla fin
    int porcentajek = obtenerPorcentajeKills();
    m_pDoomEngine->killEverything();

    std::vector<Soldier*> listaAux = m_pDoomEngine->getEnemyList();
    std::thread eliminacionFuturaEnemigos(deleteLastsSoldiers, listaAux);
    eliminacionFuturaEnemigos.detach();
    sf::Clock deltaClock;
    sf::Time t1 = deltaClock.restart();

    e1m3Music.stop();
    intermissionMusic.play();
    actualLevel = 1;
    m_pPauseMenu->RenderCarga3(porcentajek);
    m_pPauseMenu->RenderEnd();

    sf::Time elapsed = deltaClock.getElapsedTime();
    float timeToWait = 6.0f - elapsed.asSeconds();
    if (timeToWait > 0)
    {
        sf::sleep(sf::seconds(timeToWait));
    }

    delete m_pPlayer;
    delete m_pDoomEngine;

    m_pPlayer = new Player(id_new_player++);        //Si no inicias uno nuevo se pierde el sprite de la escopeta porque patata :D
    m_pPlayer->Init(m_pWindow, false);
    
    m_pDoomEngine = new DoomEngine(m_pPlayer, m_pDisplayManager, "E1M1", 1);
    if (!m_pDoomEngine->Init(m_pWindow))
    {
        std::cerr << "Could not rip and tear (initialize) the engine!" << std::endl;
    }
    m_pDoomEngine->InitEnemy(&gameState, dificultad);
    m_pDoomEngine->initVolumenes(soundLevel);

    intermissionMusic.stop();
    gameState = Status::eMAINMENU;

}



int Game::mainMenu()
{
    //Start the music and the loop on the menu:
    introMusic.play();

    //Load songs of menu
    sf::SoundBuffer shotBuffer;
    sf::Sound shot;
    if (!shotBuffer.loadFromFile("assets/MainMenu/shot.wav"))
        std::cout << "Error al cargar audio de tiro en mainMenu" << std::endl;
    shot.setBuffer(shotBuffer);
    int dificultad_;
    
    //Create the menu itself
    Menu menu((float)m_pWindow->getView().getSize().x, (float)m_pWindow->getView().getSize().y, m_pDoomEngine);
    menu.drawIntro(m_pWindow);

    while (m_pWindow->isOpen()) {
        sf::Event event;

        while (m_pWindow->pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Resized:
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Up:
                    shot.play();
                    menu.MoveUp();
                    break;

                case sf::Keyboard::Down:
                    shot.play();
                    menu.MoveDown();
                    break;

                case sf::Keyboard::Escape:
                    shot.play();
                    menu.drawIntro(m_pWindow);
                    break;

                case sf::Keyboard::Return:
                    shot.play();
                    switch (menu.GetPressedItem()) {
                    case 0:     //Entra en Play
                       dificultad_ = menu.selectDificultad(m_pWindow, &shot);
                        if (dificultad_ != -1) {
                            dificultad = dificultad_;
                            if (dificultad == 3) {
                                isOnNightmare = true; 
                                m_pPlayer->setNightmare(true);
                            }
                            introMusic.stop();
                            gameState = Status::ePLAYING;
                            m_pPlayer->setVolumenToShoot(this->soundLevel);
                            m_pPauseMenu->setVolumenes(soundLevel);

                            //Estaria muy bien mover esta parte de código a otro lado:
                            e1m1Music.openFromFile("assets/Music/E1M1.wav");
                            e1m1Music.setVolume(this->soundLevel);
                            e1m1Music.setLoop(true);
                            e1m1Music.play();

                            e1m2Music.openFromFile("assets/Music/E1M2.wav");
                            e1m2Music.setVolume(this->soundLevel);
                            e1m2Music.setLoop(true);

                            e1m3Music.openFromFile("assets/Music/E1M3.wav");
                            e1m3Music.setVolume(this->soundLevel);
                            e1m3Music.setLoop(true);

                            intermissionMusic.openFromFile("assets/Music/intermissionMusic.wav");
                            intermissionMusic.setVolume(this->soundLevel);
                            intermissionMusic.setLoop(true);


                            m_pDoomEngine->InitEnemy(&gameState, dificultad);
                            m_pDoomEngine->initVolumenes(soundLevel);
                            return 0;
                        }
                        break;

                    case 1:     //Entra en ajustes
                        this->soundLevel = menu.options(m_pWindow, &introMusic, &shot);
                        gameState = Status::eOPTIONS;
                        break;

                    case 2:     //Entra en creditos
                        menu.creditPage(m_pWindow);
                        gameState = Status::eCREDITS;
                        break;

                    case 3:     //Sale del juego
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
        m_pWindow->clear(sf::Color(0, 0, 0));

        menu.draw(m_pWindow);

        m_pWindow->display();
    }
    
    return 0;
}
