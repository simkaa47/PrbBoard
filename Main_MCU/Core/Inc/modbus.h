/*
 * modbus.h
 *
 *  Created on: Dec 1, 2022
 *      Author: Serafim
 */

#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_

#include <stdint.h>

#define READ_HOLDING_REGS 3
#define READ_INPUT_REGS 4
#define WRITE_SINGLE_REG 6
#define WRITE_MULTIPLY_REGS 16

typedef enum
{
	RS485,
	ETHERNET
}ModbusSource;

int ModbusParse(uint8_t *request, uint16_t req_length, uint8_t *answer, ModbusSource source);
void ModbusInit();


#endif /* INC_MODBUS_H_ */
