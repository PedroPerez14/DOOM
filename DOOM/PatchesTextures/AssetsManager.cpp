/*
* Name: AssetsManager.cpp
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase encargada de cargar y cachear los diferentes patches de texturas del juego (así lo hace el del tutorial)
*/
#pragma once

#include "AssetsManager.h"

bool AssetsManager::m_bInitialized = false;
std::unique_ptr<AssetsManager> AssetsManager::m_pInstance = nullptr;

AssetsManager::AssetsManager()
{
}

//Mi primer singleton en C++, nice
AssetsManager* AssetsManager::getInstance()
{
	if (!m_bInitialized)
	{
		m_pInstance = std::unique_ptr<AssetsManager>(new AssetsManager());
		m_bInitialized = true;
	}
	return m_pInstance.get();
}

void AssetsManager::Init(WADLoader* pWADLoader, DisplayManager* pDisplayManager)
{
	m_pWADLoader = pWADLoader;
	m_pDisplayManager = pDisplayManager;
	LoadTextures();			//no se puede llamar hasta que tenga pWADLoader
	//Llenar la caché de flats, de momento a mano, si hay tiempo versión bien
	//. . .
	m_flatsCache["TLITE6_6"] = std::unique_ptr<Flat>(new Flat("TLITE6_6", 166));
	m_flatsCache["TLITE6_5"] = std::unique_ptr<Flat>(new Flat("TLITE6_5", 43));
	m_flatsCache["TLITE6_4"] = std::unique_ptr<Flat>(new Flat("TLITE6_4", 69));
	m_flatsCache["TLITE6_1"] = std::unique_ptr<Flat>(new Flat("TLITE6_1", 11));
	m_flatsCache["STEP2"] = std::unique_ptr<Flat>(new Flat("STEP2", 104));
	m_flatsCache["STEP1"] = std::unique_ptr<Flat>(new Flat("STEP1", 110));
	m_flatsCache["NUKAGE3"] = std::unique_ptr<Flat>(new Flat("NUKAGE3", 124));
	m_flatsCache["NUKAGE2"] = std::unique_ptr<Flat>(new Flat("NUKAGE2", 118));
	m_flatsCache["NUKAGE1"] = std::unique_ptr<Flat>(new Flat("NUKAGE1", 120));
	m_flatsCache["MFLR8_1"] = std::unique_ptr<Flat>(new Flat("MFLR8_1", 107));
	m_flatsCache["F_SKY1"] = std::unique_ptr<Flat>(new Flat("F_SKY1", 136));
	m_flatsCache["FLOOR7_2"] = std::unique_ptr<Flat>(new Flat("FLOOR7_2", 159));
	m_flatsCache["FLOOR7_1"] = std::unique_ptr<Flat>(new Flat("FLOOR7_1", 77));
	m_flatsCache["FLOOR6_2"] = std::unique_ptr<Flat>(new Flat("FLOOR6_2", 106));
	m_flatsCache["FLOOR6_1"] = std::unique_ptr<Flat>(new Flat("FLOOR6_1", 189));
	m_flatsCache["FLOOR5_4"] = std::unique_ptr<Flat>(new Flat("FLOOR5_4", 74));
	m_flatsCache["FLOOR5_3"] = std::unique_ptr<Flat>(new Flat("FLOOR5_3", 75));
	m_flatsCache["FLOOR5_2"] = std::unique_ptr<Flat>(new Flat("FLOOR5_2", 75));
	m_flatsCache["FLOOR5_1"] = std::unique_ptr<Flat>(new Flat("FLOOR5_1", 109));
	m_flatsCache["FLOOR4_8"] = std::unique_ptr<Flat>(new Flat("FLOOR4_8", 111));
	m_flatsCache["FLOOR4_6"] = std::unique_ptr<Flat>(new Flat("FLOOR4_6", 140));
	m_flatsCache["FLOOR4_5"] = std::unique_ptr<Flat>(new Flat("FLOOR4_5", 155));
	m_flatsCache["FLOOR4_1"] = std::unique_ptr<Flat>(new Flat("FLOOR4_1", 147));
	m_flatsCache["FLOOR3_3"] = std::unique_ptr<Flat>(new Flat("FLOOR3_3", 137));
	m_flatsCache["FLOOR1_7"] = std::unique_ptr<Flat>(new Flat("FLOOR1_7", 188));
	m_flatsCache["FLOOR1_1"] = std::unique_ptr<Flat>(new Flat("FLOOR1_1", 206));
	m_flatsCache["FLOOR0_6"] = std::unique_ptr<Flat>(new Flat("FLOOR0_6", 102));
	m_flatsCache["FLOOR0_3"] = std::unique_ptr<Flat>(new Flat("FLOOR0_3", 102));
	m_flatsCache["FLOOR0_1"] = std::unique_ptr<Flat>(new Flat("FLOOR0_1", 139));
	m_flatsCache["FLAT5_5"] = std::unique_ptr<Flat>(new Flat("FLAT5_5", 141));
	m_flatsCache["FLAT5_4"] = std::unique_ptr<Flat>(new Flat("FLAT5_4", 104));
	m_flatsCache["FLAT5"] = std::unique_ptr<Flat>(new Flat("FLAT5", 74));
	m_flatsCache["FLAT23"] = std::unique_ptr<Flat>(new Flat("FLAT23", 99));
	m_flatsCache["FLAT22"] = std::unique_ptr<Flat>(new Flat("FLAT22", 204));
	m_flatsCache["FLAT20"] = std::unique_ptr<Flat>(new Flat("FLAT20", 100));
	m_flatsCache["FLAT2"] = std::unique_ptr<Flat>(new Flat("FLAT2", 101));
	m_flatsCache["FLAT18"] = std::unique_ptr<Flat>(new Flat("FLAT18", 100));
	m_flatsCache["FLAT14"] = std::unique_ptr<Flat>(new Flat("FLAT14", 206));
	m_flatsCache["FLAT10"] = std::unique_ptr<Flat>(new Flat("FLAT10", 79));
	m_flatsCache["FLAT1"] = std::unique_ptr<Flat>(new Flat("FLAT1", 109));
	m_flatsCache["FLAT10"] = std::unique_ptr<Flat>(new Flat("FLAT10", 79));
	m_flatsCache["DEM1_4"] = std::unique_ptr<Flat>(new Flat("DEM1_4", 155));
	m_flatsCache["DEM1_3"] = std::unique_ptr<Flat>(new Flat("DEM1_3", 155));
	m_flatsCache["DEM1_2"] = std::unique_ptr<Flat>(new Flat("DEM1_2", 155));
	m_flatsCache["DEM1_1"] = std::unique_ptr<Flat>(new Flat("DEM1_1", 155));
	m_flatsCache["CONS1_7"] = std::unique_ptr<Flat>(new Flat("CONS1_7", 101));
	m_flatsCache["CONS1_5"] = std::unique_ptr<Flat>(new Flat("CONS1_5", 101));
	m_flatsCache["CONS1_1"] = std::unique_ptr<Flat>(new Flat("CONS1_1", 101));
	m_flatsCache["CEIL5_2"] = std::unique_ptr<Flat>(new Flat("CEIL5_2", 78));
	m_flatsCache["CEIL5_1"] = std::unique_ptr<Flat>(new Flat("CEIL5_1", 5));
	m_flatsCache["CEIL4_3"] = std::unique_ptr<Flat>(new Flat("CEIL4_3", 244));
	m_flatsCache["CEIL4_2"] = std::unique_ptr<Flat>(new Flat("CEIL4_2", 245));
	m_flatsCache["CEIL3_5"] = std::unique_ptr<Flat>(new Flat("CEIL3_5", 108));
	m_flatsCache["CEIL3_2"] = std::unique_ptr<Flat>(new Flat("CEIL3_2", 149));
	m_flatsCache["CEIL3_1"] = std::unique_ptr<Flat>(new Flat("CEIL3_1", 74));
}

