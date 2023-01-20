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
#include <string.h>


uint8_t uart_input_buffer[2][UART_INPUT_BUFFER_SZ];
uint8_t rs_answer[UART_OUTPUT_BUFFER_SZ];



extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern osMailQId uart_queue; // Id очереди для uart_task
extern Meas_Data meas_data;
extern Settings_Struct settings;


static int RecognizePacket(Uart_Queue_Struct *request);
static void StartReceive(int index);
static void StartReciveUartAll();
static void Transmit(UART_HandleTypeDef *huart, uint8_t *p, uint16_t size);



void uart_thread(void *argument)
{
	osEvent event;
	Uart_Queue_Struct *queue_arg;
	RsReInit();
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
			if (huart1.hdmarx->State==HAL_DMA_STATE_READY) {
				HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_input_buffer[0], UART_INPUT_BUFFER_SZ);
				__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
			}
			break;
		case 1:
			if(huart6.hdmarx->State==HAL_DMA_STATE_READY){
				HAL_UARTEx_ReceiveToIdle_DMA(&huart6, uart_input_buffer[1], UART_INPUT_BUFFER_SZ);
				__HAL_DMA_DISABLE_IT(&hdma_usart6_rx,DMA_IT_HT);
			}
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
		float analog = 0;
		if(request->inpit_size==6 && *(request->input_pointer)==0x0A && *(request->input_pointer+5)==0x0D)
		{
			memcpy(&analog,(request->input_pointer)+1,sizeof(float));
			meas_data.analog_input = analog * 10;
		}
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

int RsReInit (void)
{
	int result = 0;
	result = HAL_UART_DeInit(&huart1);
	if (result == HAL_OK)
	{
		uint32_t parity = 0;
		switch (settings.retain.rs_sett.parity) {
			case 0:
				parity = UART_PARITY_NONE;
				break;
			case 1:
				parity = UART_PARITY_EVEN;
				break;
			default:
				parity = UART_PARITY_ODD;
				break;
		}
		huart6.Instance = USART6;
		huart6.Init.BaudRate = settings.retain.rs_sett.baudrate>0 ? settings.retain.rs_sett.baudrate : 9600;
		huart6.Init.WordLength = UART_WORDLENGTH_8B;
		huart6.Init.StopBits = UART_STOPBITS_1;
		huart6.Init.Parity = parity;
		huart6.Init.Mode = UART_MODE_TX_RX;
		huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart6.Init.OverSampling = UART_OVERSAMPLING_16;
		result = HAL_UART_Init(&huart6);
	}
  return result;
}




