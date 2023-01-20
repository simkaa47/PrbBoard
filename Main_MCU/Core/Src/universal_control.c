/*
 * universal_control.c
 *
 *  Created on: Jan 9, 2023
 *      Author: Serafim
 */
#include <unerversal_control.h>
#include <timer.h>
#include <vozvratkovshaFB.h>

Universal_Control_Init_State universal_control_init(Universal_Control_Struct *var)
{
	var->addTime = 100;
	if(var->toHomeTime == 0)var->toHomeTime = 10000;
	if(var->toWorkTime == 0)var->toWorkTime = 10000;
	if(Timer_Init(&(var->timer))==TIMER_FAIL)return UNIVERSAL_CONTROL_INIT_FAIL;
	if(Timer_Init(&(var->homeErr))==TIMER_FAIL)return UNIVERSAL_CONTROL_INIT_FAIL;
	if(Timer_Init(&(var->workErr))==TIMER_FAIL)return UNIVERSAL_CONTROL_INIT_FAIL;
	if(Timer_Init(&(var->sqTimer))==TIMER_FAIL)return UNIVERSAL_CONTROL_INIT_FAIL;
	if(Timer_Init(&(var->calibTimer))==TIMER_FAIL)return UNIVERSAL_CONTROL_INIT_FAIL;
	if(Timer_Init(&(var->notHomeTON))==TIMER_FAIL)return UNIVERSAL_CONTROL_INIT_FAIL;
	return UNIVERSAL_CONTROL_INIT_OK;

}

