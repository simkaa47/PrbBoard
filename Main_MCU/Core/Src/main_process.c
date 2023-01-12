/*
 * main_process.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Serafim
 */
#include <string.h>
#include <dio.h>
#include <analog.h>
#include <timer.h>
#include <probotbornik.h>



void main_process()
{
	Read_Inputs();
	Analog_Read();
	probotbor_process();
	Write_Outputs();

	osDelay(1);
}

void main_process_init()
{

}

