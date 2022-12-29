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
	temp++;
}

