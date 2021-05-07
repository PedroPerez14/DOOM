/*
* Name: DisplayManager.cpp
* Date: 22/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase que se encargará de gestionar la ventana del juego
*/

#include "DisplayManager.h"
#include "../doomdef.h"
#include <iostream>

DisplayManager::DisplayManager()
{
}

DisplayManager::~DisplayManager()
{}

sf::RenderWindow* DisplayManager::Init(const std::string& windowTitle)
{
	m_CurrentPalette = 0;
	m_pWindow = new sf::RenderWindow(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), windowTitle);
	m_pWindow->setSize(sf::Vector2u(SCREENWIDTH, SCREENHEIGHT));
	m_pWindow->setPosition(sf::Vector2i(100, 100));
	if (m_pWindow == nullptr)
	{
		std::cerr << "SFML could not create a window to play DOOM in. Going back to hell..." << std::endl;
		return false;
	}
	return m_pWindow;
}

void DisplayManager::AddColorPalette(WADPalette palette)
{
	m_Palettes.push_back(palette);
}

int DisplayManager::getCurrentPaletteNumber()
{
	return m_CurrentPalette;
}

void DisplayManager::setCurrentPalette(int paletteIndex)
{
	m_CurrentPalette = paletteIndex;
}

WADPalette DisplayManager::getCurrentPalette()
{
	return m_Palettes.at(m_CurrentPalette);
}
