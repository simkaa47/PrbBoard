/*
 * uart.c
 *
 *  Created on: Dec 23, 2022
 *      Author: ivan.liutov
 */
#include <uart.h>

extern UART_HandleTypeDef huart1;
uint8_t out_buffer[20];


static void Start_Transmit(uint8_t *p, uint8_t size);

static void Start_Transmit(uint8_t *p, uint8_t size)
{
	//HAL_UART_Transmit_DMA(&huart1, p, size);
	HAL_UART_Transmit(&huart1, p, size, 100);
}

void Send_Adc(float value)
{
	out_buffer[0]=0x0A;
	memcpy(out_buffer+1, &value,4);
	out_buffer[5]=0x0D;
	Start_Transmit(out_buffer, 6);
}

