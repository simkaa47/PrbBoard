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
	unsigned in0:1;
	unsigned in1:1;
	unsigned in2:1;
	unsigned in3:1;
	unsigned in4:1;
	unsigned in5:1;
	unsigned in6:1;
	unsigned in7:1;
	unsigned in8:1;
	unsigned in9:1;
	unsigned in10:1;
	unsigned in11:1;
}Discrete_Inputs;

typedef struct
{
	unsigned out0:1;
	unsigned out1:1;
	unsigned out2:1;
	unsigned out3:1;
	unsigned out4:1;
	unsigned out5:1;
	unsigned out6:1;
	unsigned out7:1;
	unsigned out8:1;
	unsigned out9:1;
	unsigned out10:1;
	unsigned out11:1;
}Discrete_Outputs;

typedef struct
{
	Discrete_Inputs input_emulation_switches;
	Discrete_Inputs input_emulation_values;
	Discrete_Outputs output_emulation_switches;
	Discrete_Outputs output_emulation_values;
    uint16_t data[50];
    uint16_t mb_addr;
}Settings_Struct;

typedef struct
{
	Discrete_Inputs d_inputs;
	Discrete_Outputs d_outputs;
    uint16_t data[40];
}Meas_Data;



#endif /* INC_SETTINGS_H_ */
