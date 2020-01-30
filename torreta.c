
#include "torreta.h"
#include "softPwm.h"


//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTorreta (TipoTorreta *p_torreta) {
	p_torreta->servo_x.incremento = 1;
	p_torreta->servo_x.inicio = 15;
	p_torreta->servo_x.maximo = 20;
	p_torreta->servo_x.minimo = 10;

	p_torreta->servo_y.incremento = 1;
	p_torreta->servo_y.inicio = 15;
	p_torreta->servo_y.maximo = 20;
	p_torreta->servo_y.minimo = 10;

	p_torreta->posicion.x = 15;
	p_torreta->posicion.y = 15;

	p_torreta->timer = tmr_new(timer_duracion_disparo_isr);


}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;
	piLock(0);
	result = flags_system & (1 << FLAG_SYSTEM_START );
	piUnlock(0);
	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;
	piLock(0);
	result =flags_juego & (1 << FLAG_JOYSTICK_UP );
	piUnlock(0);
	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;
	piLock(0);
	result =flags_juego & (1 << FLAG_JOYSTICK_DOWN );
	piUnlock(0);
	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;
	piLock(0);
	result =flags_juego & (1 << FLAG_JOYSTICK_LEFT );
	piUnlock(0);
	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;
	piLock(0);
	result =flags_juego & (1 << FLAG_JOYSTICK_RIGHT );
	piUnlock(0);
	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;
	piLock(0);
	result =flags_juego & (1 << FLAG_SHOOT_TIMEOUT );
	piUnlock(0);
	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;
	piLock(0);
	result = flags_juego & (1 << FLAG_TARGET_DONE );
	piUnlock(0);

	printf("impacto = %d !", result);
	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;
	piLock(0);
	result =flags_juego & (1 << FLAG_TRIGGER_BUTTON );
	piUnlock(0);
	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;
	piLock(0);
	result =flags_juego & (1 << FLAG_SYSTEM_END );
	piUnlock(0);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ComienzaSistema (fsm_t* this) {
	flags_juego = 0;

//	pinMode(SERVO_VERTICAL_PIN, PWM_OUTPUT);
//	pinMode(SERVO_HORIZONTAL_PIN, PWM_OUTPUT);
//
//	pwmSetMode(PWM_MODE_MS);
//	pwmSetRange(2000);
//	pwmSetClock(192);
//
//	pwmWrite(SERVO_VERTICAL_PIN, 150);
//	pwmWrite(SERVO_HORIZONTAL_PIN, 150);


	softPwmCreate(SERVO_VERTICAL_PIN, 0 , SERVO_MAX);
	softPwmCreate(SERVO_HORIZONTAL_PIN, 0 , SERVO_MAX);

	softPwmWrite(SERVO_VERTICAL_PIN, 15);
	softPwmWrite(SERVO_HORIZONTAL_PIN, 15);

	printf("System started! \n");
}

void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta* torreta = (TipoTorreta*)( this->user_data);

	piLock(0);
	flags_juego &= ~(1 << FLAG_JOYSTICK_UP);
	piUnlock(0);

	if(torreta->posicion.y < torreta->servo_y.maximo)
		torreta->posicion.y += torreta->servo_y.incremento;

	softPwmWrite(SERVO_VERTICAL_PIN, torreta->posicion.y);

	printf("MUEVE ARRIBA %d flags = %d \n", torreta->posicion.y, flags_juego);
}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta* torreta = (TipoTorreta*)( this->user_data);

	piLock(0);
	flags_juego &= ~(1 << FLAG_JOYSTICK_DOWN);
	piUnlock(0);

	if(torreta->posicion.y > torreta->servo_y.minimo)
		torreta->posicion.y -= torreta->servo_y.incremento;

	softPwmWrite(SERVO_VERTICAL_PIN, torreta->posicion.y);
}

void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta* torreta = (TipoTorreta*)( this->user_data);

	piLock(0);
	flags_juego &= ~(1 << FLAG_JOYSTICK_LEFT);
	piUnlock(0);

	if(torreta->posicion.x < torreta->servo_x.maximo)
		torreta->posicion.x += torreta->servo_x.incremento;

	softPwmWrite(SERVO_HORIZONTAL_PIN, torreta->posicion.x);
}

void MueveTorretaDerecha (fsm_t* this) {
	TipoTorreta* torreta = (TipoTorreta*)( this->user_data);

	piLock(0);
	flags_juego &= ~(1 << FLAG_JOYSTICK_RIGHT);
	piUnlock(0);

	if(torreta->posicion.x > torreta->servo_x.minimo)
		torreta->posicion.x -= torreta->servo_x.incremento;

	softPwmWrite(SERVO_HORIZONTAL_PIN, torreta->posicion.x);
	printf("MUEVE ARRIBA %d flags = %d \n", torreta->posicion.x, flags_juego);
}

void DisparoIR (fsm_t* this) {
	TipoTorreta* torreta = (TipoTorreta*)( this->user_data);
	digitalWrite(IR_TX_PIN, HIGH);

	piLock(PLAYER_FLAGS_KEY);
	flags_juego &= ~(1 << FLAG_TRIGGER_BUTTON);
	flags_player |= (1 << FLAG_START_DISPARO);
	piUnlock(PLAYER_FLAGS_KEY);

	printf("DISPARO \n");

	tmr_startms(torreta->timer, TORRETA_TIMEOUT);
}

void FinalDisparoIR (fsm_t* this) {
	digitalWrite(IR_TX_PIN, LOW);

	piLock(0);
	flags_juego &= ~(1 << FLAG_SHOOT_TIMEOUT);
	piUnlock(0);

}

void ImpactoDetectado (fsm_t* this) {
	digitalWrite(IR_TX_PIN, LOW);

	printf("impacto!\n");
	piLock(PLAYER_FLAGS_KEY);
	flags_player |= (1 << FLAG_START_IMPACTO);
	piUnlock(PLAYER_FLAGS_KEY);

	piLock(0);
	flags_juego &= ~(1 << FLAG_TARGET_DONE);
	piUnlock(0);

}

void FinalizaJuego (fsm_t* this) {
	printf("final del juego \n");
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_duracion_disparo_isr (union sigval value) {
	piLock(0);
	flags_juego |= (1 << FLAG_SHOOT_TIMEOUT);
	piUnlock(0);
	printf("timeout!");
}
