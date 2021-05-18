/*
* Name: Geometry.h
* Date: 18/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Operaciones varias que pueden resultar útiles en varios puntos del código, como la intersección de dos rectas
*/

#pragma once


//Intersecta la recta dada por los puntos [x1,y1] y [x2,y2] con la dada por [x3,y3] y [x4,y4].
//Devuelve true sii ha encontrado una intersección, y false en caso de que no, como por ejemplo, que sean rectas paralelas
//Si ha encontrado una intersección, pondrá los valores x e y de ese punto en Px y Py, respectivamente
bool intersect(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3, const float& x4, const float& y4, float& Px, float& Py)
{
	float denominador = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (denominador == 0)
	{
		return false;
	}
	else
	{
		float x1y2_y1x2 = (x1 * y2 - y1 * x2);
		float x3y4_y3x4 = (x3 * y4 - y3 * x4);
		Px = (x1y2_y1x2 * (x3 - x4) - (x1 - x2) * x3y4_y3x4) / denominador;
		Py = (x1y2_y1x2 * (y3 - y4) - (y1 - y2) * x3y4_y3x4) / denominador;
		return true;
	}
}

//Devuelve la distancia entre dos puntos dados por [x1,y1] y [x2,y2], respectivamente
float dist2Points(const float& x1, const float& y1, const float& x2, const float& y2)
{
	return sqrt(powf(x2 - x1 ,2.0f) + pow(y2 - y1, 2.0f));
}
