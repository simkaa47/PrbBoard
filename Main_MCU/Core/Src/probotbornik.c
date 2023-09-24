/*
 * probotbornik.c
 *
 *  Created on: 12 янв. 2023 г.
 *      Author: Serafim
 */
#include <unerversal_control.h>
#include <dio.h>
#include <settings.h>
#include <timer.h>

extern DI d_inputs;
extern DO d_outputs;
extern Settings_Struct settings;
extern Meas_Data meas_data;
uint8_t initialized;
uint8_t probotbornik_error;
uint8_t cycle_probotbor;
uint8_t cycle_vozvrat;
uint8_t startOtborCommand;
uint8_t targetSq; // Цtль движения // 0 = left, 1 - right
uint8_t lastSq_kovsh_left_1;
uint8_t lastSq_kovsh_right_1;
uint8_t rtSq_kovsh_left_1;
uint8_t rtSq_kovsh_right_1;
uint8_t lastSbMakeProba;
uint8_t rtSbMakeProba;
uint8_t lastSbMakeVozvrat;
uint8_t rtSbMakeVozvrat;




uint8_t auto_mode;
uint32_t select_period;


TON sqHomeTON;
TON sqWorkTON;
TON automatTON;


static uint8_t Init_timers();
static void SetTimers();
static void GetErrors();
static void Moving();
static uint8_t CheckCommonError();
static uint8_t CheckCriticalError();
static void OnCommandCycle();
static void StopCycles();
static void Indication();
static void GetPositiveFronts();
static void NakopitelCheck();
static void GetClientInfo();
static void LocalRemoteControl();

void probotbor_process()
{
	LocalRemoteControl();
	SetTimers();
	if(!initialized)
	{
		if(!Init_timers())return;
		initialized = 1;
		return;
	}
	NakopitelCheck();
	GetPositiveFronts();
	GetErrors();
	OnCommandCycle();
	Moving();
	if(automatTON.OUT && d_outputs.prob_ready)
	{
		startOtborCommand = 1;
	}
	Indication();
	GetClientInfo();

}

static void GetErrors()
{
	// Inputs
	meas_data.errors.nakopitel_door_err = !d_inputs.sq_kanistra_door;
	meas_data.errors.nakopitel_kanistra_err = !d_inputs.sq_kanistra;
	meas_data.errors.uz_err = !d_inputs.uz_prob_fail;
	meas_data.errors.sb_abort_err = d_inputs.sb_stop;
	meas_data.errors.sq_left_err = d_inputs.sq_kovsh_prob_left_1;
	meas_data.errors.sq_right_err = d_inputs.sq_kovsh_prob_right_1;
	meas_data.errors.full_nakop_err = meas_data.nakopitelFull;


	//Timeouts
	if(sqHomeTON.OUT){
		meas_data.errors.timeout_moving_left_err = 1;
	}
	if(sqWorkTON.OUT){
		meas_data.errors.timeout_moving_right_err = 1;
	}
	if(rtSbMakeProba|| rtSbMakeVozvrat)
	{
		meas_data.errors.timeout_moving_left_err = 0;
		meas_data.errors.timeout_moving_right_err = 0;
	}
	// Need Return
	meas_data.errors.need_return_err = (!d_outputs.uz_prob_forv && !d_outputs.uz_prob_rev) &&
			(!d_inputs.sq_kovsh_prob_left_2 && !d_inputs.sq_kovsh_prob_right_2);

	// Remote stops
	if(settings.retain.remote_mode)
	{
		meas_data.errors.conveyer_off_err = !settings.from.konveyer_on;
		meas_data.errors.no_concentrat_err = !settings.from.koncentrat_on;
	}
	else
	{
		meas_data.errors.conveyer_off_err = 0;
		meas_data.errors.no_concentrat_err = 0;

	}

}

