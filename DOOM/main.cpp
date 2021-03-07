﻿/*
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
	WADLoader wadloader("C:\\Users\\Pedro\\Desktop\\Universidad\\Cuarto\\Videojuegos\\assets_doom\\DOOM.WAD");	//WIP, es el wad de la versión shareware para probar
	wadloader.LoadWAD();
	Map map("E1M1");
	bool potat = wadloader.LoadMapData(map);
	if (potat)
	{
		cout << "PATATAAAAAA" << endl;	//TEST
	}
	int c = getchar();

    /*
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

	return 0;
}
