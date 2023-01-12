/*
 * timer.h
 *
 *  Created on: Jan 9, 2023
 *      Author: Serafim
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <main.h>

typedef struct
{
	uint32_t SV;
	uint32_t ET;
	uint8_t IN;
	uint8_t OUT;
}TON;

typedef enum
{
	TIMER_OK,
	TIMER_FAIL

}Timer_Init_State;

Timer_Init_State Timer_Init(TON *timer);

void Start_Timers(void);
void UpdateTimers(void);
Timer_Init_State Timer_Init(TON *timer);



#endif /* INC_TIMER_H_ */
