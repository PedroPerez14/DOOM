/*
* Name: Patch.h
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para gestionar los patches, la forma que tiene doom de crear las "texturas del juego", y de gestionar las transparencias.
*/

#pragma once
#include "SFML/Graphics.hpp"
#include "../WAD/DataTypes.h"
#include "../DisplayManager/DisplayManager.h"

class Patch
{
public:
	Patch(std::string name);
	~Patch();

	void Init(WADPatchHeader& patchHeader, DisplayManager* pDisplayManager);
	void Initialize(WADPatchHeader& patchHeader);
	void AppendPatchColumn(WADPatchColumn& patchColumn);
	void AppendColumnStartIndex();
	void Render(uint8_t* pixels, sf::RenderWindow* rw, int iXScreenLocation, int iYScreenLocation);
	void composeColumn(uint8_t* pOverLapColumnData, int iHeight, int& iPatchColumnIndex, int iColumnOffsetIndex, int iYOrigin);
	void RenderColumn(uint8_t* buffer, int iColumn, int iXScreenLocation, int iYScreenLocation, int iXMaxHeight, int iYOffset);	//TODO faltará algún parámetro seguro

	int getHeight();
	int getWidth();
	int getXOffset();
	int getYOffset();
	int getColumnDataIndex(int iIndex);
	std::string getPName();

protected:

	std::string m_name;						//El nombre de ese patch en el WAD
	DisplayManager* m_pDisplayManager;

	uint16_t m_Width;
	uint16_t m_Height;
	int16_t m_XOffset;
	int16_t m_YOffset;

	sf::RenderWindow* m_pRenderWindow;
	std::vector<WADPatchColumn> m_PatchData;
	std::vector<int> m_columnIndex;			//TODO cuando lo toco?????
};
