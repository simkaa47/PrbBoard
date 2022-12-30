/*
 * analog.h
 *
 *  Created on: Dec 29, 2022
 *      Author: Serafim
 */

#ifndef INC_ANALOG_H_
#define INC_ANALOG_H_

#include <main.h>

#define PT100_INPUT_BUFFER_SIZE 40
#define V_REF_VALUE *VREFINT_CAL_ADDR

typedef struct
{
	uint16_t Pt100_0;
	uint16_t Pt100_1;
	uint16_t Pt100_2;
	uint16_t Pt100_3;
	uint16_t Pt100_4;
	uint16_t V_ref;

}Pt100_Struct;

void Analog_Start(void);
void Analog_Read(void);

#endif /* INC_ANALOG_H_ */
