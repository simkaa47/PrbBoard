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
	HAL_GPIO_WritePin(S3CS0_GPIO_Port, S3CS0_Pin, GPIO_PIN_RESET);
	result = HAL_SPI_Transmit(&hspi3, buff, 3, 200);
	result |= HAL_SPI_Receive(&hspi3, data, size, 200);
	HAL_GPIO_WritePin(S3CS0_GPIO_Port, S3CS0_Pin, GPIO_PIN_SET);

	return result;
}

int framDataWrite (uint16_t address, uint8_t * data, uint16_t size)
{
	int result = 0, status = 0, cntr = 0;
	uint8_t buff[3];

	buff[0] = FRAM_CMD_WREN;
	HAL_GPIO_WritePin(S3CS0_GPIO_Port, S3CS0_Pin, GPIO_PIN_RESET);
	result = HAL_SPI_Transmit(&hspi3, buff, 1, 200);
	HAL_GPIO_WritePin(S3CS0_GPIO_Port, S3CS0_Pin, GPIO_PIN_SET);
	buff[0] = FRAM_CMD_RDSR;
	while ((status == 0) && (cntr++ < 100))
	{
		HAL_GPIO_WritePin(S3CS0_GPIO_Port, S3CS0_Pin, GPIO_PIN_RESET);
		result |= HAL_SPI_Transmit(&hspi3, buff, 1, 200);
		result |= HAL_SPI_Receive(&hspi3, buff, 1, 200);
		HAL_GPIO_WritePin(S3CS0_GPIO_Port, S3CS0_Pin, GPIO_PIN_SET);
		status = buff[0] & 0x02;
	}
	if (!status)
		result = -2;
	else
	{
		buff[0] = FRAM_CMD_WRITE;
		buff[1] = address >> 8;
		buff[2] = address & 0xFF;
		HAL_GPIO_WritePin(S3CS0_GPIO_Port, S3CS0_Pin, GPIO_PIN_RESET);
		result |= HAL_SPI_Transmit(&hspi3, buff, 3, 200);
		result |= HAL_SPI_Transmit(&hspi3, data, size, 200);
		HAL_GPIO_WritePin(S3CS0_GPIO_Port, S3CS0_Pin, GPIO_PIN_SET);
	}

	return result;
}
