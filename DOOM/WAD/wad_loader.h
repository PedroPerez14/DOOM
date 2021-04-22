/*
* Name: wad_loader.h
* Date: 03/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Cargador de archivos .WAD para nuestro DOOM. 
*	Basado en https://github.com/amroibrahim/DIYDoom/tree/master/DIYDOOM/Notes001/notes
*/

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include "../DisplayManager/DisplayManager.h"
#include "DataTypes.h"
#include "../maps/map.h"
#include "wad_reader.h"

#ifndef WADLOADER_H
#define WADLOADER_H

class WADLoader
{
public:
	WADLoader();
	WADLoader(std::string filePath, DisplayManager* dm); //Path del .WAD
	~WADLoader();	//Destructor

	bool LoadWAD();					//Carga los datos del .WAD, a los que apuntará WAD_data
	bool LoadMapData(Map* map);		//Lee y carga todos los datos de un mapa (vert y linedef de momento)

protected:
	bool OpenAndLoad();				//Abre y carga el .WAD
	void ReadDirectories();			//Recorre los directorios del .WAD
	int FindMapIndex(Map* map);		//Encuentra el índice del mapa entre los lumps del .WAD
	bool ReadMapVertex(Map* map);	//Lee los vértices de un mapa
	bool ReadMapLinedef(Map* map);	//Lee los linedefs de un mapa
	bool ReadMapThing(Map* map);	//Lee los things de un mapa
	bool ReadMapNodes(Map* map);	//Lee los BSP_Nodes de un mapa
	bool ReadMapSegs(Map* map);		//Lee los segs de un mapa
	bool ReadMapSubs(Map* map);		//Lee los subsectores de un mapa
	bool ReadMapSectors(Map* map);	//Lee los sectores de un mapa
	bool ReadMapSidedefs(Map* map);	//Lee los sidedefs de un mapa
	bool LoadPalette(DisplayManager* pDisplayManager);	//Carga de paletas de colores
	int FindLumpByName(std::string lump_name);

	std::string WAD_path;			//Path que recibe el constructor
	std::ifstream WAD_stream;		//flujo de entrada de datos que recibe del .WAD
	uint8_t* WAD_data;				//Puntero a los datos del archivo .WAD
	std::vector<WAD_Directory> WAD_dirs;	//Directorios del .WAD
	WADReader reader;				//Reader de ficheros .WAD para acceder a ellos
	DisplayManager* m_pDisplayManager;
};

#endif