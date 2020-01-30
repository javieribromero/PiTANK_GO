
#include "player.h"

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//------------------------------------------------------
// void InicializaEfecto (TipoEfecto *p_efecto)
//
// Descripcion: inicializa los parametros del objeto efecto.
// Ejemplo de uso:
//
// ...
//
// TipoEfecto efecto_demo;
//
// if ( InicializaEfecto (&efecto_demo, "DISPARO", frecuenciasDisparo, tiemposDisparo, 16) < 0 ) {
// 		printf("\n[ERROR!!!][InicializaEfecto]\n");
// 		fflush(stdout);
// 		}
//
// ...
//
//------------------------------------------------------
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {

	memcpy(p_efecto->duraciones, array_duraciones, num_notas*sizeof(int));
	memcpy(p_efecto->frecuencias, array_frecuencias, num_notas*sizeof(int));
	strcpy(p_efecto->nombre, nombre);

	p_efecto->num_notas = num_notas;
	return p_efecto->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicialización común a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer (TipoPlayer *p_player) {
	p_player->timer = tmr_new(timer_player_duracion_nota_actual_isr);

	flags_player = 0;
	p_player->p_efecto =  NULL;
	p_player->posicion_nota_actual = 0;
	p_player->frecuencia_nota_actual = 0;
	p_player->duracion_nota_actual = 0;
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;

	piLock(0);

	result = flags_player & (1 << FLAG_START_DISPARO);

	piUnlock(0);

	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;

	piLock(0);

	result = flags_player & (1 << FLAG_START_IMPACTO);

	piUnlock(0);

	return result;
}

int CompruebaNuevaNota (fsm_t* this){
	int result = 0;

	piLock(0);

	result = !(flags_player & (1 << FLAG_PLAYER_END));

	printf("res = %d \n", result);
	piUnlock(0);//

	return result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result = 0;

	piLock(0);

	result = flags_player & (1 << FLAG_NOTA_TIMEOUT);

	piUnlock(0);

	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;

	piLock(0);

	result = flags_player & (1 << FLAG_PLAYER_END);

	piUnlock(0);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaPlayDisparo (fsm_t* this) {

	TipoPlayer* player = (TipoPlayer*)this->user_data;

	//CLEAN FLAGS
	piLock(0);

	flags_player &= ~(1 << FLAG_START_DISPARO);

	piUnlock(0);

	player->p_efecto =  &(player->efecto_disparo);
	player->posicion_nota_actual = 0;
	player->frecuencia_nota_actual = 0;
	player->duracion_nota_actual = 0;

	piLock(0);
	flags_player |= (1 << FLAG_NOTA_TIMEOUT);
	piUnlock(0);
}

void InicializaPlayImpacto (fsm_t* this) {
	TipoPlayer* player = (TipoPlayer*)this->user_data;

	//CLEAN FLAGS
	piLock(0);

	flags_player &= ~(1 << FLAG_START_IMPACTO);

	piUnlock(0);

	player->p_efecto =  &(player->efecto_impacto);
	player->posicion_nota_actual = 0;
	player->frecuencia_nota_actual = 0;
	player->duracion_nota_actual = 0;

	piLock(0);
	flags_player |= (1 << FLAG_NOTA_TIMEOUT);
	piUnlock(0);
}

void ActualizaPlayer (fsm_t* this) {

	TipoPlayer* player = (TipoPlayer*)this->user_data;

	//CLEAN FLAGS
	piLock(0);

	flags_player &= ~(1 << FLAG_NOTA_TIMEOUT);

	piUnlock(0);

	if(player->posicion_nota_actual == player->p_efecto->num_notas){
		flags_player |= (1 << FLAG_PLAYER_END);
		return;
	}

	player->frecuencia_nota_actual = player->p_efecto->frecuencias[player->posicion_nota_actual];
	player->duracion_nota_actual = player->p_efecto->duraciones[player->posicion_nota_actual];

	player->posicion_nota_actual++;




}

void ComienzaNuevaNota (fsm_t* this) {
	TipoPlayer* player = (TipoPlayer*)this->user_data;

	softToneWrite(PLAYER_PWM_PIN, player->frecuencia_nota_actual);
	tmr_startms(player->timer, player->duracion_nota_actual);

}

void FinalEfecto (fsm_t* this) {
	piLock(0);
	flags_player &= ~(1 << FLAG_NOTA_TIMEOUT);
	flags_player &= ~(1 << FLAG_PLAYER_END);
	flags_player &= ~(1 << FLAG_START_IMPACTO);
	flags_player &= ~(1 << FLAG_START_DISPARO);
	piUnlock(0);

	softToneWrite(PLAYER_PWM_PIN,0);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_player_duracion_nota_actual_isr (union sigval value) {
	piLock(0);
	flags_player |= (1 << FLAG_NOTA_TIMEOUT);
	piUnlock(0);
}
