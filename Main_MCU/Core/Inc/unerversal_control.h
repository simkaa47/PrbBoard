/*
 * unerversal_control.h
 *
 *  Created on: Jan 9, 2023
 *      Author: Serafim
 */

#ifndef INC_UNERVERSAL_CONTROL_H_
#define INC_UNERVERSAL_CONTROL_H_

#include <timer.h>

typedef struct
{
	// input var
	unsigned short homePos:1;
	unsigned short workPos:1;
	unsigned short forw_rev:1;
	unsigned short stop :1;
	unsigned short tostart :1;
	unsigned short uzFail :1;
	unsigned short sqStop1 :1;
	unsigned short sqStop2 :1;
	uint8_t type;
	uint16_t setSpeed;
	uint16_t timeToWork;


	// output_var
	unsigned short busy  :1;
	unsigned short rev  :1;
	unsigned short forw  :1;
	unsigned short vozvratBusy  :1;
	uint8_t error;
	uint8_t  statusout;
	uint8_t vozvratStatusOut;
	uint32_t speedout;
	uint16_t timeout;
	uint16_t statusIcon;

	// in_out var
	unsigned short startCycle  :1;
	unsigned short startVozvrat  :1;

	//var
	uint16_t operation;
	uint32_t toHomeTime;
	uint32_t toWorkTime;
	uint16_t status;
	uint16_t addTime;
	uint16_t calibStatus;
	TON timer;
	TON homeErr;
	TON workErr;
	TON sqTimer;
	TON calibTimer;
	TON notHomeTON;
	uint8_t statusOld;
	unsigned short startFlagLast  :1;
	unsigned short startVozvratFlagLast  :1;
	unsigned short calibComplete  :1;
	unsigned short stopFlagLast  :1;
	unsigned short moveDirection  :1;
	unsigned short temp  :1;
	unsigned short sq1StopLast  :1;
	unsigned short sq2StopLast  :1;
	unsigned short sq1StopRt  :1;
	unsigned short sq2StopRt  :1;
	unsigned short initialized  :1;

}Universal_Control_Struct;

typedef enum
{
	UNIVERSAL_CONTROL_INIT_OK,
	UNIVERSAL_CONTROL_INIT_FAIL

}Universal_Control_Init_State;

void universal_control_execute(Universal_Control_Struct *var);
Universal_Control_Init_State universal_control_init(Universal_Control_Struct *var);

#endif /* INC_UNERVERSAL_CONTROL_H_ */