static uint8_t Init_timers()
{
	if(Timer_Init(&(sqHomeTON))==TIMER_FAIL)return 0;
	if(Timer_Init(&(sqWorkTON))==TIMER_FAIL)return 0;
	if(Timer_Init(&(automatTON))==TIMER_FAIL)return 0;
	if(settings.retain.prob_toHomeTime==0)settings.retain.prob_toHomeTime = 10;
	if(settings.retain.prob_toWorkTime==0)settings.retain.prob_toWorkTime = 10;
	if(settings.retain.automat_timer==0)settings.retain.automat_timer = 60;
	SetTimers();
	return 1;
}

static void SetTimers()
{
	// Тайм-аут движения влево
	sqHomeTON.IN = !d_inputs.sq_kovsh_prob_left_2 && d_outputs.uz_prob_rev;
	sqHomeTON.SV = settings.retain.prob_toHomeTime * 1000;

	// Тайм-аут движения вправо
	sqWorkTON.IN = !d_inputs.sq_kovsh_prob_right_2 && d_outputs.uz_prob_forv;
	sqWorkTON.SV = settings.retain.prob_toWorkTime * 1000;

	// Таймер автоматической работы
	automatTON.IN = auto_mode && !cycle_probotbor && !meas_data.nakopitelFull;
	automatTON.SV = select_period * 1000;

	// Осталось до следующей пробы
}

static void LocalRemoteControl()
{
	auto_mode = settings.retain.remote_mode ? settings.from.auto_on : d_inputs.sb_auto_local;
	select_period = settings.retain.remote_mode ? (uint32_t)settings.from.select_period : settings.retain.automat_timer;
	// действия по нажатию кнопки
	if((d_inputs.sb_make_proba && !settings.retain.remote_mode) ||
				(settings.from.select_cmd && settings.retain.remote_mode))
	{
		if(!auto_mode)
		{
			if(settings.retain.remote_mode)settings.from.select_cmd = 0;
						startOtborCommand = 1;
		}
	}

}


static void Moving()
{
	if(!CheckCriticalError())
	{
		cycle_probotbor = 0;
	}
	// Условия остановки движения
	if(cycle_probotbor || cycle_vozvrat)
	{
		// остановка по концевикам b
		if((targetSq && (d_inputs.sq_kovsh_prob_right_1 || d_inputs.sq_kovsh_prob_right_2))||
				(!targetSq && (d_inputs.sq_kovsh_prob_left_1 || d_inputs.sq_kovsh_prob_left_2))
				|| rtSq_kovsh_left_1 || rtSq_kovsh_right_1)
		{
			if(cycle_probotbor)
			{
				meas_data.probInKanistra++;
			}
			StopCycles();

		}
		// остановка по критической ошибке
		if(!CheckCriticalError()){
			StopCycles();
		}
	}


	// Управление двигателем
	if(cycle_probotbor || cycle_vozvrat)
	{
		d_outputs.prob_ready=0;
		if(targetSq)
		{
			d_outputs.uz_prob_forv = 1;
			d_outputs.uz_prob_rev = 0;
		}
		else
		{
			d_outputs.uz_prob_forv = 0;
			d_outputs.uz_prob_rev = 1;
		}
		d_outputs.prob_ready = 0;
	}
	else
	{
		d_outputs.uz_prob_forv =0 ;
		d_outputs.uz_prob_rev = 0 ;
		d_outputs.prob_ready = CheckCommonError();
	}
}

static uint8_t CheckCommonError()
{
	uint16_t *errPointer = (uint16_t*)&(meas_data.errors);
	uint16_t result = *errPointer ? 0 : 1;
	return result;

}

static uint8_t CheckCriticalError()
{
	// Ошибки, при которых нужно срочно остановить ковш
	uint8_t error =  meas_data.errors.sb_abort_err ||
			meas_data.errors.uz_err ||
			(meas_data.errors.sq_left_err && d_outputs.uz_prob_rev)||
			(meas_data.errors.sq_right_err && d_outputs.uz_prob_forv)||
			(meas_data.errors.timeout_moving_left_err)||
			(meas_data.errors.timeout_moving_right_err);
	return !error;
}

