/*
 * fram.c
 *
 *  Created on: 26 дек. 2022 г.
 *      Author: ivan.liutov
 */

#include <fram.h>

extern SPI_HandleTypeDef hspi3;

int framDataRead (uint16_t address, uint8_t * data, uint16_t size)
{
	int result = 0;
	uint8_t buff[3];

	buff[0] = FRAM_CMD_READ;
	buff[1] = address >> 8;
	buff[2] = address & 0xFF;
	HAL_GPIO_WritePin(S1CS_ROM_GPIO_Port, S1CS_ROM_Pin, GPIO_PIN_RESET);
	result = HAL_SPI_Transmit(&hspi1, buff, 3, 200);
	result |= HAL_SPI_Receive(&hspi1, data, size, 200);
	HAL_GPIO_WritePin(S1CS_ROM_GPIO_Port, S1CS_ROM_Pin, GPIO_PIN_SET);

	return result;
}
