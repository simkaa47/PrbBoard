/*
 * settings.c
 *
 *  Created on: 5 дек. 2022 г.
 *      Author: Serafim
 */
#include <settings.h>
#include "FreeRTOS.h"
#include <fram.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <task.h>

Settings_Struct settings;
Meas_Data meas_data;

static uint8_t checksumCalc (uint8_t * data, uint16_t size);

int settingsLoadFRAM (void)
{
	int result = 0;
	uint16_t header;

	result = framDataRead(FRAM_SETTINGS_ADDR, (uint8_t *) &header, sizeof(header));
	if (!result)
	{
		if (header == FRAM_SETTINGS_HEADER)
		{
			result = framDataRead(FRAM_SETTINGS_ADDR + sizeof(header), (uint8_t *) &(settings.retain), sizeof(Retain));
			if (!result)
			{

				uint8_t xor = checksumCalc((uint8_t *) &(settings.retain), sizeof(Retain));
				uint8_t checksum;
				result = framDataRead(FRAM_SETTINGS_ADDR + sizeof(header) + sizeof(Retain), &checksum, sizeof(checksum));
 				if (!result)
				{
					if (xor != checksum)
						result = -2;
				}
				else
					result = -1;
			}
			else
				result = -1;
		}
		else
			result = -1;
	}

	return result;
}

int settingsSaveFRAM (void)
{
	int result = 0;
	uint16_t header = FRAM_SETTINGS_HEADER, address = FRAM_SETTINGS_ADDR;

	result = framDataWrite(address, (uint8_t *) &header, sizeof(header));
	address += sizeof(header);
	vTaskSuspendAll();	// Останавливаем планировщик, чтобы предотвратить фоновое изменение структуры settings
	result |= framDataWrite(address, (uint8_t *) &(settings.retain), sizeof(Retain));
	address += sizeof(Retain);
	uint8_t xor = checksumCalc((uint8_t *) &(settings.retain), sizeof(Retain));
	xTaskResumeAll();	// Возобновляем работу планировщика
	result |= framDataWrite(address, &xor, sizeof(xor));

	return result;
}

static uint8_t checksumCalc (uint8_t * data, uint16_t size)
{
	uint8_t xor = 0;
	for (int i = 0; i < size; i++)
		xor ^= data[i];
	return xor;
}





