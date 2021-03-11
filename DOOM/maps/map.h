/*
* Name: map.h
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Mapas de DOOM.
*/

#pragma once

#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include "map_types.h"
#include "../Player/Player.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Map
{
public:
	Map(std::string n, Player *player);
	~Map();

	std::string GetName();				//Devuelve el nombre del mapa
	void addVertex(Vertex& v);			//A�ade un v�rtice [x,y] al mapa
	void addLinedef(Linedef& l);		//A�ade un linedef al mapa
	void addThing(Thing& th);			//A�ade un nuevo thing al mapa

	Linedef getLinedef(const int& n);	//Devuelve el n-�simo linedef
	Vertex getVertex(const int& n);		//Devuelve el n-�simo vertex

	int getLinedefsSize();				//Tama�o del vector
	int getVertexSize();				//Tama�o del vector
	int getThingsSize();				//Tama�o del vector de things

	void Automap(sf::RenderWindow* r_window);//Dibuja el mapa 2D en pantalla
	void LoadPlayer();					//Establece la posici�n inicial del jugador

	int getMapIndex();					//Devuelve el �ndice del mapa
	void setMapIndex(int idx);			//Fija el �ndice del mapa

protected:
	int map_index;						//�ndice del mapa en los lumps, para acelerar la b�squeda
	std::string name;					//nombre del mapa
	std::vector<Vertex> map_verts;		//V�rtices del mapa
	std::vector<Linedef> map_linedefs;	//Linedefs del mapa
	std::vector<Thing> map_things;		//Things del mapa
	Automap_info automap_info;			//Informaci�n necesaria para hacer automap correctamente
	Player* m_pPlayer;					//Referencia al jugador y su personaje

	void AutomapPlayer(sf::RenderWindow* r_window);		//Auxiliar a automap, pinta al jugador
	void AutomapWalls(sf::RenderWindow* r_window);		//Auxiliar a automap, pinta el mapeado

};

#endif