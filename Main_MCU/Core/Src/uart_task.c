/*
 * uart_task.c
 *
 *  Created on: 12 дек. 2022 г.
 *      Author: Serafim
 */
#include <settings.h>
#include <uart_task.h>
#include <modbus.h>
#include <cmsis_os.h>


uint8_t uart_input_buffer[2][UART_INPUT_BUFFER_SZ];
uint8_t rs_answer[UART_OUTPUT_BUFFER_SZ];



extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern osMailQId uart_queue; // Id очереди для uart_task


static int RecognizePacket(Uart_Queue_Struct *request);
static void StartReceive(int index);
static void StartReciveUartAll();
static void Transmit(UART_HandleTypeDef *huart, uint8_t *p, uint16_t size);


void uart_thread(void *argument)
{
	osEvent event;
	Uart_Queue_Struct *queue_arg;
	StartReciveUartAll();
	while(1)
	{
		event = osMailGet(uart_queue, osWaitForever);
		if(event.status == osEventMail)
		{
			int result = 0;
			queue_arg = event.value.p;
			result = RecognizePacket(queue_arg);
			if(result)Transmit(queue_arg->huart, queue_arg->output_pointer, result);
			StartReceive(1);
			osMailFree(uart_queue, queue_arg);

		}
	}
}


static void StartReciveUartAll()
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
	Uart_Queue_Struct *queue_arg;
	uint8_t *input_pointer = NULL;
	uint8_t *output_pointer = NULL;
	if (huart->Instance==USART1) {
		input_pointer = uart_input_buffer[0];
		StartReceive(0);
	}
	else if(huart->Instance==USART6){
		input_pointer = uart_input_buffer[1];
		output_pointer = rs_answer;

	}
	else return;
	queue_arg = osMailAlloc(uart_queue, 0);
	if(queue_arg==NULL)return;
	queue_arg->inpit_size = size;
	queue_arg->input_pointer = input_pointer;
	queue_arg->output_pointer = output_pointer;
	queue_arg->huart = huart;
	osMailPut(uart_queue, queue_arg);
}

static int RecognizePacket(Uart_Queue_Struct *request)
{
	int result = 0;
	if(request->input_pointer==NULL)return 0;
	if(request->huart->Instance==USART1)
	{
		// Написать обработку пакета от АЦП
		return 0;
	}
	else if (request->huart->Instance==USART6) {
		if(request->output_pointer == NULL)return 0;
		result = ModbusParse(request->input_pointer, request->inpit_size, request->output_pointer, RS485);
	}
	return result;
}

static void Transmit(UART_HandleTypeDef *huart, uint8_t *p, uint16_t size)
{
	if(huart->Instance==USART6)
	{
		HAL_GPIO_WritePin(RS485_TX_GPIO_Port, RS485_TX_Pin, SET);
		HAL_UART_Transmit(huart, p, size, 1000);
		HAL_GPIO_WritePin(RS485_TX_GPIO_Port, RS485_TX_Pin, RESET);

	}
}




