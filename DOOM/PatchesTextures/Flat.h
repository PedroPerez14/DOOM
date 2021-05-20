/*
* Name: Falt.h
* Date: 20/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que se encargará de representar las texturas de los flats, o
*		una versión muy reducida de estas para llegar a tiempo a la entrega.
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

	uint8_t getTexel(int u, int v);		//Probablemente se va a quedar en "return este color prefijado" pero soñar es gratis
protected:

	std::string m_name;
	uint8_t m_colorInPalette;				//Versíón barata
	//std::vector<std::vector<uint8_t>> m_flatColors;
};
