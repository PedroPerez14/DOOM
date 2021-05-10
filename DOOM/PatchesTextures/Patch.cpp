/*
* Name: Patch.cpp
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para gestionar los patches, la forma que tiene doom de crear las "texturas del juego", y de gestionar las transparencias.
*/

#include "Patch.h"
#include "../doomdef.h"
#include <iostream>

Patch::Patch(std::string name) : m_name(name)
{
}

Patch::~Patch()
{
}

void Patch::Init(WADPatchHeader& patchHeader, WADPalette& wadPalette)
{
	m_Height = patchHeader.Height;
	m_Width = patchHeader.Width;
	m_XOffset = patchHeader.LeftOffset;
	m_YOffset = patchHeader.TopOffset;

	m_currentPalette = wadPalette;
}

void Patch::Initialize(WADPatchHeader& patchHeader)
{
	m_Height = patchHeader.Height;
	m_Width = patchHeader.Width;
	m_XOffset = patchHeader.LeftOffset;
	m_YOffset = patchHeader.TopOffset;
}

void Patch::AppendPatchColumn(WADPatchColumn& patchColumn)
{
	m_PatchData.push_back(patchColumn);
}

void Patch::Render(uint8_t* pixels, sf::RenderWindow* rw, int iXScreenLocation, int iYScreenLocation)
{
	int iXIndex = 0;
	//sf::Color* _pixels = new sf::Color[SCREENWIDTH * SCREENHEIGHT];
	//uint8_t* pixels = new uint8_t[SCREENWIDTH * SCREENHEIGHT * 4];
	sf::Texture texture;
	texture.create(SCREENWIDTH, SCREENHEIGHT);
	for (size_t iPatchColumnIndex = 0; iPatchColumnIndex < m_PatchData.size(); iPatchColumnIndex++)
	{
		if (m_PatchData[iPatchColumnIndex].TopDelta == 0xff)
		{
			++iXIndex;
			continue;
		}

		for (int iYIndex = 0; iYIndex < m_PatchData[iPatchColumnIndex].Length; ++iYIndex)
		{
			pixels[SCREENWIDTH * (iYScreenLocation + m_PatchData[iPatchColumnIndex].TopDelta + iYIndex) * 4 + (iXScreenLocation + iXIndex) * 4 + 0] = m_currentPalette.Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].r;
			pixels[SCREENWIDTH * (iYScreenLocation + m_PatchData[iPatchColumnIndex].TopDelta + iYIndex) * 4 + (iXScreenLocation + iXIndex) * 4 + 1] = m_currentPalette.Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].g;
			pixels[SCREENWIDTH * (iYScreenLocation + m_PatchData[iPatchColumnIndex].TopDelta + iYIndex) * 4 + (iXScreenLocation + iXIndex) * 4 + 2] = m_currentPalette.Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].b;
			pixels[SCREENWIDTH * (iYScreenLocation + m_PatchData[iPatchColumnIndex].TopDelta + iYIndex) * 4 + (iXScreenLocation + iXIndex) * 4 + 3] = m_currentPalette.Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].a;
		}
	}

	/*for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			pixels[i * 4 + (j * m_Width)] =		(uint8_t)_pixels[i + (j * m_Width)].r;
			pixels[i * 4 + (j * m_Width) + 1] = (uint8_t)_pixels[i + (j * m_Width)].g;
			pixels[i * 4 + (j * m_Width) + 2] = (uint8_t)_pixels[i + (j * m_Width)].b;
			pixels[i * 4 + (j * m_Width) + 3] = (uint8_t)255;
		}
	}*/

	//TODO HACER COSAS
	texture.update(pixels);
	sf::Sprite sprite(texture);
	//sprite.setPosition(0, 0);
	//sprite.setTextureRect(sf::IntRect(0,0, m_Width, m_Height));
	rw->draw(sprite);
	//delete _pixels;
	//_pixels = nullptr;
}

int Patch::getHeight()
{
	return m_Height;
}

int Patch::getWidth()
{
	return m_Width;
}

int Patch::getXOffset()
{
	return m_XOffset;
}

int Patch::getYOffset()
{
	return m_YOffset;
}
