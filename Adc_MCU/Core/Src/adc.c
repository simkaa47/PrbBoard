/*
 * adc.c
 *
 *  Created on: Dec 23, 2022
 *      Author: ivan.liutov
 */
#include <adc.h>
#include <uart.h>

extern ADC_HandleTypeDef hadc;
extern TIM_HandleTypeDef htim3;
uint16_t input_buffer[ADC_BUFFER_SIZE];
float adc_value;
uint32_t tick = 0;
float voltage;




void Start_Adc(void)
{
	HAL_GPIO_WritePin(ADC_LED_GPIO_Port, ADC_LED_Pin, SET);
	HAL_ADCEx_Calibration_Start(&hadc);// калибровка АЦП
	HAL_TIM_Base_Start(&htim3); // старт таймера АЦП
	HAL_ADC_Start_DMA(&hadc, (uint32_t*)input_buffer, ADC_BUFFER_SIZE);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
	tick+=1;
	uint32_t sum = 0;
	for (int i = 0; i < ADC_BUFFER_SIZE; ++i) {
		sum+=input_buffer[i];
	}
	adc_value = ((float)sum)/ADC_BUFFER_SIZE;
	voltage  = 3 * adc_value/4095;
	Send_Adc(adc_value);
}
