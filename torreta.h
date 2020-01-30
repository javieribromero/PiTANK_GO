/*
 * torreta.h
 *
 *  Created on: 21 de enero de 2019
 *      Author: FFM
 */

#ifndef _TORRETA_H_
#define _TORRETA_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "piTankGoLib.h"

enum fsm_state{
	WAIT_START,
	JOYSTICK_UP,
	WAIT_PUSH,
	WAIT_NEXT,
	JOYSTICK_DOWN,
	JOYSTICK_RIGTH,
	JOYSTICK_LEFT,
	WAIT_MOVE,
	WAIT_END,
	TRIGGER_BUTTON
};

typedef struct {
	int inicio; // Valor correspondiente a la posicion inicial del servo
	int incremento; // Cuantía en que se incrementa el valor de la posicion con cada movimiento del servo
	int minimo; // Valor mínimo correspondiente a la posicion del servo
	int maximo; // Valor maximo correspondiente a la posicion del servo
} TipoServo;

typedef struct {
	int x; // Coordenada x correspondiente a la posicion del servo horizontal
	int y; // Coordenada y correspondiente a la posicion del servo vertical
} TipoPosicionTorreta;

typedef struct {
	TipoPosicionTorreta posicion;
	TipoServo servo_x;
	TipoServo servo_y;

	tmr_t* timer;
} TipoTorreta;

extern int flags_juego;

// Prototipos de procedimientos de inicializacion de los objetos especificos
void InicializaTorreta (TipoTorreta *p_torreta);

// Prototipos de funciones de entrada
int CompruebaComienzo (fsm_t* this);
int CompruebaJoystickUp (fsm_t* this);
int CompruebaJoystickDown (fsm_t* fsm_player);
int CompruebaJoystickLeft (fsm_t* this);
int CompruebaJoystickRight (fsm_t* this);
int CompruebaTimeoutDisparo (fsm_t* this);
int CompruebaImpacto (fsm_t* this);
int CompruebaTriggerButton (fsm_t* this);
int CompruebaFinalJuego (fsm_t* this);
static int ok(fsm_t* this){return 1;};

// Prototipos de funciones de salida
void ComienzaSistema (fsm_t* this);
void MueveTorretaArriba (fsm_t* this);
void MueveTorretaAbajo (fsm_t* this);
void MueveTorretaIzquierda (fsm_t* this);
void MueveTorretaDerecha (fsm_t* this);
void DisparoIR (fsm_t* this);
void FinalDisparoIR (fsm_t* this);
void ImpactoDetectado (fsm_t* this);
void FinalizaJuego (fsm_t* this);


// Prototipos de procedimientos de atencion a las interrupciones
void timer_duracion_disparo_isr (union sigval value);


static fsm_trans_t torreta_tt[] = {
								{WAIT_START, CompruebaComienzo,	WAIT_MOVE, ComienzaSistema},
								{WAIT_MOVE, CompruebaJoystickRight, JOYSTICK_RIGTH, MueveTorretaDerecha},
								{WAIT_MOVE, CompruebaJoystickLeft, JOYSTICK_LEFT, MueveTorretaIzquierda},
								{WAIT_MOVE, CompruebaJoystickUp, JOYSTICK_UP, MueveTorretaArriba},
								{WAIT_MOVE, CompruebaJoystickDown, JOYSTICK_DOWN, MueveTorretaAbajo},
								{WAIT_MOVE, CompruebaTriggerButton, TRIGGER_BUTTON, DisparoIR},
								{WAIT_MOVE, CompruebaFinalJuego, WAIT_END, FinalizaJuego},
								{TRIGGER_BUTTON, CompruebaImpacto, WAIT_MOVE, ImpactoDetectado},
								{TRIGGER_BUTTON, CompruebaTimeoutDisparo, WAIT_MOVE, FinalDisparoIR},
								{JOYSTICK_UP, ok, WAIT_MOVE, NULL },
								{JOYSTICK_DOWN, ok, WAIT_MOVE, NULL },
								{JOYSTICK_RIGTH, ok, WAIT_MOVE, NULL },
								{JOYSTICK_LEFT, ok, WAIT_MOVE, NULL },
								{-1, NULL, -1, NULL }
							};

#endif /* _TORRETA_H_ */
