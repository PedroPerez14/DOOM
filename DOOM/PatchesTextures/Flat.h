/*
* Name: Falt.h
* Date: 20/05/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase que se encargar� de representar las texturas de los flats, o
*		una versi�n muy reducida de estas para llegar a tiempo a la entrega.
*/
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Flat
{
public:
	Flat(std::string name, uint8_t color);
	~Flat();
	void Init();

	uint8_t getTexel(int u, int v);		//Probablemente se va a quedar en "return este color prefijado" pero so�ar es gratis
protected:

	std::string m_name;
	uint8_t m_colorInPalette;				//Vers��n barata
	//std::vector<std::vector<uint8_t>> m_flatColors;
};