void AssetsManager::AddPName(const std::string& PName)
{
	m_PNameLookup.push_back(PName);
}

AssetsManager::~AssetsManager()
{
}

Patch* AssetsManager::AddPatch(const std::string& patchName, WADPatchHeader& patchHeader)
{
	m_PatchesCache[patchName] = std::unique_ptr<Patch>(new Patch(patchName));
	Patch* pPatch = m_PatchesCache[patchName].get();
	pPatch->Init(patchHeader, m_pDisplayManager);
	m_currentPalette = m_pDisplayManager->getCurrentPalette();
	return pPatch;
}

Patch* AssetsManager::GetPatch(const std::string& patchName)
{
	if (m_PatchesCache.count(patchName) > 0)
	{
		return m_PatchesCache[patchName].get();
	}
	LoadPatch(patchName);
	return m_PatchesCache[patchName].get();
}

Texture* AssetsManager::AddTexture(WADTextureData& texData)
{
	m_texturesCache[texData.TextureName] = std::unique_ptr<Texture>(new Texture(texData, m_pDisplayManager));
	Texture* pTexture = m_texturesCache[texData.TextureName].get();
	return pTexture;
}

Texture* AssetsManager::getTexture(const std::string& texName)
{
	std::string texNameUpper = texName;

	if (!m_texturesCache.count(texNameUpper))
	{
		return nullptr;
	}
	Texture* pTexture = m_texturesCache[texNameUpper].get();
	if (!pTexture->isComposed())
	{
		pTexture->Compose();
	}
	return pTexture;
}

uint8_t AssetsManager::getFlatColor(std::string flatname)
{
	if (!m_flatsCache.count(flatname))
	{
		return 0;
	}
	else //Debería ser este
	{
		return m_flatsCache[flatname]->getTexel(0,0);		//Versión barata de programar
	}
}

std::string AssetsManager::getPName(int pNameIndex)
{
	return m_PNameLookup[pNameIndex];
}

void AssetsManager::LoadPatch(const std::string& patchName)
{
	m_pWADLoader->LoadPatch(patchName);
}

void AssetsManager::LoadTextures()
{
	m_pWADLoader->LoadPNames();
	m_pWADLoader->LoadTextures("TEXTURE1");
	m_pWADLoader->LoadTextures("TEXTURE2");	//Si se tiene el wad de shareware no existe, no debería crashear *en teoría*
}
