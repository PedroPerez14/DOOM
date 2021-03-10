/*
* Name: map_tyeps.h
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Tipos de datos para los mapas de DOOM, como los vértices.
*/

#pragma once

#ifndef MAP_TYPES_H
#define MAP_TYPES_H

struct Vertex
{
	int16_t x;		//signed short
	int16_t y;
};

struct Linedef
{
	uint16_t vert1;
	uint16_t vert2;
	uint16_t flags;
	uint16_t line_type;	//TODO nombre descriptivo?
	uint16_t sector_tag;
	uint16_t sidedef_r;
	uint16_t sidedef_l;
};

/* Linedef flag values */

enum LINEDEF_FLAGS
{
	eBLOCKING = 0,
	eBLOCKMONSTERS = 1,
	eTWOSIDED = 2,
	eDONTPEGTOP = 4,
	eDONTPEGBOTTOM = 8,
	eSECRET = 16,
	eSOUNDBLOCK = 32,
	eDONTDRAW = 64,
	eDRAW = 128
};

struct Automap_info
{
	int16_t maxX;
	int16_t minX;
	int16_t maxY;
	int16_t minY;
};


#endif
