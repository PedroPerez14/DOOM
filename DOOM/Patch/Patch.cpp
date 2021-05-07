/*
* Name: Patch.cpp
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para gestionar los patches, la forma que tiene doom de crear las "texturas del juego", y de gestionar las transparencias.
*/

#include "Patch.h"

Patch::Patch(std::string name) : m_name(name)
{
}

Patch::~Patch()
{
}

void Patch::Init(WADPatchHeader& patchHeader, sf::RenderWindow* renderwindow, WADPalette& wadPalette)
{
	m_Height = patchHeader.Height;
	m_Width = patchHeader.Width;
	m_XOffset = patchHeader.LeftOffset;
	m_YOffset = patchHeader.TopOffset;
	m_pRenderWindow = renderwindow;

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

void Patch::Render(int iXScreenLocation, int iYScreenLocation)
{
	int iXIndex = 0;
	uint8_t* pixels = new uint8_t[m_Width * m_Height * 4];
	sf::Texture texture;
	texture.create(m_Width, m_Height);
	sf::Sprite sprite(texture);
	for (size_t iPatchColumnIndex = 0; iPatchColumnIndex < m_PatchData.size(); iPatchColumnIndex++)
	{
		if (m_PatchData[iPatchColumnIndex].TopDelta == 0xff)
		{
			++iXIndex;
			continue;
		}

		for (int iYIndex = 0; iYIndex < m_PatchData[iPatchColumnIndex].Length; ++iYIndex)
		{
			//TODO ARREGLAR ESTE DESASTRE xd
			pixels[iXIndex * m_Width + (4 * iYIndex)] = (uint8_t)m_currentPalette.Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].r;
			pixels[iXIndex * m_Width + 4 * iYIndex + 1] = (uint8_t)m_currentPalette.Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].g;
			pixels[iXIndex * m_Width + 4 * iYIndex + 2] = (uint8_t)m_currentPalette.Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].b;
			pixels[iXIndex * m_Width + 4 * iYIndex + 3] = 255;

		}
	}
	//TODO HACER COSAS
	texture.update(pixels);
	m_pRenderWindow->draw(sprite);
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
