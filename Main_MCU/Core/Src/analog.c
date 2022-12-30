/*
 * analog.c
 *
 *  Created on: Dec 29, 2022
 *      Author: Serafim
 */
#include <analog.h>

Pt100_Struct pt100_input_buffer[PT100_INPUT_BUFFER_SIZE];
uint32_t temp;



extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim3;

void Analog_Start(void)
{

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&pt100_input_buffer, sizeof(pt100_input_buffer)/2);
	HAL_TIM_Base_Start(&htim3);

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	float pt100_0, pt100_1, pt100_2, pt100_3, pt100_4, vRef;
	for (int i = 0; i < PT100_INPUT_BUFFER_SIZE; ++i) {
		pt100_0 += pt100_input_buffer[i].Pt100_0;
		pt100_1 += pt100_input_buffer[i].Pt100_1;
		pt100_2 += pt100_input_buffer[i].Pt100_2;
		pt100_3 += pt100_input_buffer[i].Pt100_3;
		pt100_4 += pt100_input_buffer[i].Pt100_4;
		vRef += pt100_input_buffer[i].V_ref;
	}
	pt100_0 = pt100_0/PT100_INPUT_BUFFER_SIZE;
	pt100_1 = pt100_1/PT100_INPUT_BUFFER_SIZE;
	pt100_2 = pt100_2/PT100_INPUT_BUFFER_SIZE;
	pt100_3 = pt100_3/PT100_INPUT_BUFFER_SIZE;
	pt100_4 = pt100_4/PT100_INPUT_BUFFER_SIZE;
	vRef = vRef/PT100_INPUT_BUFFER_SIZE;
	temp+=1;
	//HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&pt100_input_buffer, sizeof(pt100_input_buffer)/2);
}

