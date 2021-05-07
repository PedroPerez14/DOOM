/*
* Name: wad_reader.h
* Date: 03/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Cargador de archivos .WAD para nuestro DOOM.
*	Basado en https://github.com/amroibrahim/DIYDoom/tree/master/DIYDOOM/Notes001/notes
*/

#pragma once

#include <iostream>
#include <cstdint>
#include "DataTypes.h"
#include "../maps/map_types.h"

#ifndef WADREADER_H
#define WADREADER_H

//TODO a lo mejor simplemente se podía poner duera de una clase pero bueno, de momento sirve
class WADReader
{
public:

	void ReadHeader(const uint8_t *WAD_data, int offset, WAD_Header &header);

	void ReadDirectory(const uint8_t* WAD_data, int offset, WAD_Directory &dir);

	uint16_t Read2Bytes(const uint8_t *WAD_data, int offset);

	uint32_t Read4Bytes(const uint8_t * WAD_data, int offset);

	void ReadVertexData(const uint8_t* WAD_data, int offset, Vertex& vertex);

	void ReadLinedefData(const uint8_t* WAD_data, int offset, WADLinedef& line);

	void ReadThingData(const uint8_t* WAD_data, int offset, Thing& thing);

	void ReadNodesData(const uint8_t* WAD_data, int offset, BSP_Node& node);

	void ReadSegsData(const uint8_t* WAD_data, int offset, WADSeg& seg);

	void ReadSSecsData(const uint8_t* WAD_data, int offset, Subsector& ssec);

	void ReadSecsData(const uint8_t* WAD_data, int offset, WADSector& sec);

	void ReadSidedefsData(const uint8_t* WAD_data, int offset, WADSidedef& sidedef);

	void ReadPalette(const uint8_t* WAD_data, int offset, WADPalette& palette);

	void ReadPatchHeader(const uint8_t* WAD_data, int offset, WADPatchHeader& header);

	int ReadPatchColumn(const uint8_t* WAD_data, int offset, WADPatchColumn& col);
};

#endif

