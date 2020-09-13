/*=============================================================================
 * Copyright (c) 2020, Francisco Timez <franciscotimez@gmail.com>
 * All rights reserved.
 * License: mit (see LICENSE.txt)
 * Date: 2020/09/12
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "RTOS1_EJ_B6.h"
#include "macros.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"


#include "sapi.h"

#include <string.h>



/*=====[Definicion de variables globales]==============================*/

uint16_t tiempos[] = {500,1000,2000,4000};
gpioMap_t leds[]   = {LEDB,LED1,LED2,LED3};

#define N_LEDS  sizeof(leds)/sizeof(gpioMap_t)

led_tiempo_t LEDs[N_LEDS];

/*=====[Definicion de prototipos de funcion]=============================*/

void tarea_led( void* taskParmPtr );
void leds_init(void);

void crear_tareas(TaskFunction_t tarea,const char * const nombre);
void crear_tarea(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	boardInit();
	debugPrintConfigUart( UART_USB, 115200 );		// UART for debug messages
	printf( "Se inicio la placa. \n\r" );

	leds_init();
	printf( "Se inicio la estructura de los leds. \n\r" );

	crear_tareas(tarea_led,"tarea_led_");
	printf( "Se crearon las tareas para los leds. \n\r" );

	vTaskStartScheduler(); // Initialize scheduler

	while( true ); // If reach heare it means that the scheduler could not start

	// YOU NEVER REACH HERE, because this program runs directly or on a
	// microcontroller and is not called by any Operating System, as in the
	// case of a PC program.
	return 0;
}

/*******************************************
 * 		leds_init FUNCTION
 * 	Realiza el linkeo entre los tiempos y
 * 	los leds
 *******************************************/
void leds_init(void){
	uint16_t i;

	for(i = 0 ; i < N_LEDS ; i++){
		LEDs[i].led = leds[i];
		LEDs[i].tiempo = tiempos[i];
	}
	return;
}

/*******************************************
 * 		crear_tareas FUNCTION
 * 	Crea el conjunto de tareas para las
 * 	teclas/leds
 *******************************************/
void crear_tareas(TaskFunction_t tarea,const char * const nombre)
{
	uint16_t i;

	char nombre_aux[STR_AUX];

	for(i = 0 ; i < N_LEDS ; i++)
	{
		strcpy(nombre_aux, nombre);
		strcat(nombre_aux, (char)(i+48));
		//          tarea    ,nombre      ,SIZE*STACK,parametro           ,idle+1,0;
		crear_tarea(tarea,
				nombre_aux,
				SIZE,
				&LEDs[i],
				PRIORITY,
				NULL);
	}
	return;
}


/*******************************************
 * 		crear_tarea FUNCTION
 * 	Crea la tarea RTOS y gestiona los errores
 *******************************************/
void crear_tarea(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero)
{
	// Crear tarea en freeRTOS
	BaseType_t res = xTaskCreate(tarea,
			nombre,
			configMINIMAL_STACK_SIZE*stack,
			parametros,
			tskIDLE_PRIORITY+prioridad,
			puntero);                         		// Puntero a la tarea creada en el sistema

	// Gestion de errores
	if(res == pdFAIL)
	{
		gpioWrite( LEDR, ON );
		printf( "Error al crear las tareas. \n\r" );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}
	return;
}

/*******************************************
 * 		tarea_led FUNCTION
 * 	Tarea que realiza el led
 *******************************************/
void tarea_led( void* taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------
	led_tiempo_t* config = (led_tiempo_t*) taskParmPtr;

	TickType_t xPeriodicity =  config->tiempo;					// Idem anterior pero mas elegante

	TickType_t xLastWakeTime = xTaskGetTickCount();

	while(TRUE){
		gpioToggle( config->led );
		// Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}
