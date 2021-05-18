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

void Patch::Init(WADPatchHeader& patchHeader, DisplayManager* pDisplayManager)
{
	m_Height = patchHeader.Height;
	m_Width = patchHeader.Width;
	m_XOffset = patchHeader.LeftOffset;
	m_YOffset = patchHeader.TopOffset;

	m_pDisplayManager = pDisplayManager;
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

void Patch::AppendColumnStartIndex()
{
	m_columnIndex.push_back(m_PatchData.size());
}

void Patch::Render(uint8_t* pixels, sf::RenderWindow* rw, int iXScreenLocation, int iYScreenLocation)
{
	int iXIndex = 0;
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
			pixels[SCREENWIDTH * (iYScreenLocation + m_PatchData[iPatchColumnIndex].TopDelta + iYIndex) * 4 + (iXScreenLocation + iXIndex) * 4 + 0] = m_pDisplayManager->getCurrentPalette().Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].r;
			pixels[SCREENWIDTH * (iYScreenLocation + m_PatchData[iPatchColumnIndex].TopDelta + iYIndex) * 4 + (iXScreenLocation + iXIndex) * 4 + 1] = m_pDisplayManager->getCurrentPalette().Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].g;
			pixels[SCREENWIDTH * (iYScreenLocation + m_PatchData[iPatchColumnIndex].TopDelta + iYIndex) * 4 + (iXScreenLocation + iXIndex) * 4 + 2] = m_pDisplayManager->getCurrentPalette().Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].b;
			pixels[SCREENWIDTH * (iYScreenLocation + m_PatchData[iPatchColumnIndex].TopDelta + iYIndex) * 4 + (iXScreenLocation + iXIndex) * 4 + 3] = m_pDisplayManager->getCurrentPalette().Colors[m_PatchData[iPatchColumnIndex].pColumnData[iYIndex]].a;
		}
	}

	//TODO HACER COSAS
	texture.update(pixels);
	sf::Sprite sprite(texture);
	rw->draw(sprite);
}

void Patch::composeColumn(uint8_t* pOverLapColumnData, int iHeight, int& iPatchColumnIndex, int iColumnOffsetIndex, int iYOrigin)
{
	while (m_PatchData[iPatchColumnIndex].TopDelta != 0xFF)
	{
		int iYPosition = iYOrigin + m_PatchData[iPatchColumnIndex].TopDelta;
		int iMaxRun = m_PatchData[iPatchColumnIndex].Length;

		if (iYPosition < 0)
		{
			iMaxRun += iYPosition;
			iYPosition = 0;
		}

		if (iYPosition + iMaxRun > iHeight)
		{
			iMaxRun = iHeight - iYPosition;
		}

		for (int iYIndex = 0; iYIndex < iMaxRun; ++iYIndex)
		{
			pOverLapColumnData[iColumnOffsetIndex + iYPosition + iYIndex] = m_PatchData[iPatchColumnIndex].pColumnData[iYIndex];
		}
		++iPatchColumnIndex;
	}
}

void Patch::RenderColumn(uint8_t* buffer, int iColumn, int iXScreenLocation, int iYScreenLocation, int iMaxHeight, int iYOffset)
{
	int iTotalHeight = 0;
	int iYIndex = 0;

	if (iYOffset < 0)
	{
		iYIndex = -iYOffset;
	}
	while(m_PatchData[iColumn].TopDelta != 0xFF && iTotalHeight < iMaxHeight)
	{
		while (iYIndex < m_PatchData[iColumn].Length && iTotalHeight < iMaxHeight)
		{
			buffer[SCREENWIDTH * (iYScreenLocation + m_PatchData[iColumn].TopDelta + iYIndex + iYOffset) * 4 + (iXScreenLocation * 4) + 0] = m_pDisplayManager->getCurrentPalette().Colors[m_PatchData[iColumn].pColumnData[iYIndex]].r;
			buffer[SCREENWIDTH * (iYScreenLocation + m_PatchData[iColumn].TopDelta + iYIndex + iYOffset) * 4 + (iXScreenLocation * 4) + 1] = m_pDisplayManager->getCurrentPalette().Colors[m_PatchData[iColumn].pColumnData[iYIndex]].g;
			buffer[SCREENWIDTH * (iYScreenLocation + m_PatchData[iColumn].TopDelta + iYIndex + iYOffset) * 4 + (iXScreenLocation * 4) + 2] = m_pDisplayManager->getCurrentPalette().Colors[m_PatchData[iColumn].pColumnData[iYIndex]].b;
			buffer[SCREENWIDTH * (iYScreenLocation + m_PatchData[iColumn].TopDelta + iYIndex + iYOffset) * 4 + (iXScreenLocation * 4) + 3] = m_pDisplayManager->getCurrentPalette().Colors[m_PatchData[iColumn].pColumnData[iYIndex]].a;
			++iTotalHeight;
			++iYIndex;
		}
		++iColumn;
		iYIndex = 0;
	}
}

uint8_t Patch::getTexel(int u, int v, bool& transp)
{
	if (m_PatchData[u].TopDelta != 0xFF)
	{
		return m_PatchData[u].pColumnData[v];
	}
	else
	{
		transp = true;
		return 0;
	}
}

uint8_t* Patch::getColumn(int iColumn)
{
	return m_PatchData[iColumn % m_PatchData.size()].pColumnData;
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

int Patch::getColumnDataIndex(int iIndex)
{
	return m_columnIndex[iIndex];
}

std::string Patch::getPName()
{
	return m_name;
}
