/*
 * keyboard.c
 *
 *  Created on: 24 jun. 2019
 *      Author: Javier
 */
#include "keyboard.h"




void IniciaTeclado(){
	flag_key = 1;

	pinMode(TECLADO_ROW_1, INPUT);
	pinMode(TECLADO_ROW_2, INPUT);
	pinMode(TECLADO_ROW_3, INPUT);
	pinMode(TECLADO_ROW_4, INPUT);

	pinMode(TECLADO_COL_1, OUTPUT);
	pinMode(TECLADO_COL_2, OUTPUT);
	pinMode(TECLADO_COL_3, OUTPUT);
	pinMode(TECLADO_COL_4, OUTPUT);

	digitalWrite(TECLADO_COL_1, LOW);
	digitalWrite(TECLADO_COL_2, LOW);
	digitalWrite(TECLADO_COL_3, LOW);
	digitalWrite(TECLADO_COL_4, LOW);


	keyTimer = tmr_new(&Timer_key_isr);


}

// Prototipos de funciones de entrada

int CompruebaColumnaTimeout (fsm_t* this){
	int resultado = 0;

	piLock(1);

	resultado = flag_key&1;

	piUnlock(1);

	return resultado;
}

// Prototipos de funciones de salida

void col_1 (fsm_t* this){
	TipoSistema* system = (TipoSistema*)(this->user_data);

	flag_key = 0;


	digitalWrite(TECLADO_COL_1, HIGH);
	digitalWrite(TECLADO_COL_2, LOW);
	digitalWrite(TECLADO_COL_3, LOW);
	digitalWrite(TECLADO_COL_4, LOW);

	if(digitalRead(TECLADO_ROW_1) == HIGH){
		piLock(0);

		system->teclaPulsada = '1';

		printf("1"); fflush(stdout);
		piUnlock(0);


	}
	if(digitalRead(TECLADO_ROW_2) == HIGH){
		piLock(0);

		system->teclaPulsada = '4';
		flags_juego |= (1 << FLAG_JOYSTICK_LEFT);
		piUnlock(0);
		printf("4"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_3) == HIGH){
		piLock(0);

		system->teclaPulsada = '7';

		piUnlock(0);
	}
	if(digitalRead(TECLADO_ROW_4) == HIGH){
		piLock(0);

		system->teclaPulsada = 'A';

		piUnlock(0);
	}


	tmr_startms(keyTimer, KEY_TIMEOUT);
}
void col_2 (fsm_t* this){
	TipoSistema* system = (TipoSistema*)(this->user_data);

	flag_key = 0;

	digitalWrite(TECLADO_COL_1, LOW);
	digitalWrite(TECLADO_COL_2, HIGH);
	digitalWrite(TECLADO_COL_3, LOW);
	digitalWrite(TECLADO_COL_4, LOW);

	if(digitalRead(TECLADO_ROW_1) == HIGH){
		piLock(0);

		system->teclaPulsada = '2';

		flags_juego |= (1 << FLAG_JOYSTICK_UP);
		piUnlock(0);
		printf("2"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_2) == HIGH){
		piLock(0);

		system->teclaPulsada = '5';
		flags_juego |= (1 << FLAG_TRIGGER_BUTTON);
		printf("5"); fflush(stdout);
		piUnlock(0);
	}
	if(digitalRead(TECLADO_ROW_3) == HIGH){
		piLock(0);

		system->teclaPulsada = '8';

		flags_juego |= (1 << FLAG_JOYSTICK_DOWN);

		piUnlock(0);

		printf("8"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_4) == HIGH){
		piLock(0);

		system->teclaPulsada = '0';
		piUnlock(0);
		printf("0"); fflush(stdout);
	}

	tmr_startms(keyTimer, KEY_TIMEOUT);
}
void col_3 (fsm_t* this){
	TipoSistema* system = (TipoSistema*)(this->user_data);

	flag_key = 0;

	digitalWrite(TECLADO_COL_1, LOW);
	digitalWrite(TECLADO_COL_2, LOW);
	digitalWrite(TECLADO_COL_3, HIGH);
	digitalWrite(TECLADO_COL_4, LOW);

	if(digitalRead(TECLADO_ROW_1) == HIGH){
		piLock(0);

		system->teclaPulsada = '3';

		piUnlock(0);
		printf("3"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_2) == HIGH){
		piLock(0);

		system->teclaPulsada = '6';

		flags_juego |= (1 << FLAG_JOYSTICK_RIGHT);
		piUnlock(0);
		printf("6"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_3) == HIGH){
		piLock(0);

		system->teclaPulsada = '9';

		piUnlock(0);

		printf("9"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_4) == HIGH){
		piLock(0);

		system->teclaPulsada = 'B';

		piUnlock(0);

		printf("B"); fflush(stdout);
	}

	tmr_startms(keyTimer, KEY_TIMEOUT);
}

void col_4 (fsm_t* this){
	TipoSistema* system = (TipoSistema*)(this->user_data);

	flag_key = 0;

	digitalWrite(TECLADO_COL_1, LOW);
	digitalWrite(TECLADO_COL_2, LOW);
	digitalWrite(TECLADO_COL_3, LOW);
	digitalWrite(TECLADO_COL_4, HIGH);

	if(digitalRead(TECLADO_ROW_1) == HIGH){
		piLock(0);

		system->teclaPulsada = 'C';

		piUnlock(0);

		printf("C"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_2) == HIGH){
		piLock(0);

		system->teclaPulsada = 'D';

		piUnlock(0);

		printf("D"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_3) == HIGH){
		piLock(0);

		system->teclaPulsada = 'E';

		piUnlock(0);

		printf("E"); fflush(stdout);
	}
	if(digitalRead(TECLADO_ROW_4) == HIGH){
		piLock(0);

		system->teclaPulsada = 'F';

		piUnlock(0);

		printf("F"); fflush(stdout);
	}

	tmr_startms(keyTimer, KEY_TIMEOUT);
}


void Timer_key_isr (union sigval value) {
	piLock(1);
	flag_key = 1;
	piUnlock(1);
}
