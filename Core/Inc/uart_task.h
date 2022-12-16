/*
 * uart_task.h
 *
 *  Created on: 16 дек. 2022 г.
 *      Author: ivan.liutov
 */

#ifndef INC_UART_TASK_H_
#define INC_UART_TASK_H_

#define UART_INPUT_BUFFER_SZ 256
#define UART_OUTPUT_BUFFER_SZ 256


typedef struct
{
	struct
	{
		uint8_t state;
		uint8_t bytes_rcvd;
		uint8_t bytes_req;
		uint32_t symbol_tmr_ms;
		uint8_t data[UART_INPUT_BUFFER_SZ];
	} RX;
	struct
	{
		uint8_t bytes_req;
		uint8_t data[UART_OUTPUT_BUFFER_SZ];
	} TX;
}Uart_Buffer;

void StartReciveUart(UART_HandleTypeDef *huart);

#endif /* INC_UART_TASK_H_ */
