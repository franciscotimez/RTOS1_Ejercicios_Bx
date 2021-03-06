/*=============================================================================
 * Copyright (c) 2020, Francisco Timez <franciscotimez@gmail.com>
 * All rights reserved.
 * License: mit (see LICENSE.txt)
 * Date: 2020/09/10
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "RTOS1_EJ_B4.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"


#include "sapi.h"
#include "fsm_debounce_RTOS.h"

#include <string.h>

/*=====[Definicion de macros]==============================*/
#define RATE 40
#define LED_RATE pdMS_TO_TICKS(RATE)


#define PRIORITY 1
#define SIZE     1
#define STR_AUX  20

#define BUTTON_RATE 1

DEBUG_PRINT_ENABLE;

typedef struct{
	TeclaRTOSfsm_t tecla;
	gpioMap_t		led;
} led_tecla_t;

/*=====[Definicion de variables globales]==============================*/

gpioMap_t teclas[] = {TEC1,TEC2,TEC3,TEC4};
gpioMap_t leds[]   = {LEDB,LED1,LED2,LED3};

#define N_TECLAS  sizeof(teclas)/sizeof(gpioMap_t)		// 4 * gpioMap_t / gpioMap_t = 4

led_tecla_t teclaLED[N_TECLAS];

/*=====[Definicion de prototipos de funcion]=============================*/

void tarea_tecla( void* taskParmPtr );  // Task declaration
void tarea_led( void* taskParmPtr );
void tecla_led_init(void);
void crear_tareas(TaskFunction_t tarea,const char * const nombre);
void crear_tarea(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	boardInit();
	debugPrintConfigUart( UART_USB, 115200 );		// UART for debug messages
	printf( "Se inicio la placa. \n\r" );

	tecla_led_init();
	printf( "Se inicio la estructura de las teclas y leds. \n\r" );

	// Create a task in freeRTOS with dynamic memory
	crear_tareas(tarea_tecla,"tarea_tecla_");
	printf( "Se crearon las tareas para las teclas. \n\r" );

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
 * 		tecla_led_init FUNCTION
 * 	Realiza el linkeo entre los leds y
 * 	las teclas
 *******************************************/
void tecla_led_init(void){
	uint16_t i;

	for(i = 0 ; i < N_TECLAS ; i++)
	{
		teclaLED[i].led = leds[i];
		teclaLED[i].tecla.tecla_fsm = teclas[i];
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

	for(i = 0 ; i < N_TECLAS ; i++)
	{
		strcpy(nombre_aux, nombre);
		strcat(nombre_aux, (char)(i+48));
		//          tarea    ,nombre      ,SIZE*STACK,parametro           ,idle+1,0;
		crear_tarea(tarea,
				nombre_aux,
				SIZE,
				&teclaLED[i],
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
 * 		tarea_tecla FUNCTION
 * 	Tarea que realiza la tecla
 *******************************************/
void tarea_tecla( void* taskParmPtr )
{
	led_tecla_t* config = (led_tecla_t*)taskParmPtr;

	fsmButtonInit( &(config->tecla) );

	while( TRUE )
	{
		fsmButtonUpdate(&(config->tecla));
		//vTaskDelay( BUTTON_RATE / portTICK_RATE_MS );
		vTaskDelay ( pdMS_TO_TICKS(BUTTON_RATE) );
	}
}

/*******************************************
 * 		tarea_led FUNCTION
 * 	Tarea que realiza el led
 *******************************************/
void tarea_led( void* taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------
	led_tecla_t* config = (led_tecla_t*) taskParmPtr;

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		if(config->tecla.tiempo_medido != 0){
			gpioWrite( config->led , ON );
			vTaskDelay( config->tecla.tiempo_medido );
			gpioWrite( config->led , OFF );

			config->tecla.tiempo_medido = 0;
		}
	}
}
