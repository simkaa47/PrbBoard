/*
 * control_bit.h
 *
 *  Created on: 6 дек. 2022 г.
 *      Author: Serafim
 */

#ifndef INC_CONTROL_BIT_H_
#define INC_CONTROL_BIT_H_

#include <stdint.h>
#include <stddef.h>
uint16_t set_bit(uint16_t value, uint8_t num);

uint16_t reset_bit(uint16_t value, uint8_t num);

uint8_t read_bit(uint16_t value, uint8_t num);

#endif /* INC_CONTROL_BIT_H_ */
