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
#define STEP 100
#define PERIODO 1000

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

// Task implementation
void myTask( void* taskParmPtr )
{
	uint16_t duty = 0;

	// ---------- CONFIGURACIONES ------------------------------
	gpioWrite( LED1, ON );
	vTaskDelay( 1000 / portTICK_RATE_MS );	 			// Envia la tarea al estado bloqueado durante 1 s (delay)
	//vTaskDelay ( 2*LED_RATE );								// Idem anterior pero mas elegante
	gpioWrite( LED1, OFF );


	TickType_t xPeriodicity =  pdMS_TO_TICKS(PERIODO);	// Tarea periodica cada 1000 ms
	//TickType_t xPeriodicity =  2*LED_RATE;			// Idem anterior pero mas elegante

	TickType_t xLastWakeTime = xTaskGetTickCount();

	// ----- Task repeat for ever -------------------------
	while(TRUE) {
		duty += STEP;
		gpioWrite( LEDB , ON );
		vTaskDelay( pdMS_TO_TICKS(duty) );
		//vTaskDelay( LED_RATE);							// Idem anterior pero mas elegante
		gpioWrite( LEDB ,OFF );

		if(duty == PERIODO - STEP) duty = 0;

		// Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

