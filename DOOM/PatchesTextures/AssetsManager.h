/*
* Name: AssetsManager.h
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase encargada de cargar y cachear los diferentes patches de texturas del juego (así lo hace el del tutorial)
*/

#pragma once
#include "../WAD/DataTypes.h"
#include "../WAD/wad_loader.h"
#include "Texture.h"
#include "Patch.h"

class AssetsManager
{
public: 
	static AssetsManager* getInstance();
	void Init(WADLoader* pWADLoader, DisplayManager* pDisplayManager);
	void AddPName(const std::string& PName);

	~AssetsManager();

	Patch* AddPatch(const std::string& patchName, WADPatchHeader& patchHeader);
	Patch* GetPatch(const std::string& patchName);

	Texture* AddTexture(WADTextureData& texData);
	Texture* getTexture(const std::string& texName);

	std::string getPName(int pNameIndex);

protected:

	static bool m_bInitialized;
	static std::unique_ptr <AssetsManager> m_pInstance;
	std::map<std::string, std::unique_ptr<Patch>> m_PatchesCache;
	std::map<std::string, std::unique_ptr<Texture>> m_texturesCache;
	WADLoader* m_pWADLoader;
	DisplayManager* m_pDisplayManager;

	AssetsManager();
	void LoadPatch(const std::string& patchName);
	void LoadTextures();

	std::vector<std::string> m_PNameLookup;

};
