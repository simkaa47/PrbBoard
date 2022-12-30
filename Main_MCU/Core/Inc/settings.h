/*
 * settings.h
 *
 *  Created on: 5 дек. 2022 г.
 *      Author: Serafim
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#include <stdint.h>

#define FRAM_SETTINGS_ADDR		0x00
#define FRAM_SETTINGS_HEADER	0xABD8

typedef struct
{
	unsigned short in0:1;
	unsigned short in1:1;
	unsigned short in2:1;
	unsigned short in3:1;
	unsigned short in4:1;
	unsigned short in5:1;
	unsigned short in6:1;
	unsigned short in7:1;
	unsigned short in8:1;
	unsigned short in9:1;
	unsigned short in10:1;
	unsigned short in11:1;
}Discrete_Inputs;

typedef struct
{
	unsigned short out0:1;
	unsigned short out1:1;
	unsigned short out2:1;
	unsigned short out3:1;
	unsigned short out4:1;
	unsigned short out5:1;
	unsigned short out6:1;
	unsigned short out7:1;
	unsigned short out8:1;
	unsigned short out9:1;
	unsigned short out10:1;
	unsigned short out11:1;
}Discrete_Outputs;

typedef struct
{
	Discrete_Inputs input_emulation_switches;
	Discrete_Inputs input_emulation_values;
	Discrete_Outputs output_emulation_switches;
	Discrete_Outputs output_emulation_values;
	uint16_t mb_addr;
}Retain;

typedef struct
{

	uint16_t data[50];
}Non_Retain;



typedef struct
{
    Retain retain;
    Non_Retain non_retain;

}Settings_Struct;

typedef struct
{
	Discrete_Inputs d_inputs;
	Discrete_Outputs d_outputs;
	float analog_input;
	float pt100;
    uint16_t data[40];
}Meas_Data;

int settingsLoadFRAM (void);
int settingsSaveFRAM (void);

#endif /* INC_SETTINGS_H_ */
