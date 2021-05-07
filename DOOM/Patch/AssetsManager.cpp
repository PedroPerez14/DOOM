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

void AssetsManager::Init(WADLoader* pWADLoader)
{
	m_pWADLoader = pWADLoader;
}

AssetsManager::~AssetsManager()
{
}

Patch* AssetsManager::AddPatch(const std::string& patchName, WADPatchHeader& patchHeader)
{
	m_PatchesCache[patchName] = std::unique_ptr<Patch>(new Patch(patchName));
	Patch* pPatch = m_PatchesCache[patchName].get();
	pPatch->Initialize(patchHeader);
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

void AssetsManager::LoadPatch(const std::string& patchName)
{
	m_pWADLoader->LoadPatch(patchName);
}