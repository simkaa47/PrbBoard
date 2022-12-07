/*
 * control_bit.c
 *
 *  Created on: 6 дек. 2022 г.
 *      Author: Serafim
 */

#include <control_bit.h>

uint16_t set_bit(uint16_t value, uint8_t num)
{
     return value | (1 << num);
}

uint16_t reset_bit(uint16_t value, uint8_t num)
{
     return value & (~(1 << num));
}

uint8_t read_bit(uint16_t value, uint8_t num)
{
    return value & (1<<num);
}