void universal_control_execute(Universal_Control_Struct *var)
{
	// 1   Бункер с электрическим шибером,  Бункер с пневматическим шибером
	// 2   Продольно пересечной проботборник,  Скиповый мехнизм
	// 3   маятниковый проботборник,    Вторичный маятниковый пробоотборник
	// 4   Роторный делитель
	// 5   Ленточный дозатор,   Щековая дробилка,    Валковая дробилка,   Конусная дробилка,   Конвейер,    Шнековый питатель
	// 6   Пересечной проботборник,   вторичный пересечной пробоотборник

	// тип шаг1    шаг2                шаг3              шаг4                 шаг5                шаг6
	// 1   вперед  ждать датчик work   ждать             назад                ждать датчик home   стоп
	// 2   вперед  ждать датчик work   назад             ждать датчик  home   ждать               стоп
	// 3   вперед  ждать датчик home                                                              стоп
	// 4   вперед  ждать               ждать датчик home                                          стоп
	// 5   вперед  ждать                                                                          стоп
	// 6   вперед/назад                ждать датчик work/home                                     стоп
	//тип 0 идет по умолчанию
	//
	Vozvrat_Kovsha_Struct toHome;
	if(!var->initialized)
	{
		Universal_Control_Init_State result = universal_control_init(var);
		if(result==UNIVERSAL_CONTROL_INIT_OK)var->initialized = 1;
		else return;
	}
	if (var->startCycle && var->tostart) {
		var->busy = 1;
		var->operation = 1;
		if (var->type==0) {
			var->status = 6;
		} else {
			var->status = 1;
		}
		var->startCycle = 0;
	}
	// timer
	var->timer.SV = var->timeToWork;
	var->timer.IN = var->status ==3;
	var->timeout = var->timer.ET/1000;

	// sq timer
	var->sqTimer.IN = (var->status == 5 || var->status ==2);
	var->sqTimer.SV = 3000;
	if (var->sqTimer.OUT) {
		var->error = 5;
	}

	// not_home_timer
	var->notHomeTON.IN = !(var->homePos);
	var->notHomeTON.SV = 1000;

	// positive triggers
	var->sq1StopRt = var->sq1StopLast==0 && var->sqStop1>0 ? 1:0;
	var->sq1StopLast = var->sqStop1;

	var->sq2StopRt = var->sq2StopLast==0 && var->sqStop2>0 ? 1:0;
	var->sq2StopLast = var->sqStop2;


	if (var->stop || var->error==21 || var->error==51 || var->error==5 || var->error==2) {
		var->operation = 1;
		var->status = 6;
		var->error = 6;
	}

	if (var->sq1StopRt || var->sq2StopRt) {
		var->forw = 0;
		var->rev = 0;
	}

	if(var->startVozvrat)
	{
		var->busy = 0;
		var->status = 6;
		var->operation = 2;
		var->startVozvrat = 0;
	}

	// xz, zachem
	var->stopFlagLast = var->startFlagLast;
	var->statusout = var->status;

	if(var->calibComplete)
	{
		//homeErr timer
		var->homeErr.IN = (var->status==51
				|| var->status==5
				|| (var->status==3 && var->homePos)
				|| (var->operation==2 && var->homePos && var->vozvratStatusOut==1));
		var->homeErr.SV = var->toHomeTime;
		if(var->homeErr.OUT)
		{
			var->error = 5;
		}
		if(var->type==1 || var->type==2 || var->type==6 )
		{
			var->workErr.IN = (var->status==21 || var->status==2);
			var->workErr.SV = var->toWorkTime;
		}
		else
		{
			var->workErr.IN = (var->status==21);
			var->workErr.SV = var->toWorkTime;
		}

		if (var->status==21 || var->status==2) {
			var->speedout = var->workErr.ET*180/(var->toHomeTime-var->addTime);
		} else if(var->status==3) {
			var->speedout = 180;
		}
		else if(var->status==51){
			if(var->type==3 || var->type==4){
				var->speedout = 360 - var->homeErr.ET*360/(var->toHomeTime-var->addTime);
			}
			else{
				var->speedout = 180 - var->homeErr.ET*180/(var->toHomeTime-var->addTime);
			}

		}
		else if(var->status==6){
			var->speedout = 0;
		}
		if(var->workErr.OUT)
		{
			var->error = 2;
		}

	}
	switch (var->operation) {
		case 1://цикл движения
			switch (var->status) {
				case 1://вперед
					if (var->type==6)
					{
						if (var->homePos)
						{
							var->forw = 0;
							var->rev = 1;
							var->moveDirection = 0;
							var->status = 2;
						}
						else if(var->workPos)
						{
							var->forw = 1;
							var->rev = 0;
							var->moveDirection = 1;
							var->status = 5;
						}
						else if(var->moveDirection)
						{
							var->forw = 1;
							var->rev = 0;
						}
						else
						{
							var->forw = 0;
							var->rev = 1;
						}
					}
					else
					{
						if(var->forw_rev)
						{
							var->rev = 0;
							var->forw = 1;
							switch (var->type) {
								case 5:
									var->status = 5;
									break;
								case 3:
								case 4:
									var->status=2;
									break;
								default:
									var->status = 2;
									break;
							}
						}
						else
						{
							var->rev = 1;
							var->forw = 0;
							switch (var->type) {
								case 5:
									var->status = 3;
									break;
								case 3:
								case 4:
									var->status=2;
									break;
								default:
									var->status = 2;
									break;
							}
						}
					}
					break;
				case 2:	//ждать датчик рабочего положения
					if(var->notHomeTON.OUT)
					{
						switch (var->type) {
							case 3:
								var->status = 51;
								break;
							case 4:
								var->status = 3;
								break;
							default:
								var->status = 21;
								break;
						}
					}
					break;
				case 21:
					if(var->workPos)
					{
						var->forw = 0;
						switch (var->type) {
							case 2:
								var->status =4;
								break;
							case 6:
								var->status =6;
								break;
							default:
								var->status =3;
								break;
						}
					}
					if(var->homePos && !(var->type==4 || var->type==3))
					{
						var->error = 21;
						var->status = 6;
					}
					break;
				case 3://delay
					if(var->timer.OUT)
					{
						switch (var->type) {
							case 4:
								var->status = 5;
								break;
							case 2:
							case 5:
								var->status = 6;
								break;
							default:
								var->status = 4;
								break;
						}
					}
					break;
				case 4://назад
					if(var->forw_rev)
					{
						var->forw = 0;
						var->rev = 1;
						var->status = 5;
					}
					else
					{
						var->forw = 1;
						var->rev = 0;
						var->status = 5;
					}
					break;
				case 5: // Ждать датчик домашнего положения
					if(!var->workPos)
					{
						var->status = 51;
					}
					break;
				case 51:
					if(var->homePos)
					{
						switch (var->type) {
							case 2:
								var->status = 3;
								break;
							default:
								var->status = 6;
								break;
						}
					}
					if(var->workPos && !(var->type==4 || var->type==3))
					{
						var->error = 51;
						var->status = 6;
					}
					break;
				case 6:
					var->forw = 0;
					var->rev = 0;
					var->busy = 0;
					var->vozvratStatusOut = 0;
					var->status=0;
					break;
				case 0:
					if (var->type==6) {
						if (var->homePos || var->workPos) {
							var->error=0;
						} else {
							var->error = 6;
						}
					} else if((!(var->homePos) || var->workPos) && !(var->type==5)){
						var->error = 6;
					}
					else{
						var->error = 0;
					}
					break;
				default:
					break;
			}
			break;
		case 2://Возврат ковша
			toHome.sqStop1 = var->sqStop1;
			toHome.sqStop2 = var->sqStop2;
			toHome.moveDirection = var->moveDirection;
			toHome.startflag = var->startVozvrat;
			toHome.type = var->type;
			toHome.homePos = var->homePos;
			toHome.workPos = var->workPos;
			toHome.forw_rev = var->forw_rev;
			toHome.setSpeed = var->setSpeed;
			Vozvrat_Kovsha(&toHome);
			var->vozvratBusy = toHome.busy;
			var->vozvratStatusOut = toHome.statusout;
			var->rev = toHome.rev;
			var->forw = toHome.forw;


			if(var->homePos)
			{
				var->operation = 1;
				var->status = 6;
				var->error = 0;
			}
			if(var->vozvratBusy)
			{
				var->error = 0;
			}
			break;
		default:
			break;
	}
	//Таймер калибровки
	var->calibTimer.SV = 100000;
	if(var->type==4)
	{
		var->calibTimer.IN = !var->calibComplete && (var->forw || var->rev) && !var->homePos;
	}
	else
	{
		var->calibTimer.IN = !var->calibComplete && (var->forw || var->rev);
	}

	// калибровка
	if(!var->calibComplete)
	{
		if(var->status==2 || var->status==3)
		{
			var->calibStatus = 1;
		}
		switch (var->calibStatus) {
			case 1:
				if(var->type==4)
				{
					if((var->forw || var->rev) && !var->homePos)
					{
						var->toHomeTime = var->calibTimer.ET;
					}
				}
				else
				{
					if(var->forw)
					{
						var->toWorkTime = var->calibTimer.ET;
					}
					if(var->rev)
					{
						var->toHomeTime = var->calibTimer.ET;
					}
				}
				if(var->type==6)
				{
					if(var->status==0 && var->moveDirection)
					{
						var->calibStatus = 2;
					}
				}
				else
				{
					if(var->status==0)
					{
						var->calibStatus = 2;
					}
				}
				break;
			case 2:
				if(var->type==4)
				{
					var->toHomeTime+=var->addTime;
				}
				else
				{
					var->toWorkTime+=var->addTime;
					var->toHomeTime+=var->addTime;
				}
				var->calibComplete = 1;
				var->calibStatus = 0;
				break;
			default:
				break;
		}
	}

}
