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
