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


#endif