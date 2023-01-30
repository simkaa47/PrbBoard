/*
 * keyboard_action.c
 *
 *  Created on: 18 янв. 2023 г.
@ *      Author: Serafim
 */
#include <keyboard_action.h>
#include <string.h>
#include <settings.h>
#include <math.h>

extern Settings_Struct settings;
extern Meas_Data meas_data;

Row *currentParameters = NULL;
char *groupParamName;
uint8_t paramIndex = 0;
uint8_t editMode = 0;
uint8_t currentParamLength;
char *commParametersName = "Comm Parameters";
char *processParametersName = "ProcParameters";
char *errorsName = "Errors";
char *indicationName = "Indication";
uint8_t groupParamNameLength;
uint8_t err_count;
int8_t currentCursorPosition = -1;
char editedValue[20];

Error_Indication_Struct errors_info[10];


char lcdAnswer[4][20];


static void Clear();
static uint8_t OnEnterPressAction();
static uint8_t ShowMainDisplay();
static uint8_t OnDownPressKey();
static uint8_t OnEscPressKey();
static uint8_t OnUpPressKey();
static uint8_t OnF1PressKey();
static uint8_t OnF2PressKey();
static uint8_t OnF3PressKey();
static uint8_t OnF4PressKey();
static void ShowParameter(Row *parameter);
static int Min(int x, int y);
static uint8_t GetAllErrors();
static uint8_t ShowErrors();
static Dictionary *FindDictionaryFromValue(float value, Dictionary *dictionary, uint8_t dict_len);
static void Print(Row *parameter, uint8_t index);

Dictionary baudrates[]={
		{
				.name = "1200",
				.value = 1200
		},
		{
				.name = "2400",
				.value = 2400
		},
		{
				.name = "4800",
				.value = 4800
		},
		{
				.name = "9600",
				.value = 9600
		},
		{
				.name = "19200",
				.value = 19200
		},
		{
				.name = "38400",
				.value = 38400
		},
		{
				.name = "57600",
				.value = 57600
		},
		{
				.name = "115200",
				.value = 115200
		}
};

Dictionary parities[] = {
		{
				.name = "None",
				.value = 0
		},
		{
				.name = "Even",
				.value = 1
		},
		{
				.name = "Odd",
				.value = 2
		}
};

Row Errors[] = {
		{
				.name = "No kanistra"
		},
		{
				.name = "Err kanistra door"
		},
		{
				.name = "Err freq conv"
		},
		{
				.name = "Err sb abort"
		},
		{
				.name = "Err sq left"
		},
		{
				.name = "Err sq right"
		},
		{
				.name = "Err timeout right"
		},
		{
				.name = "Err timeout left"
		},
		{
				.name = "Err need return"
		},
};


Row probotbor_parameters[] = {
		{
				.name = "1.Timeout left, s",
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.prob_toHomeTime),
				.isEnum = 0,
				.name_len = strlen("1.Timeout left, s"),
				.param_len = 3,
				.param_pos = 17,
				.type = ROW_USHORT

		},
		{
				.name = "2.Timeout right, s",
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.prob_toWorkTime),
				.isEnum = 0,
				.name_len = strlen("2.Timeout right, s"),
				.param_len = 3,
				.param_pos = 17,
				.type = ROW_USHORT
		},
		{
				.name = "3.Timer auto, s",
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.automat_timer),
				.isEnum = 0,
				.name_len = strlen("3.Timer auto, s"),
				.param_len = 3,
				.param_pos = 17,
				.type = ROW_USHORT
		}


};

Row commParameters[] = {
		{
				.name = "1.Ethernet address",
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.eth_sett.addr0),
				.isEnum = 0,
				.name_len = strlen("1.Ethernet address"),
				.param_pos = 2,
				.type = ROW_ETH_ADDR
		},
		{
				.name = "2.Network Mask",
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.eth_sett.mask0),
				.isEnum = 0,
				.name_len = strlen("2.Network Mask"),
				.param_len = 15,
				.param_pos = 2,
				.type = ROW_ETH_ADDR
		},
		{
				.name = "3.Gateway",
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.eth_sett.gateway0),
				.isEnum = 0,
				.name_len = strlen("3.Gateway"),
				.param_pos = 2,
				.type = ROW_ETH_ADDR
		},
		{
				.name = "4.RS485-Baudrate",
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.rs_sett.baudrate),
				.isEnum = 1,
				.name_len = strlen("4.RS485-Baudrate"),
				.param_pos = 2,
				.enums = baudrates,
				.enums_len = sizeof(baudrates)/sizeof(Dictionary),
				.type = ROW_UINT
		},
		{
				.name = "5.RS485-Parity",
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.rs_sett.parity),
				.isEnum = 1,
				.name_len = strlen("5.RS485-Parity"),
				.param_pos = 2,
				.enums = parities,
				.enums_len = sizeof(parities)/sizeof(Dictionary),
				.type = ROW_USHORT
		}



};

