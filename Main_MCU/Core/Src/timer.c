/*
 * timer.c
 *
 *  Created on: Jan 9, 2023
 *      Author: Serafim
 */

#include <timer.h>
#include <string.h>



struct list_item{
	TON *timer;
	struct list_item *next;
};

struct list_item *timers = NULL;

extern TIM_HandleTypeDef htim4;

static Timer_Init_State Push_To_List(TON *timer);
static void UpdateTimer(TON *timer);

void Start_Timers(void)
{
	HAL_TIM_Base_Start_IT(&htim4);

}

Timer_Init_State Timer_Init(TON *timer)
{
	return Push_To_List(timer);

}

static Timer_Init_State Push_To_List(TON *timer)
{
	struct list_item *tmp, *top;
	tmp = malloc(sizeof(struct list_item));
	if(tmp == NULL)return TIMER_FAIL;
	tmp->next = NULL;
	tmp->timer = timer;
	if(timers)
	{
		top = timers;
		while(top->next)
		{
			top = top->next;
		}
		top->next = tmp;
	}
	else
	{
		timers = tmp;
	}
	return TIMER_OK;
}

/*Функция обновления значений всех таймеров*/
void UpdateTimers()
{
	struct list_item *tmp = timers;
	while(tmp)
	{
		UpdateTimer(tmp->timer);
		tmp = tmp->next;
	}
}

/*Функция обновления значений одного таймера*/
static void UpdateTimer(TON *timer)
{
	if(timer->IN)
	{
		timer->ET = timer->ET < timer->SV ? timer->ET+1 : timer->ET;
	}
	else
	{
		timer->ET = 0;
	}
	timer->OUT = timer->ET == timer->SV;
}




