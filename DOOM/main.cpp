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
    Game doom_game;
    doom_game.Init();

    while (!doom_game.IsOver())
    {
        doom_game.ProcessInput();   //A medio hacer
        doom_game.Update();         //TODO
        doom_game.Render();         //WIP
        //doom_game.Delay();        //TODO Sin implementar de momento
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
