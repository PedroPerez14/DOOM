/*
* Name: DataTypes.h
* Date: 03/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Definiciones auxiliares varias de tipos de dato para la carga de WADs.
*	Basado en https://github.com/amroibrahim/DIYDoom/tree/master/DIYDOOM/Notes001/notes
*/

#pragma once

#ifndef DATATYPES_H
#define DATATYPES_H

#include "SFML/Graphics.hpp"

struct WAD_Header
{
	char WAD_type[5];			//En el .WAD sob 4 caracteres, pero necesitamos el '\0'
	uint32_t dir_count;			//Numero de directorios del .WAD
	uint32_t listing_offset;	//Offset en Bytes hasta las listas de directorios 
};

struct WAD_Directory
{
	uint32_t lump_offset;		//Offset en Bytes hasta el principio del lump con los datos en el WAD
	uint32_t lump_size;			//Tamaño del lump en Bytes
	char lump_name[9];			//Nombre del lump del WAD. Añadido un byte para el '\0'
};

struct WADPatchHeader
{
    uint16_t Width;
    uint16_t Height;
    int16_t LeftOffset;
    int16_t TopOffset;
    uint32_t* ColumnOffset;
};

struct WADPatchColumn
{
    uint8_t TopDelta;
    uint8_t Length;
    uint8_t	PaddingPre;
    uint8_t* pColumnData;
    uint8_t PaddingPost;
};

enum LUMPINDEX {
    eTHINGS = 1,
    eLINEDEFS,
    eSIDEDDEFS,
    eVERTEXES,
    eSEGS,
    eSSECTORS,
    eNODES,
    eSECTORS,
    eREJECT,
    eBLOCKMAP,
    eCOUNT
};

#define NUM_PALETAS 14          //En el wad original hay 14 paletas de colores
struct WADPalette
{
    sf::Color Colors[256];      //Una sola paleta son 256 bytes
};

struct WADPNames
{
    uint32_t PNameCount;
    uint32_t PNameOffset;
};

struct WADTextureHeader
{
    uint32_t TexturesCount;
    uint32_t TexturesOffset;
    uint32_t* pTexturesDataOffset;
};

struct WADTexturePatch
{
    int16_t XOffset;
    int16_t YOffset;
    uint16_t PNameIndex;
    uint16_t StepDir;               //No se usa
    uint16_t ColorMap;              //ni este
};

struct WADTextureData
{
    char TextureName[9];
    uint32_t Flags;                 //ni este
    uint16_t Width;
    uint16_t Height;
    uint32_t ColumnDirectory;       //ni este
    uint16_t PatchCount;
    WADTexturePatch* pTexturePatch;
};


#endif