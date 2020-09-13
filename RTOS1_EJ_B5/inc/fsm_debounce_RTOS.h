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
#include "macros.h"
#include "task.h"

/*****************************************
 * 			Prototipos
 ****************************************/
void fsmButtonUpdate( TeclaRTOSfsm_t* button );
//void fsmButtonError( void );
void fsmButtonInit( TeclaRTOSfsm_t* button );


/* prototipo de la tarea led   */
void tarea_led( void* taskParmPtr );
void crear_tarea(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);

#endif /* MISPROGRAMAS_RTOS1_EJ_B4_INC_FSM_DEBOUNCE_RTOS_H_ */
