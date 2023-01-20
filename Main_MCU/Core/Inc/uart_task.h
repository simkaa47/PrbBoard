/*
 * uart_task.h
 *
 *  Created on: 16 дек. 2022 г.
 *      Author: ivan.liutov
 */
#include <main.h>

#ifndef INC_UART_TASK_H_
#define INC_UART_TASK_H_

#define UART_INPUT_BUFFER_SZ 256
#define UART_OUTPUT_BUFFER_SZ 256
#define UART_QUEUE_SIZE 3

typedef struct
{
	uint8_t *input_pointer;
	uint8_t *output_pointer;
	uint16_t inpit_size;
	uint16_t output_size;
	UART_HandleTypeDef *huart;
}Uart_Queue_Struct;


void uart_thread(void *argument);
int RsReInit (void);


#endif /* INC_UART_TASK_H_ */
