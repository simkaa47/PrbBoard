/*
 * uart_task.c
 *
 *  Created on: 12 дек. 2022 г.
 *      Author: Serafim
 */
#include <main.h>
#include <settings.h>
#include <uart_task.h>


uint8_t uart_input_buffer[2][20];
Uart_Buffer uart_packets[2];


extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart1;

static void RecognizePacket(int index, int bytes_count);


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	int index = 0;
	if (huart == &huart6) {
		index = 1;
	}
	else return;
	RecognizePacket(index,1);
	StartReciveUart(huart);
}

void StartReciveUart(UART_HandleTypeDef *huart)
{
	if (huart == &huart6) {
		HAL_UART_Receive_IT(huart, uart_input_buffer[1], 1);
	}
	return;
}

static void RecognizePacket(int index, int bytes_count)
{

}




