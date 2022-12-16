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




void StartReciveUart()
{



	HAL_UARTEx_ReceiveToIdle_DMA(&huart6, uart_input_buffer[1], UART_INPUT_BUFFER_SZ);
	__HAL_DMA_DISABLE_IT(&hdma_usart6_rx,DMA_IT_HT);
}

static void RecognizePacket(int index, int bytes_count)
{

}




