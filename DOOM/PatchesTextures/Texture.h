/*
* Name: Texture.h
* Date: 10/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para gestionar las texturas, y componerlas a partir de los patches.
*/

#pragma once
#include "../WAD/DataTypes.h"
#include "../DisplayManager/DisplayManager.h"

class Texture
{
public:
	Texture(WADTextureData& texData, DisplayManager* manager);
	~Texture();

	bool isComposed();
	bool Init();
	bool Compose();

	//Renderiza la textura en la posición X,Y de pantalla
	void Render(uint8_t* buffer, int iXScreenLocation, int iYScreenLocation);

	//Renderiza una columna de la textura en la posición X,Y de la pantalla
	void RenderColumn(uint8_t* buffer, int iXScreenLocation, int iYScreenLocation, int iColIndex);

	//TODO comprobar que no me dejo nada sin hacer: render + rendercols(tex + patch)

	uint8_t* getColumn(int iColIndex);
	uint8_t getTexel(int u, int v, bool& transp);
	int getWidth();
	int getHeight();

protected:
	int m_Width;
	int m_Height;
	int m_overlapSize;

	bool m_isComposed;
	DisplayManager* m_pDisplayManager;

	std::string m_name;
	std::vector<int> m_colPatchCount;
	std::vector<int> m_colIndex;
	std::vector<int> m_colPatch;
	std::vector<WADTexturePatch> m_texturePatches;
	std::unique_ptr<uint8_t[]> m_pOverlapColumnData;
};