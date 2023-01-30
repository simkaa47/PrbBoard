/*
 * keyboard.h
 *
 *  Created on: Dec 22, 2022
 *      Author: Serafim
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#include <main.h>




#define COLUMNS_COUNT 6
#define ROWS_COUNT 6
#define KEY_SENCE 25

#define C0_HIGH HAL_GPIO_WritePin(KEYB_C0_GPIO_Port, KEYB_C0_Pin, SET)
#define C0_LOW HAL_GPIO_WritePin(KEYB_C0_GPIO_Port, KEYB_C0_Pin, RESET)

#define C1_HIGH HAL_GPIO_WritePin(KEYB_C1_GPIO_Port, KEYB_C1_Pin, SET)
#define C1_LOW HAL_GPIO_WritePin(KEYB_C1_GPIO_Port, KEYB_C1_Pin, RESET)

#define C2_HIGH HAL_GPIO_WritePin(KEYB_C2_GPIO_Port, KEYB_C2_Pin, SET)
#define C2_LOW HAL_GPIO_WritePin(KEYB_C2_GPIO_Port, KEYB_C2_Pin, RESET)

#define C3_HIGH HAL_GPIO_WritePin(KEYB_C3_GPIO_Port, KEYB_C3_Pin, SET)
#define C3_LOW HAL_GPIO_WritePin(KEYB_C3_GPIO_Port, KEYB_C3_Pin, RESET)

#define C4_HIGH HAL_GPIO_WritePin(KEYB_C4_GPIO_Port, KEYB_C4_Pin, SET)
#define C4_LOW HAL_GPIO_WritePin(KEYB_C4_GPIO_Port, KEYB_C4_Pin, RESET)

#define C5_HIGH HAL_GPIO_WritePin(KEYB_C5_GPIO_Port, KEYB_C5_Pin, SET)
#define C5_LOW HAL_GPIO_WritePin(KEYB_C5_GPIO_Port, KEYB_C5_Pin, RESET)

#define R0_IN HAL_GPIO_ReadPin(KEYB_R0_GPIO_Port, KEYB_R0_Pin);
#define R1_IN HAL_GPIO_ReadPin(KEYB_R1_GPIO_Port, KEYB_R1_Pin);
#define R2_IN HAL_GPIO_ReadPin(KEYB_R2_GPIO_Port, KEYB_R2_Pin);
#define R3_IN HAL_GPIO_ReadPin(KEYB_R3_GPIO_Port, KEYB_R3_Pin);
#define R4_IN HAL_GPIO_ReadPin(KEYB_R4_GPIO_Port, KEYB_R4_Pin);
#define R5_IN HAL_GPIO_ReadPin(KEYB_R5_GPIO_Port, KEYB_R5_Pin);

void keyscan_thread(void *argument);


#endif /* INC_KEYBOARD_H_ */
