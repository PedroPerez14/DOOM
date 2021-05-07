/*
* Name: Patch.h
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Clase para gestionar los patches, la forma que tiene doom de crear las "texturas del juego", y de gestionar las transparencias.


#pragma once
#include "SFML/Graphics.hpp"
#include "../WAD/DataTypes.h"

class Patch
{
public:
	Patch(std::string name);
	~Patch();

	void Init(WADPatchHeader& patchHeader, sf::RenderWindow* m_pRenderWindow, WADPalette& wadPalette);
	void AppendPatchColumn(WADPatchColumn& patchColumn);
	void Render(int iBufferPitch, int iXScreenLocation, int iYScreenLocation);

	int getHeight();
	int getWidth();
	int getXOffset();
	int getYOffset();

protected:

	std::string m_name;						//El nombre de ese patch en el WAD

	uint16_t m_Width;
	uint16_t m_Height;
	int16_t m_XOffset;
	int16_t m_YOffset;

	sf::RenderWindow* m_pRenderWindow;
	std::vector<WADPatchColumn> m_PatchData;
	WADPalette m_currentPalette;
};
*/