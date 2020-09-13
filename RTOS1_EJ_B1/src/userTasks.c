/*=============================================================================
 * Copyright (c) 2020, Francisco Timez <franciscotimez@gmail.com>
 * All rights reserved.
 * License: mit (see LICENSE.txt)
 * Date: 2020/09/10
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/

#include "userTasks.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

// Task implementation
void TaskHeartBeat( void* taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------
	gpioWrite( LED1, ON );
	vTaskDelay( 1000 / portTICK_RATE_MS );	 			// Envia la tarea al estado bloqueado durante 1 s (delay)
	//vTaskDelay ( 2*LED_RATE );								// Idem anterior pero mas elegante
	gpioWrite( LED1, OFF );


	TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;	// Tarea periodica cada 1000 ms
	//TickType_t xPeriodicity =  2*LED_RATE;			// Idem anterior pero mas elegante

	TickType_t xLastWakeTime = xTaskGetTickCount();

	// ----- Task repeat for ever -------------------------
	while(TRUE) {
		gpioWrite( LEDB , ON );
		vTaskDelay( 500 / portTICK_RATE_MS );
		//vTaskDelay( LED_RATE);							// Idem anterior pero mas elegante
		gpioWrite( LEDB ,OFF );

		//vTaskDelay( 500 / portTICK_RATE_MS ); //NO USAR!!

		// Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

