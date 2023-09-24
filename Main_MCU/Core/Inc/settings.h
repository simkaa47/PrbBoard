/*
 * settings.h
 *
 *  Created on: 5 дек. 2022 г.
 *      Author: Serafim
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#include <stdint.h>

#define FRAM_SETTINGS_ADDR		0x00
#define FRAM_SETTINGS_HEADER	0xABD8

typedef struct
{
	uint32_t baudrate;
	uint16_t parity;
}RS485_Sett;

typedef struct
{
	uint16_t addr0;
	uint16_t addr1;
	uint16_t addr2;
	uint16_t addr3;
	uint16_t mask0;
	uint16_t mask1;
	uint16_t mask2;
	uint16_t mask3;
	uint16_t gateway0;
	uint16_t gateway1;
	uint16_t gateway2;
	uint16_t gateway3;
}Ethernet_Settings;




typedef struct
{
	unsigned short in0:1;
	unsigned short in1:1;
	unsigned short in2:1;
	unsigned short in3:1;
	unsigned short in4:1;
	unsigned short in5:1;
	unsigned short in6:1;
	unsigned short in7:1;
	unsigned short in8:1;
	unsigned short in9:1;
	unsigned short in10:1;
	unsigned short in11:1;
}Discrete_Inputs;

typedef struct
{
	unsigned short out0:1;
	unsigned short out1:1;
	unsigned short out2:1;
	unsigned short out3:1;
	unsigned short out4:1;
	unsigned short out5:1;
	unsigned short out6:1;
	unsigned short out7:1;
	unsigned short out8:1;
	unsigned short out9:1;
	unsigned short out10:1;
	unsigned short out11:1;
}Discrete_Outputs;

typedef struct
{
	Discrete_Inputs input_emulation_switches;
	Discrete_Inputs input_emulation_values;
	Discrete_Outputs output_emulation_switches;
	Discrete_Outputs output_emulation_values;
	uint16_t mb_addr;
	RS485_Sett rs_sett;
	Ethernet_Settings eth_sett;
	uint16_t prob_toHomeTime;
	uint16_t prob_toWorkTime;
	uint16_t nakop_SV;
	uint16_t remote_mode;
	uint32_t automat_timer;


}Retain;

typedef struct
{
	unsigned int nakopitel_kanistra_err:1;
	unsigned int nakopitel_door_err:1;
	unsigned int uz_err:1;
	unsigned int sb_abort_err:1;
	unsigned int sq_left_err:1;
	unsigned int sq_right_err:1;
	unsigned int timeout_moving_right_err:1;
	unsigned int timeout_moving_left_err:1;
	unsigned int need_return_err:1;
	unsigned int full_nakop_err:1;
	unsigned int no_concentrat_err:1;
	unsigned int conveyer_off_err:1;
	unsigned int remote_stop_err:1;


}Errors_Struct;

typedef struct
{
	unsigned short ready_for_select :1;
	unsigned short automat :1;
	unsigned short errors :1;
	unsigned short prob_err :1;
	unsigned short res_0 :1;
	unsigned short res_1 :1;
	unsigned short timeout_err :1;
	unsigned short sq_kovsh_err :1;
	unsigned short sb_abort_err :1;

}To_Client_Info;


typedef struct
{
	unsigned short auto_on :1;
	unsigned short abort :1;
	unsigned short koncentrat_on :1;
	unsigned short konveyer_on :1;
	unsigned short select_cmd :1;
	float select_period;

}From_Client_Info;

typedef struct
{
	uint16_t start_otbor;
	uint16_t start_vozvrat;
	uint16_t data[50];
}Non_Retain;

typedef struct
{
	From_Client_Info from;
    Retain retain;
    Non_Retain non_retain;

}Settings_Struct;

typedef struct
{
	To_Client_Info to;
	Discrete_Inputs d_inputs;
	Discrete_Outputs d_outputs;
	float analog_input;
	float pt100;
	uint16_t probotbor_ready;
	uint16_t probotbor_busy;
	uint16_t vozvrat_probotbor_busy;
	uint16_t automat_mode;
	uint16_t probInKanistra;
	uint16_t nakopitelFull;
	uint32_t toNextOtborTime;
	Errors_Struct errors;
    uint16_t data[40];
}Meas_Data;

int settingsLoadFRAM (void);
int settingsSaveFRAM (void);

#endif /* INC_SETTINGS_H_ */
