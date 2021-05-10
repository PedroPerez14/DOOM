/*
* Name: Patch.h
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para gestionar los patches, la forma que tiene doom de crear las "texturas del juego", y de gestionar las transparencias.
*/

#pragma once
#include "SFML/Graphics.hpp"
#include "../WAD/DataTypes.h"

class Patch
{
public:
	Patch(std::string name);
	~Patch();

	void Init(WADPatchHeader& patchHeader, WADPalette& wadPalette);
	void Initialize(WADPatchHeader& patchHeader);
	void AppendPatchColumn(WADPatchColumn& patchColumn);
	void Render(uint8_t* pixels, sf::RenderWindow* rw, int iXScreenLocation, int iYScreenLocation);

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
