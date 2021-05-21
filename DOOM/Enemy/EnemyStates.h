
/*
* Name: EnemyStates.h
* Date: 07/05/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
* Coms: Posibles estados en los que puede encontrarse los enemigos
*/
#pragma once

//Antes en Game.h pero daba problemas
enum class EnemyState { await = 0, moveLeft, moveRight, moveTop, moveDown, moveAwait, moveRightPlayer, moveLeftPlayer, moveFollowPlayer, shoot, dead};	//Completar con tantos estados como se necesite

