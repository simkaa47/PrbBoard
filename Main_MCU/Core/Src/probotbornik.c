/*
 * probotbornik.c
 *
 *  Created on: 12 янв. 2023 г.
 *      Author: Serafim
 */
#include <unerversal_control.h>
#include <dio.h>
#include <settings.h>



Universal_Control_Struct probotbornik_control;
extern DI d_inputs;
extern DO d_outputs;
extern Settings_Struct settings;
extern Meas_Data meas_data;

void probotbor_process()
{
	probotbornik_control.type = 6; // Продольно пересечной проботборник
	probotbornik_control.homePos = d_inputs.sq_kovsh_prob_left_2;
	probotbornik_control.workPos = d_inputs.sq_kovsh_prob_right_2;
	probotbornik_control.sqStop1 = d_inputs.sq_kovsh_prob_left_1;
	probotbornik_control.sqStop2 = d_inputs.sq_kovsh_prob_right_1;
	probotbornik_control.stop = d_inputs.sb_stop;
	probotbornik_control.uzFail = d_inputs.uz_prob_fail;
	probotbornik_control.tostart = d_inputs.sb_make_proba;
	probotbornik_control.timeToWork = 2000;
	probotbornik_control.toHomeTime = settings.retain.prob_toHomeTime ? settings.retain.prob_toHomeTime : 10000;
	probotbornik_control.toWorkTime = settings.retain.prob_toWorkTime ? settings.retain.prob_toWorkTime : 10000;

	universal_control_execute(&probotbornik_control);

	d_outputs.uz_prob_forv = probotbornik_control.forw;
	d_outputs.uz_prob_rev = probotbornik_control.rev;
	d_outputs.prob_ready = !probotbornik_control.busy;
	meas_data.probotbor_busy = probotbornik_control.busy;
	meas_data.probotbor_error = probotbornik_control.error;
	meas_data.vozvrat_probotbor_busy = probotbornik_control.vozvratBusy;
	meas_data.vozvrat_status = probotbornik_control.vozvratStatusOut;
}


