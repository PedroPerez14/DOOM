/*
* Name: Flat.cpp
* Date: 20/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase encargada de implementar los flats, o una versión cutre.
*		Un flat es la textura que se pone en el techo / suelo.
*/
#pragma once
#include "Flat.h"

Flat::Flat(std::string name, uint8_t color)
{
	m_name = name;
	m_colorInPalette = color;
}

Flat::~Flat()
{

}

void Flat::Init()
{

}

uint8_t Flat::getTexel(int u, int v)
{
	return m_colorInPalette;	//Versión barata y rápida de hacer, ya veremos si hay tiempo para hacerlo bien pero parece que no
}