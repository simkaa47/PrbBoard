/*
 * main_process.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Serafim
 */
#include <string.h>
#include <dio.h>
#include <analog.h>

void main_process()
{
	Read_Inputs();
	Write_Outputs();
	Analog_Read();
}

