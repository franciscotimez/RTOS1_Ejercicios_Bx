/*
 * sw_uart_rtos1.h
 *
 *  Created on: 12 sep. 2020
 *      Author: francisco
 */

#ifndef MISPROGRAMAS_RTOS1_EJ_B9_INC_SW_UART_RTOS1_H_
#define MISPROGRAMAS_RTOS1_EJ_B9_INC_SW_UART_RTOS1_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sapi.h"

#define SW_UART_TX GPIO8

static uint16_t ticksData = 0;

void sw_uart_sent(uint8_t byte_a_transmitir);
void sw_uart_config(uint16_t baud);

#endif /* MISPROGRAMAS_RTOS1_EJ_B9_INC_SW_UART_RTOS1_H_ */