static void StopCycles()
{
	cycle_probotbor = 0;
	cycle_vozvrat = 0;
}

static void OnCommandCycle()
{
	if(meas_data.errors.need_return_err &&
			CheckCriticalError() &&
			!cycle_vozvrat &&
			(d_inputs.sb_vozvrat || settings.non_retain.start_vozvrat))
	{
		cycle_probotbor = 0;
		cycle_vozvrat = 1;
		if(d_inputs.sq_kovsh_prob_left_1 || d_inputs.sq_kovsh_prob_left_2 ){
			targetSq = 1;
		}
		else if(d_inputs.sq_kovsh_prob_right_1 || d_inputs.sq_kovsh_prob_right_2){
			targetSq = 0;
		}
		else{
			targetSq = !targetSq;
		};
	}
	settings.non_retain.start_vozvrat = 0;

	if(startOtborCommand)
	{
		startOtborCommand = 0;
		if(d_outputs.prob_ready)
		{
			cycle_probotbor = 1;
			cycle_vozvrat = 0;

			if(d_inputs.sq_kovsh_prob_left_2 || d_inputs.sq_kovsh_prob_left_1)
			{
				targetSq = 1;
			}
			else if(d_inputs.sq_kovsh_prob_right_2 || d_inputs.sq_kovsh_prob_right_1)
			{
				targetSq = 0;
			}
		}
	}
}

static void Indication()
{
	meas_data.probotbor_ready = d_outputs.prob_ready;
	meas_data.probotbor_busy = cycle_probotbor;
	meas_data.vozvrat_probotbor_busy = cycle_vozvrat;
	meas_data.automat_mode = auto_mode;
	meas_data.toNextOtborTime = select_period - automatTON.ET/1000;
}

static void GetPositiveFronts()
{
	rtSq_kovsh_left_1 = d_inputs.sq_kovsh_prob_left_1 && !lastSq_kovsh_left_1;
	rtSq_kovsh_right_1 = d_inputs.sq_kovsh_prob_right_1 && !lastSq_kovsh_right_1;
	rtSbMakeProba =  (d_inputs.sb_make_proba || settings.non_retain.start_otbor) && !lastSbMakeProba;
	rtSbMakeVozvrat = (d_inputs.sb_vozvrat || settings.non_retain.start_vozvrat) && !lastSbMakeVozvrat;
	lastSq_kovsh_left_1 = d_inputs.sq_kovsh_prob_left_1;
	lastSq_kovsh_right_1 = d_inputs.sq_kovsh_prob_right_1;
	lastSbMakeProba = d_inputs.sb_make_proba || settings.non_retain.start_otbor;
	lastSbMakeVozvrat = d_inputs.sb_vozvrat || settings.non_retain.start_vozvrat;

}

static void NakopitelCheck()
{
	if(settings.retain.nakop_SV==0)settings.retain.nakop_SV = 1;
	if(!d_inputs.sq_kanistra)meas_data.probInKanistra = 0;
	meas_data.nakopitelFull = meas_data.probInKanistra>=settings.retain.nakop_SV;

}

static void GetClientInfo()
{
	meas_data.to.ready_for_select = d_outputs.prob_ready;
	meas_data.to.automat = auto_mode;
	meas_data.to.errors = !CheckCommonError();
	meas_data.to.prob_err = meas_data.errors.need_return_err ||
			meas_data.errors.sq_left_err || meas_data.errors.sq_right_err || meas_data.errors.sb_abort_err
			|| meas_data.errors.uz_err || meas_data.errors.timeout_moving_left_err || meas_data.errors.timeout_moving_right_err
			|| meas_data.errors.need_return_err;
	meas_data.to.timeout_err = meas_data.errors.timeout_moving_left_err || meas_data.errors.timeout_moving_right_err;
	meas_data.to.sq_kovsh_err = meas_data.errors.need_return_err;
	meas_data.to.sb_abort_err = meas_data.errors.sb_abort_err;
}