Row indication_rows[] = {
		{
				.name = "1.DI",
				.isEdited = 0,
				.data = (uint8_t*)(&meas_data.d_inputs),
				.isEnum = 0,
				.name_len = strlen("1.DI"),
				.param_len = 12,
				.param_pos = 8,
				.type = ROW_BINARY_12
		},
		{
				.name = "2.DO",
				.isEdited = 0,
				.data = (uint8_t*)(&meas_data.d_outputs),
				.isEnum = 0,
				.name_len = strlen("2.DO"),
				.param_len = 12,
				.param_pos = 8,
				.type = ROW_BINARY_12
		},
};

static int get_answer(uint8_t len, uint8_t *req, uint8_t *answer);

int OnKeyPress(uint8_t *req,uint8_t req_length, uint8_t *answer)
{
	char *p = (char*)req;
	if(!strncmp(p, "Enter", strlen("Enter")))
	{
		if(!OnEnterPressAction()) return 0;
	}
	else if(!strncmp(p, "Esc", strlen("Esc")))
	{
		if(!OnEscPressKey()) return 0;
	}
	else if(!strncmp(p, "Delete", strlen("Delete")))
	{
		return get_answer(strlen("Delete"),req,answer);
	}
	else if(!strncmp(p, "Up", strlen("Up")))
	{
		if(!OnUpPressKey())return 0;
	}
	else if(!strncmp(p, "Right", strlen("Right")))
	{
		return 0;
	}
	else if(!strncmp(p, "Left", strlen("Left")))
	{
		return 0;
	}
	else if(!strncmp(p, "Down", strlen("Down")))
	{
		if(!OnDownPressKey())return 0;

	}
	else if(!strncmp(p, "F1", strlen("F1")))
	{
		if(!OnF1PressKey())return 0;
	}
	else if(!strncmp(p, "F2", strlen("F2")))
	{
		if(!OnF2PressKey())return 0;
	}
	else if(!strncmp(p, "F3", strlen("F3")))
	{
		if(!OnF3PressKey())return 0;
	}
	else if(!strncmp(p, "F4", strlen("F4")))
	{
		if(!OnF4PressKey())return 0;
	}
	else if(!strncmp(p, "NumPad0", strlen("NumPad0")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad1", strlen("NumPad1")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad2", strlen("NumPad2")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad3", strlen("NumPad3")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad4", strlen("NumPad4")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad5", strlen("NumPad5")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad6", strlen("NumPad6")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad7", strlen("NumPad7")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad8", strlen("NumPad8")))
	{
		return 0;
	}
	else if(!strncmp(p, "NumPad9", strlen("NumPad9")))
	{
		return 0;
	}
	else if(!strncmp(p, "Dec", strlen("Dec")))
	{
		return 0;
	}
	else return 0;
	memcpy(answer,lcdAnswer,80);
	if(currentCursorPosition==-1)
	{
		//answer[80] = 255;
		return -1;

	}
	else
	{
		//answer[80] = currentCursorPosition+40;
	}
	return currentCursorPosition+40;
}

static int get_answer(uint8_t len, uint8_t *req, uint8_t *answer)
{
	memcpy(answer,req,len);
	return len;
}

static uint8_t OnEnterPressAction()
{
	Row *par = NULL;
	if(currentParameters == NULL)
	{
		currentCursorPosition = -1;
		currentParameters = indication_rows;
		paramIndex = 0;
		currentParamLength = sizeof(indication_rows)/sizeof(Row);
		editMode = 0;
		groupParamName = indicationName;
		groupParamNameLength = strlen(indicationName);
		ShowParameter(currentParameters);
		return 1;
	}
	else if(currentParameters!=Errors)// В случае если находимся в параметре
	{
		par = currentParameters+paramIndex;
		if(!editMode)// Если находимся не в режиме редактирования
		{

			if(par->isEdited)
			{
				Clear();
				Print(par,2);
//				editedValue = lcdAnswer[2]+par->param_pos;
//				int hhh = strlen(editedValue);
				if(!par->isEnum)currentCursorPosition = par->param_pos;
				editMode = 1;
				ShowParameter(par);
				return 1;
			}
		}
	}
	return 0;
}



