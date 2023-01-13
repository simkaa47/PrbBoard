/*
 * dio.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Serafim
 */
#include <string.h>
#include <main.h>
#include <settings.h>
#include <dio.h>

extern Settings_Struct settings;
extern Meas_Data meas_data;

uint8_t filter_state[INPUT_NUMBER];
uint8_t filters_inputs[INPUT_NUMBER];
DI d_inputs;
DO d_outputs;

static void Filter_Inputs();





void Read_Inputs()
{
	Filter_Inputs();
	meas_data.d_inputs.in0 	= settings.retain.input_emulation_switches.in0  ? settings.retain.input_emulation_values.in0 :  filters_inputs[0];
	meas_data.d_inputs.in1 	= settings.retain.input_emulation_switches.in1  ? settings.retain.input_emulation_values.in1 :  filters_inputs[1];
	meas_data.d_inputs.in2 	= settings.retain.input_emulation_switches.in2  ? settings.retain.input_emulation_values.in2 :  filters_inputs[2];
	meas_data.d_inputs.in3 	= settings.retain.input_emulation_switches.in3  ? settings.retain.input_emulation_values.in3 :  filters_inputs[3];
	meas_data.d_inputs.in4 	= settings.retain.input_emulation_switches.in4  ? settings.retain.input_emulation_values.in4 :  filters_inputs[4];
	meas_data.d_inputs.in5 	= settings.retain.input_emulation_switches.in5  ? settings.retain.input_emulation_values.in5 :  filters_inputs[5];
	meas_data.d_inputs.in6 	= settings.retain.input_emulation_switches.in6  ? settings.retain.input_emulation_values.in6 :  filters_inputs[6];
	meas_data.d_inputs.in7 	= settings.retain.input_emulation_switches.in7  ? settings.retain.input_emulation_values.in7 :  filters_inputs[7];
	meas_data.d_inputs.in8 	= settings.retain.input_emulation_switches.in8  ? settings.retain.input_emulation_values.in8 :  filters_inputs[8];
	meas_data.d_inputs.in9 	= settings.retain.input_emulation_switches.in9  ? settings.retain.input_emulation_values.in9 :  filters_inputs[9];
	meas_data.d_inputs.in10 = settings.retain.input_emulation_switches.in10 ? settings.retain.input_emulation_values.in10 : filters_inputs[10];
	meas_data.d_inputs.in11 = settings.retain.input_emulation_switches.in11 ? settings.retain.input_emulation_values.in11 : filters_inputs[11];

	d_inputs.uz_prob_fail 				= meas_data.d_inputs.in0;
	d_inputs.sq_kovsh_prob_right_1 		= meas_data.d_inputs.in1;
	d_inputs.sq_kovsh_prob_right_2		= meas_data.d_inputs.in2;
	d_inputs.sq_kovsh_prob_left_1 		= meas_data.d_inputs.in3;
	d_inputs.sq_kovsh_prob_left_2		= meas_data.d_inputs.in4;
	d_inputs.sb_stop					= meas_data.d_inputs.in5;
	d_inputs.sb_make_proba				= meas_data.d_inputs.in6;
	d_inputs.sb_vozvrat					= meas_data.d_inputs.in7;
	d_inputs.sb_auto_local				= meas_data.d_inputs.in8;
	d_inputs.sq_kanistra				= meas_data.d_inputs.in9;
	d_inputs.sq_kanistra_door			= meas_data.d_inputs.in10;




}

void Write_Outputs()
{
	meas_data.d_outputs.out0  = settings.retain.output_emulation_switches.out0  ? settings.retain.output_emulation_values.out0  :  d_outputs.uz_prob_forv;
	meas_data.d_outputs.out1  = settings.retain.output_emulation_switches.out1  ? settings.retain.output_emulation_values.out1  :  d_outputs.uz_prob_rev;
	meas_data.d_outputs.out2  = settings.retain.output_emulation_switches.out2  ? settings.retain.output_emulation_values.out2  :  d_outputs.prob_ready;
	meas_data.d_outputs.out3  = settings.retain.output_emulation_switches.out3  ? settings.retain.output_emulation_values.out3  :  0;
	meas_data.d_outputs.out4  = settings.retain.output_emulation_switches.out4  ? settings.retain.output_emulation_values.out4  :  0;
	meas_data.d_outputs.out5  = settings.retain.output_emulation_switches.out5  ? settings.retain.output_emulation_values.out5  :  0;
	meas_data.d_outputs.out6  = settings.retain.output_emulation_switches.out6  ? settings.retain.output_emulation_values.out6  :  0;
	meas_data.d_outputs.out7  = settings.retain.output_emulation_switches.out7  ? settings.retain.output_emulation_values.out7  :  0;
	meas_data.d_outputs.out8  = settings.retain.output_emulation_switches.out8  ? settings.retain.output_emulation_values.out8  :  0;
	meas_data.d_outputs.out9  = settings.retain.output_emulation_switches.out9  ? settings.retain.output_emulation_values.out9  :  0;
	meas_data.d_outputs.out10 = settings.retain.output_emulation_switches.out10 ? settings.retain.output_emulation_values.out10 :  0;
	meas_data.d_outputs.out11 = settings.retain.output_emulation_switches.out11 ? settings.retain.output_emulation_values.out11 :  0;
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

static void Filter_Inputs()
{
	uint8_t inputs[INPUT_NUMBER];

	inputs[0] = !HAL_GPIO_ReadPin(DIN_0_GPIO_Port, DIN_0_Pin);
	inputs[1] = !HAL_GPIO_ReadPin(DIN_1_GPIO_Port, DIN_1_Pin);
	inputs[2] = !HAL_GPIO_ReadPin(DIN_2_GPIO_Port, DIN_2_Pin);
	inputs[3] = !HAL_GPIO_ReadPin(DIN_3_GPIO_Port, DIN_3_Pin);
	inputs[4] = !HAL_GPIO_ReadPin(DIN_4_GPIO_Port, DIN_4_Pin);
	inputs[5] = !HAL_GPIO_ReadPin(DIN_5_GPIO_Port, DIN_5_Pin);
	inputs[6] = !HAL_GPIO_ReadPin(DIN_6_GPIO_Port, DIN_6_Pin);
	inputs[7] = !HAL_GPIO_ReadPin(DIN_7_GPIO_Port, DIN_7_Pin);
	inputs[8] = !HAL_GPIO_ReadPin(DIN_8_GPIO_Port, DIN_8_Pin);
	inputs[9] = !HAL_GPIO_ReadPin(DIN_9_GPIO_Port, DIN_9_Pin);
	inputs[10] = !HAL_GPIO_ReadPin(DIN_10_GPIO_Port, DIN_10_Pin);
	inputs[11] = !HAL_GPIO_ReadPin(DIN_11_GPIO_Port, DIN_11_Pin);
	for (int i = 0; i < INPUT_NUMBER; ++i) {
		if(inputs[i])
		{
			filter_state[i] = filter_state[i]<ANTIDREBEZG_CNT ? filter_state[i]+1 : ANTIDREBEZG_CNT;
		}
		else
		{
			filter_state[i] = filter_state[i]>0 ? filter_state[i]-1 : 0;
		}
		if (filter_state[i]>=ANTIDREBEZG_CNT) {
			filters_inputs[i]=1;
		} else if(filter_state[i]==0) {
			filters_inputs[i]=0;
		}
	}

}

