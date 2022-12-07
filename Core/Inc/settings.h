/*
 * settings.h
 *
 *  Created on: 5 дек. 2022 г.
 *      Author: Serafim
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#include <stdint.h>

typedef struct
{
    uint16_t data[50];
    uint16_t mb_addr;
}Settings_Struct;

typedef struct
{
    uint16_t data[40];
}Meas_Data;



#endif /* INC_SETTINGS_H_ */
