/*
 * dio.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Serafim
 */
#include <string.h>
#include <main.h>
#include <settings.h>

extern Settings_Struct settings;
extern Meas_Data meas_data;

void Read_Inputs()
{
	meas_data.d_inputs.in0 	= settings.input_emulation_switches.in0  ? settings.input_emulation_values.in0 :  HAL_GPIO_ReadPin(DIN_0_GPIO_Port, DIN_0_Pin);
	meas_data.d_inputs.in1 	= settings.input_emulation_switches.in1  ? settings.input_emulation_values.in1 :  HAL_GPIO_ReadPin(DIN_1_GPIO_Port, DIN_1_Pin);
	meas_data.d_inputs.in2 	= settings.input_emulation_switches.in2  ? settings.input_emulation_values.in2 :  HAL_GPIO_ReadPin(DIN_2_GPIO_Port, DIN_2_Pin);
	meas_data.d_inputs.in3 	= settings.input_emulation_switches.in3  ? settings.input_emulation_values.in3 :  HAL_GPIO_ReadPin(DIN_3_GPIO_Port, DIN_3_Pin);
	meas_data.d_inputs.in4 	= settings.input_emulation_switches.in4  ? settings.input_emulation_values.in4 :  HAL_GPIO_ReadPin(DIN_4_GPIO_Port, DIN_4_Pin);
	meas_data.d_inputs.in5 	= settings.input_emulation_switches.in5  ? settings.input_emulation_values.in5 :  HAL_GPIO_ReadPin(DIN_5_GPIO_Port, DIN_5_Pin);
	meas_data.d_inputs.in6 	= settings.input_emulation_switches.in6  ? settings.input_emulation_values.in6 :  HAL_GPIO_ReadPin(DIN_6_GPIO_Port, DIN_6_Pin);
	meas_data.d_inputs.in7 	= settings.input_emulation_switches.in7  ? settings.input_emulation_values.in7 :  HAL_GPIO_ReadPin(DIN_7_GPIO_Port, DIN_7_Pin);
	meas_data.d_inputs.in8 	= settings.input_emulation_switches.in8  ? settings.input_emulation_values.in8 :  HAL_GPIO_ReadPin(DIN_8_GPIO_Port, DIN_8_Pin);
	meas_data.d_inputs.in9 	= settings.input_emulation_switches.in9  ? settings.input_emulation_values.in9 :  HAL_GPIO_ReadPin(DIN_9_GPIO_Port, DIN_9_Pin);
	meas_data.d_inputs.in10 = settings.input_emulation_switches.in10 ? settings.input_emulation_values.in10 : HAL_GPIO_ReadPin(DIN_10_GPIO_Port, DIN_10_Pin);
	meas_data.d_inputs.in11 = settings.input_emulation_switches.in11 ? settings.input_emulation_values.in11 : HAL_GPIO_ReadPin(DIN_11_GPIO_Port, DIN_11_Pin);
}

void Write_Outputs()
{
	meas_data.d_outputs.out0  = settings.output_emulation_switches.out0  ? settings.output_emulation_values.out0  :  0;
	meas_data.d_outputs.out1  = settings.output_emulation_switches.out1  ? settings.output_emulation_values.out1  :  0;
	meas_data.d_outputs.out2  = settings.output_emulation_switches.out2  ? settings.output_emulation_values.out2  :  0;
	meas_data.d_outputs.out3  = settings.output_emulation_switches.out3  ? settings.output_emulation_values.out3  :  0;
	meas_data.d_outputs.out4  = settings.output_emulation_switches.out4  ? settings.output_emulation_values.out4  :  0;
	meas_data.d_outputs.out5  = settings.output_emulation_switches.out5  ? settings.output_emulation_values.out5  :  0;
	meas_data.d_outputs.out6  = settings.output_emulation_switches.out6  ? settings.output_emulation_values.out6  :  0;
	meas_data.d_outputs.out7  = settings.output_emulation_switches.out7  ? settings.output_emulation_values.out7  :  0;
	meas_data.d_outputs.out8  = settings.output_emulation_switches.out8  ? settings.output_emulation_values.out8  :  0;
	meas_data.d_outputs.out9  = settings.output_emulation_switches.out9  ? settings.output_emulation_values.out9  :  0;
	meas_data.d_outputs.out10 = settings.output_emulation_switches.out10 ? settings.output_emulation_values.out10 :  0;
	meas_data.d_outputs.out11 = settings.output_emulation_switches.out11 ? settings.output_emulation_values.out11 :  0;
	HAL_GPIO_WritePin(DOUT_0_GPIO_Port, DOUT_0_Pin, meas_data.d_outputs.out0);
	HAL_GPIO_WritePin(DOUT_1_GPIO_Port, DOUT_1_Pin, meas_data.d_outputs.out1);
	HAL_GPIO_WritePin(DOUT_2_GPIO_Port, DOUT_2_Pin, meas_data.d_outputs.out2);
	HAL_GPIO_WritePin(DOUT_3_GPIO_Port, DOUT_3_Pin, meas_data.d_outputs.out3);
	HAL_GPIO_WritePin(DOUT_4_GPIO_Port, DOUT_4_Pin, meas_data.d_outputs.out4);
	HAL_GPIO_WritePin(DOUT_5_GPIO_Port, DOUT_5_Pin, meas_data.d_outputs.out5);
	HAL_GPIO_WritePin(DOUT_6_GPIO_Port, DOUT_6_Pin, meas_data.d_outputs.out6);
	HAL_GPIO_WritePin(DOUT_7_GPIO_Port, DOUT_7_Pin, meas_data.d_outputs.out7);
	HAL_GPIO_WritePin(DOUT_8_GPIO_Port, DOUT_8_Pin, meas_data.d_outputs.out8);
	HAL_GPIO_WritePin(DOUT_9_GPIO_Port, DOUT_9_Pin, meas_data.d_outputs.out9);
	HAL_GPIO_WritePin(DOUT_10_GPIO_Port, DOUT_10_Pin, meas_data.d_outputs.out10);
	HAL_GPIO_WritePin(DOUT_11_GPIO_Port, DOUT_11_Pin, meas_data.d_outputs.out11);
}

