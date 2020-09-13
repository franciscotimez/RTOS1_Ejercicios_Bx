/*
 * sw_uart_rtos1.c
 *
 *  Created on: 12 sep. 2020
 *      Author: francisco
 */
#include "sw_uart_rtos1.h"

void sw_uart_sent(uint8_t byte_a_transmitir){
	uint16_t i;
	uint8_t buffer = byte_a_transmitir;
	if(ticksData != 0){
		gpioWrite(SW_UART_TX, OFF);
		vTaskDelay(ticksData);
		for(i = 0; i < 8; i++)
		{
			if((buffer%2) == 1)
				gpioWrite(SW_UART_TX, ON);
			else
				gpioWrite(SW_UART_TX, OFF);
			vTaskDelay(ticksData);
			buffer = buffer>>1;
		}
		gpioWrite(SW_UART_TX, ON);
	}
}

void sw_uart_config(uint16_t baud){
	gpioInit(SW_UART_TX, GPIO_OUTPUT);
	gpioWrite(SW_UART_TX, ON);
	ticksData = pdMS_TO_TICKS(1000/baud);
	return;
}