static void Clear()
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 20; ++j) {
			lcdAnswer[i][j] = 0;
		}
	}
}

static uint8_t ShowMainDisplay()
{
	Clear();
	if(meas_data.automat_mode)
	{
		memcpy(lcdAnswer[0],"Auto mode ON",strlen("Auto mode ON"));
		if(meas_data.probotbor_ready)sprintf(lcdAnswer[2],"Next sel. in %d s",meas_data.toNextOtborTime);
	}
	else
	{
		memcpy(lcdAnswer[0],"Auto mode OFF",strlen("Auto mode OFF"));
	}

	if(meas_data.probotbor_ready)
	{
		memcpy(lcdAnswer[1],"Ready for selection",strlen("Ready for selection"));
	}
	else if(meas_data.probotbor_busy)
	{
		memcpy(lcdAnswer[1],"Executing selection",strlen("Executing selection"));
	}
	else if(meas_data.vozvrat_probotbor_busy)
	{
		memcpy(lcdAnswer[1],"Executing return",strlen("Executing return"));
	}
	else if(*((uint16_t*)&meas_data.errors))
	{
		memcpy(lcdAnswer[1],"Errors, press F4",strlen("Errors, press F4"));
	}
	memcpy(lcdAnswer[3]+3,"Indication->Enter",strlen("Indication->Enter"));
	return 1;

}


static uint8_t OnDownPressKey()
{
	if(currentParameters == NULL)return 0; // Если находимся на главном экране, то не реагируем
	if(editMode)return 0; // Если находимся в режиме редактирования, то не реагируем
	if(paramIndex>=currentParamLength-1)return 0;// Если курсор находится внизу, не реагируем
	if(currentParameters == Errors)
	{
		paramIndex++;
		return ShowErrors();
	}
	paramIndex++;
	ShowParameter(currentParameters+paramIndex);
	return 1;
}

static uint8_t OnUpPressKey()
{
	if(currentParameters == NULL)return 0; // Если находимся на главном экране, то не реагируем
	if(editMode)return 0; // Если находимся в режиме редактирования, то не реагируем
	if(paramIndex<=0)return 0;// Если курсор находится ddth[e, не реагируем
	if(currentParameters == Errors)
	{
		paramIndex = paramIndex == 0 ? paramIndex : paramIndex -1;
		return ShowErrors();

	}
	paramIndex--;
	ShowParameter(currentParameters+paramIndex);
	return 1;
}


static uint8_t OnF1PressKey()
{
	currentCursorPosition = -1;
	currentParameters = NULL;
	return ShowMainDisplay();
}

static uint8_t OnF2PressKey()
{
	currentCursorPosition = -1;
	currentParameters = probotbor_parameters;
	paramIndex = 0;
	currentParamLength = sizeof(probotbor_parameters)/sizeof(Row);
	editMode = 0;
	groupParamName = processParametersName;
	groupParamNameLength = strlen(processParametersName);
	ShowParameter(currentParameters);
	return 1;
}

static uint8_t OnF3PressKey()
{
	currentCursorPosition = -1;
	currentParameters = commParameters;
	paramIndex = 0;
	currentParamLength = sizeof(commParameters)/sizeof(Row);
	editMode = 0;
	groupParamName = commParametersName;
	groupParamNameLength = strlen(commParametersName);
	ShowParameter(currentParameters);
	return 1;
}

static uint8_t OnF4PressKey()
{
	currentCursorPosition = -1;
	currentParameters = Errors;
	editMode  = 0;
	paramIndex = 0;
	return ShowErrors();
}

static uint8_t OnEscPressKey()
{
	currentCursorPosition = -1;
	if(currentParameters==NULL)return 0;
	if(currentParameters==Errors)return 0;
	if(currentParameters==indication_rows)
	{
		currentParameters = NULL;
		return ShowMainDisplay();
	}
	else if(editMode)
	{
		editMode = 0;
		ShowParameter(currentParameters+paramIndex);
		return 1;
	}
	return 0;
}

