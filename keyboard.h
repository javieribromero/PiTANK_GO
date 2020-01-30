/*
 * keyboard.h
 *
 *  Created on: 24 jun. 2019
 *      Author: Javier
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <wiringPi.h>
#include "piTankGoLib.h"
#include "piTankGo_1.h"
#define KEY_TIMEOUT DEBOUNCE_TIME/4

enum fsm_state_key{
	KEY_COL_1,
	KEY_COL_2,
	KEY_COL_3,
	KEY_COL_4
};


int flag_key;



tmr_t*  keyTimer;

// inicialización

void IniciaTeclado();

// Prototipos de funciones de entrada

int CompruebaColumnaTimeout (fsm_t* this);

// Prototipos de funciones de salida

void col_1 (fsm_t* this);
void col_2 (fsm_t* this);
void col_3 (fsm_t* this);
void col_4 (fsm_t* this);
void Timer_key_isr (union sigval value);


static fsm_trans_t  keyboard_tt[] = {
								{KEY_COL_1,	CompruebaColumnaTimeout,	KEY_COL_2,	col_1 },
								{KEY_COL_2,	CompruebaColumnaTimeout,	KEY_COL_3,	col_2 },
								{KEY_COL_3,	CompruebaColumnaTimeout,	KEY_COL_4,	col_3 },
								{KEY_COL_4,	CompruebaColumnaTimeout,	KEY_COL_1,	col_4 },
								{-1, NULL, -1, NULL }
							};

#endif /* KEYBOARD_H_ */
