/*
 * uart_task.c
 *
 *  Created on: 12 дек. 2022 г.
 *      Author: Serafim
 */
#include <main.h>
#include <settings.h>

extern UART_HandleTypeDef huart6;
extern Meas_Data meas_data;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	int t = 0;
	meas_data.data[25]++;
	char message[] = "hello";;
	HAL_UART_Transmit(huart, (uint8_t*)message, 5, 10);
	HAL_UART_Receive_IT(huart,(uint8_t*)meas_data.data,1);
}




