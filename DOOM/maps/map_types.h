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

#include <cstdint>

// En binario es 1000000000000000
// Los nodos hoja del BSP tree usan un flag en su id
#define SUBSECTORIDENTIFIER 0x8000

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

struct Thing
{
	int16_t XPos;
	int16_t YPos;
	uint16_t Angle;
	uint16_t Type;		//TODO aclarar esto
	uint16_t Flags;		//TODO y esto
};

enum THING_ID	//Sacado de https://doom.fandom.com/wiki/Thing_types
{
	///** Están los códigos de las things del shareware y de la versión completa **///
	//Artifacts//
	eBERSERK = 2023,
	eCOMPUTERMAP = 2026,
	eHEALTHPOTION = 2014,
	eINVISIBILITY = 2024,
	eINVULNERABILITY = 2022, 
	eNIGHTVISOR = 2045,
	eSOULSPHERE = 2013,
	eSPIRITUALARMOR = 2015,
	//Powerups//
	eBACKPACK = 8,
	eBLUEARMOR = 2019,
	eGREENARMOR = 2018,
	eMEDKIT = 2012,
	eRADIATIONSUIT = 2025,
	eSTIMPACK = 2011,
	//Weapons//
	eBFG9000 = 2006,
	eCHAINGUN = 2002,
	eCHAINSAW = 2005,
	ePLASMARIFLE = 2004,
	eROCKETLAUNCHER = 2003,
	eSHOTGUN = 2001,
	//Ammo//
	eAMMOCLIP = 2007,
	eBOXOFAMMO = 2048,
	eBOXOFROCKETS = 2046,
	eBOXOFSHELLS = 2049,
	eCELLCHARGE = 2047,
	eCELLCHARGEPACK = 17,
	eROCKET = 2010,
	eSHOTGUNSHELLS = 2008,
	//Keys//
	eBLUEKEYCARD = 5,
	eBLUESKULLKEY = 40,
	eREDKEYCARD = 13,
	eREDSKULLKEY = 38,
	eYELLOWKEYCARD = 6,
	eYELLOWSKULLKEY = 39,
	//Monsters//
	eBARONOFHELL = 3003,
	eCACODEMON = 3005,
	eCYBERDEMON = 16,
	eDEMON = 3002,
	eFORMERHUMANTROOPER = 3004,
	eFORMERHUMANSERGEANT = 9,
	eIMP = 3001,
	eLOSTSOUL = 3006,
	eSPECTRE = 58,
	eSPIDERMASTERMIND = 7,
	//Obstacles//
	eBARREL = 2035,
	eBURNTTREE = 43,
	eCANDELABRA = 35,
	eEVILEYE = 41,
	eFIVESKULLS = 28,
	eFLOATINGSKULL = 42,
	eFLOORLAMP = 2028,
	eHANGINGLEG = 53,
	eHANGINGPAIROFLEGS = 52,
	eHANGINFVICTIMARMSOUT = 50,
	eHANGINGVICTIMNOLEG = 51,
	eHANGINGVICTIMTWITCHING = 49,
	eIMPALEDHUMAN = 25,
	eLARGEBROWNTREE = 54,
	ePILEOFSKULLSANDCANDLES = 29,
	eSHORTBLUEFIRESTICK = 55,
	eSHORTGREENFIRESTICK = 56,
	eSHORTGREENPILLAR = 31,
	eSHORTGREENPILLARBEATINGHEART = 36,
	eSHORTREDFIRESTICK = 57,
	eSHORTREDPILLAR = 33,
	eSHORTREDPILLARSKULL = 37,
	eSKULLONPOLE = 27,
	eSTALAGMITE = 47,
	eTALLBLUEFIRESTICK = 44,
	eTALLGREENFIRESTICK = 45,
	eTALLGREENPILLAR = 30,
	eTALLREDFIRESTICK = 46,
	eTALLREDPILLAR = 32,
	eTALLTECHNOPILLAR = 48,
	eTWITCHINGIMPALEDHUMAN = 26,
	//Decorations//
	eBLOODYMESS = 10,
	eBLOODYMESS_2 = 12,
	eCANDLE = 34,
	eDEADCACODEMON = 22,
	eDEADDEMON = 21,
	eDEADFORMERHUMAN = 18,
	eDEADFORMERSERGEANT = 19,
	eDEADIMP = 20,
	eDEADLOSTSOULINVISIBLE = 23,
	eDEADPLAYER = 15,
	eHANGINGLEG2 = 62,
	eHANGINGPAIROFLEGS2 = 60,
	eHANGINGVICTIMARMSOUT = 59,
	eHANGINGVICTIMONELEGGED = 61,
	eHANGINGVICTIMTWITCHING2 = 63,
	ePOOLOFBLOODANDFLESH = 24,
	//Other//
	eDEATHMATCHSTART = 11,
	ePLAYER1START = 1,
	ePLAYER2START = 2,
	ePLAYER3START = 3,
	ePLAYER4START = 4,
	eTELEPORT_LANDING = 14
};

struct BSP_Node
{
	/* Splitter line */
	int16_t XPartition;
	int16_t YPartition;
	int16_t XPartDir;
	int16_t YPartDir;
	/* Right box */
	int16_t RightBoxTop;		//y coord
	int16_t RightBoxBottom;		//y coord
	int16_t RightBoxLeft;		//x coord
	int16_t RightBoxRight;		//x coord
	/* Left box */
	int16_t LeftBoxTop;			//y coord
	int16_t LeftBoxBottom;		//y coord
	int16_t LeftBoxLeft;		//x coord
	int16_t LeftBoxRight;		//x coord
	/* References to child nodes */
	int16_t RightChild;
	int16_t LeftChild;
};

struct Subsector
{
	uint16_t seg_count;			//Número de segmentos del subsector
	uint16_t first_segID;		//Índice del primer seg (sigo sin saber como funcionan los segs)
};

struct Seg
{
	uint16_t vert1;				//Vértices que componen el seg
	uint16_t vert2;
	uint16_t angle;				//Ángulo del seg (?)
	uint16_t linedef_index;
	uint16_t dir;				// 0 = Misma que el linedef asociado, 1 = opuesta
	uint16_t offset;			//Distancia del linedef en la que empieza el seg
};

#endif
