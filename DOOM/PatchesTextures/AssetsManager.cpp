/*
* Name: AssetsManager.cpp
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase encargada de cargar y cachear los diferentes patches de texturas del juego (así lo hace el del tutorial)
*/

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
	if (!m_texturesCache.count(texName))
	{
		return nullptr;
	}
	Texture* pTexture = m_texturesCache[texName].get();
	if (!pTexture->isComposed())
	{
		pTexture->Compose();
	}
	return pTexture;
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
