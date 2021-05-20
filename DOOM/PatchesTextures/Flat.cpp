/*
* Name: Flat.cpp
* Date: 20/05/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase encargada de implementar los flats, o una versi�n cutre.
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
	return m_colorInPalette;	//Versi�n barata y r�pida de hacer, ya veremos si hay tiempo para hacerlo bien pero parece que no
}