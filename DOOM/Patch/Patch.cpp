/*
* Name: Patch.cpp
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para gestionar los patches, la forma que tiene doom de crear las "texturas del juego", y de gestionar las transparencias.


#include "Patch.h"
#include <rpcndr.h>

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

void Patch::AppendPatchColumn(WADPatchColumn& patchColumn)
{
	m_PatchData.push_back(patchColumn);
}

void Patch::Render(int iBufferPitch, int iXScreenLocation, int iYScreenLocation)
{
	int iXIndex = 0;
	byte* imagePixels = new byte[m_Width * m_Height * 8];
	sf::Image* drawn = new sf::Image();
	for (size_t iPatchColumnIndex = 0; iPatchColumnIndex < m_PatchData.size(); iPatchColumnIndex++)
	{
		if (m_PatchData[iPatchColumnIndex].TopDelta == 0xff)
		{
			++iXIndex;
			continue;
		}

		for (int iYIndex = 0; iYIndex < m_PatchData[iPatchColumnIndex].Length; ++iYIndex)
		{
			imagePixels[iXIndex + iYScreenLocation + m_PatchData[iPatchColumnIndex].Length + iYIndex] = m_currentPalette[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]];
		}
	}
	drawn->create((unsigned int)m_Width, (unsigned int)m_Height, imagePixels);
}*/