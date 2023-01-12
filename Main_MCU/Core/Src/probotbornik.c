/*
 * probotbornik.c
 *
 *  Created on: 12 янв. 2023 г.
 *      Author: Serafim
 */
#include <unerversal_control.h>



Universal_Control_Struct probotbornik_control;

void probotbor_process()
{
	universal_control_execute(&probotbornik_control);
}


