/*
* Name: Texture.cpp
* Date: 10/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para gestionar las texturas, y componerlas a partir de los patches.
*/

#include "Texture.h"
#include "AssetsManager.h"

Texture::Texture(WADTextureData& texData) : m_isComposed(false), m_overlapSize(0), m_pOverlapColumnData(nullptr)
{
	m_name = texData.TextureName;
	m_Width = texData.Width;
	m_Height = texData.Height;

	for (int i = 0; i < texData.PatchCount; ++i)
	{
		m_texturePatches.push_back(texData.pTexturePatch[i]);
	}

	m_colPatchCount.resize(texData.Width, 0);
	m_colIndex.resize(texData.Width, 0);
	m_colPatch.resize(texData.Width, 0);
}

Texture::~Texture()
{
}

bool Texture::isComposed()
{
	return m_isComposed;
}

bool Texture::Init()
{
	AssetsManager* pAssetsManager = AssetsManager::getInstance();

	for (int i = i; i < m_texturePatches.size(); i++)
	{
		Patch* pPatch = pAssetsManager->GetPatch(pAssetsManager->getPName(m_texturePatches[i].PNameIndex));
		int iXStart = m_texturePatches[i].XOffset;
		int iMaxWidth = iXStart + pPatch->getWidth();

		int iXIndex = iXStart;

		if (iXStart < 0)
		{
			iXIndex = 0;
		}

		if (iMaxWidth > m_Width)
		{
			iMaxWidth = m_Width;
		}

		while (iXIndex < iMaxWidth)
		{
			m_colPatchCount[iXIndex]++;
			m_colPatch[iXIndex] = i;
			m_colIndex[iXIndex] = pPatch->getColumnDataIndex(iXIndex - iXStart);
			++iXIndex;
		}
	}

	for (int i = 0; i < m_Width; ++i)
	{
		if (m_colPatchCount[i] > 1)
		{
			m_colPatch[i] = -1;
			m_colIndex[i] = m_overlapSize;
			m_overlapSize += m_Height;
		}
	}
	return true;
}

bool Texture::Compose()
{
	Init();
	m_pOverlapColumnData = std::unique_ptr<uint8_t[]>(new uint8_t[m_overlapSize]);
	AssetsManager* pAssetsManager = AssetsManager::getInstance();
	for (int i = 0; i < m_texturePatches.size(); i++)
	{
		Patch* pPatch = pAssetsManager->GetPatch(pAssetsManager->getPName(m_texturePatches[i].PNameIndex));
		int iXStart = m_texturePatches[i].XOffset;
		int iMaxWidth = iXStart + pPatch->getWidth();
		int iXIndex = iXStart;

		if (iXStart < 0)
		{
			iXIndex = 0;
		}

		if (iMaxWidth > m_Width)
		{
			iMaxWidth = m_Width;
		}

		while (iXIndex < iMaxWidth)
		{
			if (m_colPatch[iXIndex] < 0)
			{
				int iPatchColIndex = pPatch->getColumnDataIndex(iXIndex - iXStart);

				pPatch->composeColumn(m_pOverlapColumnData.get(), m_Height, iPatchColIndex, m_colIndex[iXIndex], m_texturePatches[i].XOffset);
			}
			++iXIndex;
		}
	}

	m_isComposed = true;
	return m_isComposed;
}

void Texture::Render(uint8_t* buffer, int iXScreenLocation, int iYScreenLocation)
{
	for (size_t iColIndex = 0; iColIndex < m_Width; ++iColIndex)
	{
		RenderColumn(buffer, iXScreenLocation + iColIndex, iYScreenLocation, iColIndex);
	}
}

void Texture::RenderColumn(uint8_t* buffer, int iXScreenLocation, int iYScreenLocation, int iColIndex) {
}