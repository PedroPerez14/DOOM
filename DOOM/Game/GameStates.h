/*
* Name: GameStates.cpp
* Date: 12/04/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Posibles estados en los que puede encontrarse el juego
*/
#pragma once

//Antes en Game.h pero daba problemas
enum class Status { eMAINMENU = 0, ePLAYING, eCREDITS, eOPTIONS, ePAUSE, eDEAD };	//Completar con tantos estados como se necesite

inline std::ostream& operator << (std::ostream& os, Status s) {
	switch (s)
	{
	case Status::eMAINMENU:
		os << "eMAINMENU";
		return os;
		break;
	case Status::ePLAYING:
		os << "ePLAYING";
		return os;
		break;
	case Status::eCREDITS:
		os << "eCREDITS";
		return os;
		break;
	case Status::eOPTIONS:
		os << "eOPTIONS";
		return os;
		break;
	case Status::ePAUSE:
		os << "ePAUSE";
		return os;
		break;
	case Status::eDEAD:
		os << "eDEAD";
		return os;
		break;
	default:
		return os;
		break;
	}
	return os;
}

