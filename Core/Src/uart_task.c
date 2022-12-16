/*
 * uart_task.c
 *
 *  Created on: 12 дек. 2022 г.
 *      Author: Serafim
 */
#include <main.h>
#include <settings.h>
#include <uart_task.h>


uint8_t uart_input_buffer[2][UART_INPUT_BUFFER_SZ];



extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;

static void RecognizePacket(int index, int bytes_count);
static void StartReceive(int index);



void StartReciveUartAll()
{
	for (int i = 0; i < 2; ++i) {
		StartReceive(i);
	}
}


static void StartReceive(int index)
{
	switch (index) {
		case 0:
			HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_input_buffer[0], UART_INPUT_BUFFER_SZ);
			__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
			break;
		case 1:
			HAL_UARTEx_ReceiveToIdle_DMA(&huart6, uart_input_buffer[1], UART_INPUT_BUFFER_SZ);
			__HAL_DMA_DISABLE_IT(&hdma_usart6_rx,DMA_IT_HT);
			break;
		default:
			break;
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
	if (huart->Instance==USART1) {
		StartReceive(0);
	}
	else if(huart->Instance==USART6){
		StartReceive(1);
	}
	else return;

}

static void RecognizePacket(int index, int bytes_count)
{

}




