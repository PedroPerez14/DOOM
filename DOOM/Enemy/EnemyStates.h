
/*
* Name: EnemyStates.h
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Posibles estados en los que puede encontrarse los enemigos
*/
#pragma once

//Antes en Game.h pero daba problemas
enum class EnemyState { await = 0, moveLeft, moveRight, moveTop, moveDown, shoot };	//Completar con tantos estados como se necesite

