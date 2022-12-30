/*
 * analog.c
 *
 *  Created on: Dec 29, 2022
 *      Author: Serafim
 */
#include <analog.h>
#include <settings.h>

Pt100_Struct pt100_input_buffer[PT100_INPUT_BUFFER_SIZE];
uint8_t pt100_data_ready;
uint32_t temp;



extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim3;
extern Meas_Data meas_data;

static void Calc_Pt100();

void Analog_Start(void)
{

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&pt100_input_buffer, sizeof(pt100_input_buffer)/2);
	HAL_TIM_Base_Start(&htim3);

}

void Analog_Read(void)
{
	Calc_Pt100();
}

static void Calc_Pt100()
{
	if(pt100_data_ready)
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
		vRef = vRef/PT100_INPUT_BUFFER_SIZE;
		vRef = 3.3f*(float)V_REF_VALUE/vRef;
		pt100_0 = pt100_0/PT100_INPUT_BUFFER_SIZE * 3.3f/vRef;
		pt100_1 = pt100_1/PT100_INPUT_BUFFER_SIZE * 3.3f/vRef;
		pt100_2 = pt100_2/PT100_INPUT_BUFFER_SIZE * 3.3f/vRef;
		pt100_3 = pt100_3/PT100_INPUT_BUFFER_SIZE * 3.3f/vRef;
		pt100_4 = pt100_4/PT100_INPUT_BUFFER_SIZE * 3.3f/vRef;

		float vpt100 = pt100_3 - pt100_1;
		float ref = pt100_1-pt100_0;
		float v_result = vpt100*100/ref;
		meas_data.pt100 = (v_result - 100)*2.5974f;
		pt100_data_ready = 0;
		temp++;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	pt100_data_ready = 1;
}