static void ShowParameter(Row *parameter)
{
	Clear();
	memcpy(lcdAnswer[0],groupParamName,groupParamNameLength); // Имя группы параметра
	memcpy(lcdAnswer[1],parameter->name,parameter->name_len); // Имя  параметра
	if(parameter->isEnum)
	{
		float value = 0;
		switch (parameter->type) {
			case ROW_USHORT:
				value = *((uint16_t*)parameter->data);
				break;
			case ROW_UINT:
				value = *((uint32_t*)parameter->data);
				break;
			default:
				break;
		}
		Dictionary *pair = FindDictionaryFromValue(value, parameter->enums, parameter->enums_len);
		if(pair == NULL)
		{
			strncpy(lcdAnswer[2]+parameter->param_pos,"Undefined", strlen("Undefined"));
		}
		else
		{
			strncpy(lcdAnswer[2]+parameter->param_pos,pair->name, strlen(pair->name));
		}
	}
	else
	{
		Print(parameter, 2);
	}

	if(parameter->isEdited)
	{
		if(!editMode)
		{
			strncpy(lcdAnswer[3]+9,"Edit->Enter", strlen("Edit->Enter"));
		}
		else
		{
			strncpy(lcdAnswer[3],"Back->Esc", strlen("Back->Esc"));
			strncpy(lcdAnswer[3]+11,"Ok->Enter", strlen("Ok->Enter"));
		}


	}
	else if(currentParameters == indication_rows)
	{
		strncpy(lcdAnswer[3]+7,"To main-> Esc", strlen("To main-> Esc"));
	}
}

static int Min(int x,int y)
{
	return x<y?x:y;
}

static uint8_t ShowErrors()
{
	GetAllErrors();
	Clear();
	strncpy(lcdAnswer[0],"Errors", strlen("Errors"));
	if(err_count==0)return 1;
	if(paramIndex>=err_count-1)paramIndex = err_count==0 ? 0 : err_count-1;
	for (int i = paramIndex; i < Min(err_count, paramIndex+3); ++i) {
		sprintf(lcdAnswer[1+i-paramIndex],"%d.%s",errors_info[i].num, errors_info[i].errorName);
	}
	return 1;
}

static uint8_t GetAllErrors()
{
	err_count = 0;
	if(meas_data.errors.nakopitel_kanistra_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[0].name;
		err_count++;
	}
	if(meas_data.errors.nakopitel_door_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[1].name;
		err_count++;
	}
	if(meas_data.errors.uz_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[2].name;
		err_count++;
	}
	if(meas_data.errors.sb_abort_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[3].name;
		err_count++;
	}
	if(meas_data.errors.sq_left_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[4].name;
		err_count++;
	}
	if(meas_data.errors.sq_right_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[5].name;
		err_count++;
	}
	if(meas_data.errors.timeout_moving_right_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[6].name;
		err_count++;
	}
	if(meas_data.errors.timeout_moving_left_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[7].name;
		err_count++;
	}
	if(meas_data.errors.need_return_err)
	{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[8].name;
		err_count++;
	}
	return 1;

}

/*Возращает ссылку на пару "ключ-значение", если записи нет, то возвращает NULL */
static Dictionary *FindDictionaryFromValue(float value, Dictionary *dictionary, uint8_t dict_len)
{
	Dictionary *temp = NULL;
	for (int i = 0; i < dict_len; ++i) {
		temp = dictionary+i;
		if(temp->value == value)return temp;

	}
	return NULL;
}



void LcdUpdate()
{

}

static void Print(Row *parameter, uint8_t index)
{
	uint16_t temp = 0;
	switch (parameter->type) {
		case ROW_ETH_ADDR:
			sprintf(lcdAnswer[index]+parameter->param_pos,"%d.%d.%d.%d",*((uint16_t*)parameter->data),*(((uint16_t*)parameter->data)+1),*(((uint16_t*)parameter->data)+2),*(((uint16_t*)parameter->data)+3));
			break;
		case ROW_USHORT:
			sprintf(lcdAnswer[index]+parameter->param_pos,"%d",*((uint16_t*)parameter->data));
			break;
		case ROW_UINT:
			sprintf(lcdAnswer[index]+parameter->param_pos,"%d",*((uint32_t*)parameter->data));
			break;
		case ROW_BINARY_12:
			temp = *(uint16_t*)parameter->data;
			for (int j = 0; j < 12; ++j) {
				uint8_t num = (temp & (1<<j))>0;
				sprintf(lcdAnswer[2]+parameter->param_pos+j,"%d",num);
			}

		default:
			break;
	}
}







