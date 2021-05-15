/*
* Name: RenderTypes.h
* Date: 04/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Tipos necesarios para procesar el renderizado de mapas en 3D.
*/

#pragma once

//Se usa para clipear las paredes que están dentro del FOV y elegir para mostrar
//	aquellas que no tengan ninguna otra delante (total o parcialmente)
//	El algoritmo que hace esto está en Renderer.cpp: ClipSolidWallsHorizontal
struct Cliprange
{
	int first;
	int last;
};


/*
* Básicamente se pasa un struct gordo entre las funciones de Renderer.cpp en vez de tener
*	que pasar 2000 parámetros cada vez
*/
struct SegRenderData
{
	int VertX1OnScreen;
	int VertX2OnScreen;

	Angle AngleV1;
	Angle AngleV2;
	
	float DistToV1;
	float DistToV2;
	float DistToNormal;
	float V1ScaleFactor;
	float V2ScaleFactor;
	float Steps;

	float RSecCeiling;
	float RSecFloor;
	float CeilingStep;
	float CeilingEnd;
	float FloorStep;
	float FloorStart;

	float LSecCeiling;
	float LSecFloor;

	bool DrawUpper;
	bool DrawLower;

	float UpperHeightStep;
	float iUpperHeight;
	float LowerHeightStep;
	float iLowerHeight;

	bool UpdateFloor;
	bool UpdateCeiling;

	Seg* pSeg;
};

//Para renderizar suelos y techos, de momento no lo uso porque no me hacen falta aunque para optimizar serán necesarios creo
struct Visplane
{
	// Información del material //
	int height;
	int picnum;					//??
	int lightLevel;				//Probablemente lo lo usaremos pero lo pongo

	// Información del espacio de pantalla a cubrir //
	int minX;
	int maxX;

	std::vector<int> top;		//Inicializar a screenwidth en cada frame (view->getsize.x)
	std::vector<int> bottom;	//Lo mismo que el de arriba
};
