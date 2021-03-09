/*
* Name: main.cpp
* Date: 03/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Punto de entrada del juego DOOM.
*/
#pragma once
#include "DOOM.h"
#include <SFML/Window.hpp>
using namespace std;

int main()
{
    DoomEngine d_eng;
    d_eng.Init();
    sf::VideoMode vm(SCREENWIDTH, SCREENHEIGHT);
    sf::RenderWindow* rw = new sf::RenderWindow(vm, "DOOM");

    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color(255, 10, 10));
    shape.setPosition(60, 60);

    while (rw->isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (rw->pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                rw->setView(sf::View(visibleArea));
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                rw->close();
        }
        rw->clear(sf::Color::Green);
        rw->draw(shape);
        rw->display();
        //d_eng.Render(rw);
    }

    /*
	WADLoader wadloader("C:\\Users\\Pedro\\Desktop\\Universidad\\Cuarto\\Videojuegos\\assets_doom\\DOOM.WAD");	//WIP, es el wad de la versión shareware para probar
	wadloader.LoadWAD();
	Map map("E1M1");
	bool funciona = wadloader.LoadMapData(map);
	if (funciona)
	{
		cout << "funciona" << endl;	//TEST
	}
	int c = getchar();
    */
    /*
    * // PRUEBA DE QUE SFML CHUTA, FUNCIONA EN MI ORDENADOR AUNQUE HA COSTADO
    sf::Window window(sf::VideoMode(800, 600), "My window");
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    */
    //Voy a tratar de poner cómo creo que debería ser el main que de entrada al gameloop
    //sacado de https://github.com/SFML/SFML/wiki/Tutorial:-Basic-Game-Engine más o menos

    //Game doom = new Game();
    //Hacer un assert de que no es null
    //exitCode = doom->Run();   //Aquí debería estar el gameloop
    //delete doom;
    //doom = NULL;
    //return exitCode;

	return 0;
}
