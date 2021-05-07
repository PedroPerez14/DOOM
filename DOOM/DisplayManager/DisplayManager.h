/*
* Name: DisplayManager.h
* Date: 22/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que se encargará de gestionar la ventana del juego
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "../WAD/DataTypes.h"

class DisplayManager
{
public:
	DisplayManager();
	~DisplayManager();

	sf::RenderWindow* Init(const std::string& windowTitle);
	void AddColorPalette(WADPalette palette);

	int getCurrentPaletteNumber();
	void setCurrentPalette(int paletteIndex);
	WADPalette getCurrentPalette();

protected:
	sf::RenderWindow* m_pWindow;		//Referencia a la ventana del juego
	std::vector<WADPalette> m_Palettes;	//Las 14 paletas de colores cargadas en memoria
	int m_CurrentPalette;
};
