/*
 * vozvratkovshaFB.h
 *
 *  Created on: 10 янв. 2023 г.
 *      Author: Serafim
 */

#ifndef INC_VOZVRATKOVSHAFB_H_
#define INC_VOZVRATKOVSHAFB_H_

#include <main.h>

typedef struct
{
	// input var
	unsigned short homePos:1;
	unsigned short workPos:1;
	unsigned short forw_rev:1;
	unsigned short sqStop1 :1;
	unsigned short sqStop2 :1;
	uint8_t type;
	uint16_t setSpeed;


	// output_var
	unsigned short busy  :1;
	unsigned short rev  :1;
	unsigned short forw  :1;
	uint8_t error;
	uint8_t  statusout;
	uint16_t speedout;


	// var_in_retain
	unsigned short moveDirection  :1;
	unsigned short startflag   :1;

	//var
	uint16_t status;
	unsigned short startFlagLast :1;


}Vozvrat_Kovsha_Struct;

void Vozvrat_Kovsha(Vozvrat_Kovsha_Struct *var);

#endif /* INC_VOZVRATKOVSHAFB_H_ */
