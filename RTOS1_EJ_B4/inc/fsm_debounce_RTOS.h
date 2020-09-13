/*
 * fsm_debounce_RTS.h
 *
 *  Created on: 12 sep. 2020
 *      Author: francisco
 */

#ifndef MISPROGRAMAS_RTOS1_EJ_B4_INC_FSM_DEBOUNCE_RTOS_H_
#define MISPROGRAMAS_RTOS1_EJ_B4_INC_FSM_DEBOUNCE_RTOS_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"

#define DEBOUNCE_TIME 40

/*****************************************
 * 			Data Types
 *****************************************/

typedef enum{
	FSM_BUTTON_UP,
	FSM_BUTTON_DOWN,
	FSM_BUTTON_FALLING,
	FSM_BUTTON_RISING
} fsmButtonState_t;

typedef struct{
	gpioMap_t tecla_fsm;

	// Variables
	TickType_t tiempo_medido;
	fsmButtonState_t fsmButtonState;
	TickType_t tiempo_down;
	TickType_t tiempo_up;

	uint8_t contFalling;
	uint8_t contRising;

} TeclaRTOSfsm_t;

/*****************************************
 * 			Prototipos
 ****************************************/
void fsmButtonUpdate( TeclaRTOSfsm_t* button );
//void fsmButtonError( void );
void fsmButtonInit( TeclaRTOSfsm_t* button );





#endif /* MISPROGRAMAS_RTOS1_EJ_B4_INC_FSM_DEBOUNCE_RTOS_H_ */
