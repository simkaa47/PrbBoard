/*
 * fram.h
 *
 *  Created on: 26 дек. 2022 г.
 *      Author: ivan.liutov
 */
#include <main.h>

#ifndef INC_FRAM_H_
#define INC_FRAM_H_

#define FRAM_CMD_WRSR			0x01
#define FRAM_CMD_WRITE			0x02
#define FRAM_CMD_READ			0x03
#define FRAM_CMD_WRDI			0x04
#define FRAM_CMD_RDSR			0x05
#define FRAM_CMD_WREN			0x06

int framDataRead (uint16_t address, uint8_t * data, uint16_t size);
int framDataWrite (uint16_t address, uint8_t * data, uint16_t size);

#endif /* INC_FRAM_H_ */
