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
char commParametersName[] = {0xCF, 0xE0, 0xF0, 0xE0, 0xEC, 0xE5, 0xF2, 0xF0, 0xFB, 0x20, 0xF1, 0xE2, 0xFF, 0xE7, 0xE8,0x00};
char processParametersName[] = {0xCF, 0xE0, 0xF0, 0xE0, 0xEC, 0xE5, 0xF2, 0xF0, 0xFB, 0x20, 0xEE, 0xF2, 0xE1, 0xEE, 0xF0, 0xE0,0x00};
char errorsName[] = {0xCE, 0xF8, 0xE8, 0xE1, 0xEA, 0xE8,0x00};
char indicationName[] = {0xCC, 0xEE, 0xED, 0xE8, 0xF2, 0xEE, 0xF0, 0xE8, 0xED, 0xE3,0x00};

// Ошибки
//Нет канистры
const char errKanistra[] = {0xCD, 0xE5, 0xF2, 0x20, 0xEA, 0xE0, 0xED, 0xE8, 0xF1, 0xF2, 0xF0, 0xFB, 0x00};
//Дверь канистры
const char errDoorKanistra[] = {0xC4, 0xE2, 0xE5, 0xF0, 0xFC, 0x20, 0xEA, 0xE0, 0xED, 0xE8, 0xF1, 0xF2, 0xF0, 0xFB,0x00};
//Ошибка ПЧ
const char errUZ[] = {0xCE, 0xF8, 0xE8, 0xE1, 0xEA, 0xE0, 0x20, 0xCF, 0xD7, 0x00};
//Аварийный стоп
const char abortErr[] = {0xC0, 0xE2, 0xE0, 0xF0, 0xE8, 0xE9, 0xED, 0xFB, 0xE9, 0x20, 0xF1, 0xF2, 0xEE, 0xEF, 0x00};
//Ав. конц. слева
const char sqLeft[] = {0xC0, 0xE2, 0x2E, 0x20, 0xEA, 0xEE, 0xED, 0xF6, 0x2E, 0x20, 0xF1, 0xEB, 0xE5, 0xE2, 0xE0, 0x00};
//Ав. конц. справа
const char sqRight[] = {0xC0, 0xE2, 0x2E, 0x20, 0xEA, 0xEE, 0xED, 0xF6, 0x2E, 0x20, 0xF1, 0xEF, 0xF0, 0xE0, 0xE2, 0xE0, 0x00};
//Тайм-аут вправо
const char timeoutRight[] = {0xD2, 0xE0, 0xE9, 0xEC, 0x2D, 0xE0, 0xF3, 0xF2, 0x20, 0xE2, 0xEF, 0xF0, 0xE0, 0xE2, 0xEE, 0x00};
//Тайм-аут влево
const char timeoutLeft[] = {0xD2, 0xE0, 0xE9, 0xEC, 0x2D, 0xE0, 0xF3, 0xF2, 0x20, 0xE2, 0xEB, 0xE5, 0xE2, 0xEE, 0x00};
//Необходим возврат
const char needReturn[] = {0xCD, 0xE5, 0xEE, 0xE1, 0xF5, 0xEE, 0xE4, 0xE8, 0xEC, 0x20, 0xE2, 0xEE, 0xE7, 0xE2, 0xF0, 0xE0, 0xF2, 0x00};
//Накопитель полный
const char fullNakop[] = {0xCD, 0xE0, 0xEA, 0xEE, 0xEF, 0xE8, 0xF2, 0xE5, 0xEB, 0xFC, 0x20, 0xEF, 0xEE, 0xEB, 0xED, 0xFB, 0xE9,  0x00};

// Паратметры проботборника
const char parTimeoutLeft1[] = {0x31, 0x2E, 0xD2, 0xE0, 0xE9, 0xEC, 0xE0, 0xF3, 0xF2, 0x20, 0xE2, 0xEB, 0xE5, 0xE2, 0xEE, 0x2C, 0x20, 0x63, 0x00};
const char parTimeoutRight2[] = {0x32, 0x2E, 0xD2, 0xE0, 0xE9, 0xEC, 0xE0, 0xF3, 0xF2, 0x20, 0xE2, 0xEF, 0xF0, 0xE0, 0xE2, 0xEE, 0x2C, 0x20, 0x63, 0x00};
const char parAutoSel3[] = {0x33, 0x2E, 0xCF, 0xE5, 0xF0, 0xE8, 0xEE, 0xE4, 0x20, 0xEE, 0xF2, 0xE1, 0xEE, 0xF0, 0xE0, 0x2C, 0x20, 0x63, 0x00};
//4.Емк. канистры, проб
const char nakopSV4[] = {0x34, 0x2E, 0x56, 0x20, 0xEA, 0xE0, 0xED, 0xE8, 0xF1, 0xF2, 0xF0, 0xFB, 0x2C, 0x20, 0xEF, 0xF0, 0xEE, 0xE1,  0x00};
//6.Проб в канистре
const char parNakopCV[] = {0x36, 0x2E, 0xCF, 0xF0, 0xEE, 0xE1, 0x20, 0xE2, 0x20, 0xEA, 0xE0, 0xED, 0xE8, 0xF1, 0xF2, 0xF0, 0xE5,  0x00};



// Параметры связи
// 1.IP-адрес
const char parEth1[] = {0x31, 0x2E, 0x49, 0x50, 0x2D, 0xE0, 0xE4, 0xF0, 0xE5, 0xF1,  0x00};
// 2.Маска подсети
const char parMask2[] = {0x32, 0x2E, 0xCC, 0xE0, 0xF1, 0xEA, 0xE0, 0x20, 0xEF, 0xEE, 0xE4, 0xF1, 0xE5, 0xF2, 0xE8,  0x00};
// 3.Основной шлюз
const char parGateway[] = {0x33, 0x2E, 0xCE, 0xF1, 0xED, 0xEE, 0xE2, 0xED, 0xEE, 0xE9, 0x20, 0xF8, 0xEB, 0xFE, 0xE7,  0x00};
// 4.RS485-Скорость
const char parBaudrate[] = {0x34, 0x2E, 0x52, 0x53, 0x34, 0x38, 0x35, 0x2D, 0xD1, 0xEA, 0xEE, 0xF0, 0xEE, 0xF1, 0xF2, 0xFC,  0x00};
// 5.RS485-Паритет
const char parParity[] = {0x35, 0x2E, 0x52, 0x53, 0x34, 0x38, 0x35, 0x2D, 0xCF, 0xE0, 0xF0, 0xE8, 0xF2, 0xE5, 0xF2,  0x00};
// 6.Modbus адрес
const char parModbAddr[] = {0x36, 0x2E, 0x4D, 0x6F, 0x64, 0x62, 0x75, 0x73, 0x20, 0xE0, 0xE4, 0xF0, 0xE5, 0xF1,  0x00};

uint8_t groupParamNameLength;
uint8_t err_count;
int8_t currentCursorPosition = -1;
float currentEnumKey = 0;
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
static uint8_t OnLeftPressKey();
static uint8_t OnRightPressKey();
static void ShowParameter(Row *parameter);
static int Min(int x, int y);
static uint8_t GetAllErrors();
static uint8_t ShowErrors();
static Dictionary *FindDictionaryFromValue(float value, Dictionary *dictionary, uint8_t dict_len);
static int FindIndexFromValue(float value, Dictionary *dictionary, uint8_t dict_len);
static void Print(Row *parameter, uint8_t index);
static void AcceptEdit(Row *parameter);
static int ShiftCursor(uint8_t direction, Row *par, uint8_t stepSize);
static uint8_t OnNumberPressKey(uint8_t key);
static uint8_t OnDeletePressKey();
static uint8_t OnDecPressKey();
static void ParseEthernetAddress(Row *par);
static uint8_t IsNumber(char *symbol);
static void DecIncSymbol(char *symbol, uint8_t dir);
static void GetEditedValue(Row *par);


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
				.name = errKanistra
		},
		{
				.name = errDoorKanistra
		},
		{
				.name = errUZ
		},
		{
				.name = abortErr
		},
		{
				.name = sqLeft
		},
		{
				.name = sqRight
		},
		{
				.name = timeoutRight
		},
		{
				.name = timeoutLeft
		},
		{
				.name = needReturn
		},
		{
				.name = fullNakop
		}
};


Row probotbor_parameters[] = {
		{
				.name = parTimeoutLeft1,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.prob_toHomeTime),
				.isEnum = 0,
				.name_len = sizeof(parTimeoutLeft1),
				.param_len = 3,
				.param_pos = 3,
				.type = ROW_USHORT

		},
		{
				.name = parTimeoutRight2,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.prob_toWorkTime),
				.isEnum = 0,
				.name_len = sizeof(parTimeoutRight2),
				.param_len = 3,
				.param_pos = 3,
				.type = ROW_USHORT
		},
		{
				.name = parAutoSel3,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.automat_timer),
				.isEnum = 0,
				.name_len = sizeof(parAutoSel3),
				.param_len = 5,
				.param_pos = 3,
				.type = ROW_UINT
		},
		{
				.name = nakopSV4,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.nakop_SV),
				.isEnum = 0,
				.name_len = sizeof(nakopSV4),
				.param_len = 2,
				.param_pos = 3,
				.type = ROW_USHORT
		}


};

Row commParameters[] = {
		{
				.name = parEth1,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.eth_sett.addr0),
				.isEnum = 0,
				.name_len = sizeof(parEth1),
				.param_pos = 2,
				.type = ROW_ETH_ADDR
		},
		{
				.name = parMask2,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.eth_sett.mask0),
				.isEnum = 0,
				.name_len = sizeof(parMask2),
				.param_pos = 2,
				.type = ROW_ETH_ADDR
		},
		{
				.name = parGateway,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.eth_sett.gateway0),
				.isEnum = 0,
				.name_len = sizeof(parGateway),
				.param_pos = 2,
				.type = ROW_ETH_ADDR
		},
		{
				.name = parBaudrate,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.rs_sett.baudrate),
				.isEnum = 1,
				.name_len = sizeof(parBaudrate),
				.param_pos = 2,
				.enums = baudrates,
				.enums_len = sizeof(baudrates)/sizeof(Dictionary),
				.type = ROW_UINT
		},
		{
				.name = parParity,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.rs_sett.parity),
				.isEnum = 1,
				.name_len = sizeof(parParity),
				.param_pos = 2,
				.enums = parities,
				.enums_len = sizeof(parities)/sizeof(Dictionary),
				.type = ROW_USHORT
		},
		{
				.name = parModbAddr,
				.isEdited = 1,
				.data = (uint8_t*)(&settings.retain.mb_addr),
				.isEnum = 0,
				.name_len = sizeof(parModbAddr),
				.param_pos = 2,
				.param_len = 3,
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
		{
				.name = parNakopCV,
				.isEdited = 0,
				.data = (uint8_t*)(&meas_data.probInKanistra),
				.isEnum = 0,
				.name_len = sizeof(parNakopCV),
				.param_len = 2,
				.param_pos = 3,
				.type = ROW_USHORT
		},
};


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
		if(!OnDeletePressKey())return 0;
	}
	else if(!strncmp(p, "Up", strlen("Up")))
	{
		if(!OnUpPressKey())return 0;
	}
	else if(!strncmp(p, "Right", strlen("Right")))
	{
		if(!OnRightPressKey())return 0;
	}
	else if(!strncmp(p, "Left", strlen("Left")))
	{
		if(!OnLeftPressKey())return 0;
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
		if(!OnNumberPressKey(0))return 0;
	}
	else if(!strncmp(p, "NumPad1", strlen("NumPad1")))
	{
		if(!OnNumberPressKey(1))return 0;
	}
	else if(!strncmp(p, "NumPad2", strlen("NumPad2")))
	{
		if(!OnNumberPressKey(2))return 0;
	}
	else if(!strncmp(p, "NumPad3", strlen("NumPad3")))
	{
		if(!OnNumberPressKey(3))return 0;
	}
	else if(!strncmp(p, "NumPad4", strlen("NumPad4")))
	{
		if(!OnNumberPressKey(4))return 0;
	}
	else if(!strncmp(p, "NumPad5", strlen("NumPad5")))
	{
		if(!OnNumberPressKey(5))return 0;
	}
	else if(!strncmp(p, "NumPad6", strlen("NumPad6")))
	{
		if(!OnNumberPressKey(6))return 0;
	}
	else if(!strncmp(p, "NumPad7", strlen("NumPad7")))
	{
		if(!OnNumberPressKey(7))return 0;
	}
	else if(!strncmp(p, "NumPad8", strlen("NumPad8")))
	{
		if(!OnNumberPressKey(8))return 0;
	}
	else if(!strncmp(p, "NumPad9", strlen("NumPad9")))
	{
		if(!OnNumberPressKey(9))return 0;
	}
	else if(!strncmp(p, "Dec", strlen("Dec")))
	{
		if(!OnDecPressKey())return 0;
	}
	else return 0;
	memcpy(answer,lcdAnswer,80);
	if(currentCursorPosition==-1)
	{
		answer[80] = 255;
		return -1;

	}
	else
	{
		answer[80] = currentCursorPosition+40;
	}
	return currentCursorPosition+40;
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
				memset(editedValue,0,20);
				Clear();
				if(!par->isEnum)
				{
					GetEditedValue(par);
					editMode = 1;
					currentCursorPosition = par->param_pos;
					ShowParameter(par);
					return 1;
				}
				else
				{
					if(par->enums==NULL)return 0;
					memset(editedValue,0,20);
					switch (par->type) {
						case ROW_USHORT:
							currentEnumKey = *((uint16_t*)par->data);
							break;
						case ROW_UINT:
							currentEnumKey = *((uint32_t*)par->data);
							break;
						default:
							break;
					}
					Dictionary *dict = FindDictionaryFromValue(currentEnumKey, par->enums, par->enums_len);
					if(dict==NULL)
					{
						currentEnumKey = par->enums->value;
						memcpy(editedValue+par->param_pos,par->enums->name, strlen(par->enums->name));
					}
					else
					{
						currentEnumKey = dict->value;
						memcpy(editedValue+par->param_pos,dict->name, strlen(dict->name));
					}
					currentCursorPosition = -1;
					editMode = 1;
					ShowParameter(par);
					return 1;
				}
			}
		}
		else
		{
			AcceptEdit(par);
			ShowParameter(par);
			return 1;
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
		//Aвто ВКЛ
		char  autoOn[] = {0x41, 0xE2, 0xF2, 0xEE, 0x20, 0xC2, 0xCA, 0xCB,  0x00};
		sprintf(lcdAnswer[0],"%s %d/%d",autoOn, meas_data.probInKanistra, settings.retain.nakop_SV);
		//Отбор через n с
		char next[] = {0xCE, 0xF2, 0xE1, 0xEE, 0xF0, 0x20, 0xF7, 0xE5, 0xF0, 0xE5, 0xE7,  0x00};
		// Накопитель полный
		char full[] = {0xCD, 0xE0, 0xEA, 0xEE, 0xEF, 0xE8, 0xF2, 0xE5, 0xEB, 0xFC, 0x20, 0xEF, 0xEE, 0xEB, 0xED, 0xFB, 0xE9,  0x00};

		if(meas_data.nakopitelFull)
		{
			memcpy(lcdAnswer[2],full,sizeof(full));
		}
		else
		{
			if(meas_data.probotbor_ready)sprintf(lcdAnswer[2],"%s %d c",next, meas_data.toNextOtborTime);
		}

	}
	else
	{
		//Aвто ОТКЛ
		char  autoOff[] = {0x41, 0xE2, 0xF2, 0xEE, 0x20, 0xCE, 0xD2, 0xCA, 0xCB,  0x00};
		sprintf(lcdAnswer[0],"%s %d/%d",autoOff, meas_data.probInKanistra, settings.retain.nakop_SV);
	}

	if(meas_data.probotbor_ready)
	{
		//Готов к отбору
		char  ready[] = {0xC3,0xEE,0xF2,0xEE,0xE2,0x20,0xEA,0x20,0xEE,0xF2,0xE1,0xEE,0xF0,0xF3};
		memcpy(lcdAnswer[1],ready,sizeof(ready));
	}
	else if(meas_data.probotbor_busy)
	{
		//Выполняется отбор
		char  execSel[] = {0xC2,0xFB,0xEF,0xEE,0xEB,0xED,0xFF,0xE5,0xF2,0xF1,0xFF,0x20,0xEE,0xF2,0xE1,0xEE,0xF0};
		memcpy(lcdAnswer[1],execSel,sizeof(execSel));
	}
	else if(meas_data.vozvrat_probotbor_busy)
	{
		//Выполняется возврат
		char  execRet[] = {0xC2,0xFB,0xEF,0xEE,0xEB,0xED,0xFF,0xE5,0xF2,0xF1,0xFF,0x20,0xE2,0xEE,0xE7,0xE2,0xF0,0xE0,0xF2};
		memcpy(lcdAnswer[1],execRet,sizeof(execRet));
	}
	else if(*((uint16_t*)&meas_data.errors))
	{
		// Ошибки, нажмите F4
		char err[] = {0xCE, 0xF8, 0xE8, 0xE1, 0xEA, 0xE8, 0x2C, 0x20, 0xED, 0xE0, 0xE6, 0xEC, 0xE8, 0xF2, 0xE5, 0x20, 0x46, 0x34,  0x00};
		memcpy(lcdAnswer[1],err,sizeof(err));
	}
	// Мониторинг->Enter
	char toMonitor[] = {0xCC, 0xEE, 0xED, 0xE8, 0xF2, 0xEE, 0xF0, 0xE8, 0xED, 0xE3, 0x2D, 0x3E, 0x45, 0x6E, 0x74, 0x65, 0x72};
	memcpy(lcdAnswer[3]+3,toMonitor,sizeof(toMonitor));
	return 1;

}


static uint8_t OnDownPressKey()
{
	if(currentParameters == NULL)return 0; // Если находимся на главном экране, то не реагируем
	if(editMode)
	{
		Row *par = currentParameters+paramIndex;
		if(par->isEnum)return 0;
		DecIncSymbol(editedValue+currentCursorPosition, 0);
		ShowParameter(par);
		return 1;
	}
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

static uint8_t OnLeftPressKey()
{
	int index = 0;
	if(!editMode)return 0;
	if(currentParameters==NULL)return 0;
	Row *par = currentParameters+paramIndex;
	if(par->isEnum)
	{
		if(par->enums==NULL)return 0;
		// поиск индекса вхождения
		index = FindIndexFromValue(currentEnumKey, par->enums, par->enums_len);
		if(index==-1)return 0;
		index = index==0 ? par->enums_len-1 : index-1;
		currentEnumKey = (par->enums+index)->value;
		memset(editedValue,0,20);
		strncpy(editedValue+par->param_pos,(par->enums+index)->name, strlen((par->enums+index)->name));
		ShowParameter(par);
		return 1;


	}
	else
	{
		currentCursorPosition = ShiftCursor(0, par, 1);
		ShowParameter(par);
		return 1;

	}
	return 0;
}

static uint8_t OnRightPressKey()
{
	int index = 0;
	if(!editMode)return 0;
	if(currentParameters==NULL)return 0;
	Row *par = currentParameters+paramIndex;
	if(par->isEnum)
	{
		if(par->enums==NULL)return 0;
		// поиск индекса вхождения
		index = FindIndexFromValue(currentEnumKey, par->enums, par->enums_len);
		if(index==-1)return 0;
		index = index==par->enums_len-1 ? 0 : index+1;
		currentEnumKey = (par->enums+index)->value;
		memset(editedValue,0,20);
		strncpy(editedValue+par->param_pos,(par->enums+index)->name, strlen((par->enums+index)->name));
		ShowParameter(par);
		return 1;

	}
	else
	{
		currentCursorPosition = ShiftCursor(1, par, 1);
		ShowParameter(par);
		return 1;
	}
	return 0;
}

static uint8_t OnUpPressKey()
{
	if(currentParameters == NULL)return 0; // Если находимся на главном экране, то не реагируем
	if(editMode)
	{
		Row *par = currentParameters+paramIndex;
		if(par->isEnum)return 0;
		DecIncSymbol(editedValue+currentCursorPosition, 1);
		ShowParameter(par);
		return 1;
	}
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
	groupParamNameLength = sizeof(processParametersName);
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
	if(!editMode)
	{
		currentParameters = NULL;
		return ShowMainDisplay();
	}
	else
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
		if(editMode)
		{
			strncpy(lcdAnswer[2]+parameter->param_pos,editedValue+parameter->param_pos, strlen(editedValue+parameter->param_pos));
		}
		else
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

	}
	else
	{
		Print(parameter, 2);
	}

	if(parameter->isEdited)
	{
		if(!editMode)
		{
			//Изм.->Enter
			char edit[] = {0xC8, 0xE7, 0xEC, 0x2E, 0x2D, 0x3E, 0x45, 0x6E, 0x74, 0x65, 0x72};
			strncpy(lcdAnswer[3]+9, edit, sizeof(edit));
		}
		else
		{
			// Назад->Esc
			char back[] = {0xCD, 0xE0, 0xE7, 0xE0, 0xE4, 0x2D, 0x3E, 0x45, 0x73, 0x63};
			memcpy(lcdAnswer[3],back, sizeof(back));
			// Ok->Enter
			char ok[] = { 0x4F, 0x6B, 0x2D, 0x3E, 0x45, 0x6E, 0x74, 0x65, 0x72};
			memcpy(lcdAnswer[3]+11,ok, sizeof(ok));

		}


	}
	else if(currentParameters == indication_rows)
	{
		//Главная->Esc
		char toMain[] = {0xC3, 0xEB, 0xE0, 0xE2, 0xED, 0xE0, 0xFF, 0x2D, 0x3E, 0x45, 0x73, 0x63,  0x00};
		memcpy(lcdAnswer[3]+8,toMain, sizeof(toMain));
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
	//Ошибки
	char erors[] = {0xCE, 0xF8, 0xE8, 0xE1, 0xEA, 0xE8,  0x00};
	memcpy(lcdAnswer[0],erors, sizeof(erors));
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
	if(meas_data.errors.full_nakop_err)
{
		errors_info[err_count].num = err_count+1;
		errors_info[err_count].errorName = Errors[9].name;
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

/*Возращает индекс "ключ-значение", если записи нет, -1*/
static int FindIndexFromValue(float value, Dictionary *dictionary, uint8_t dict_len)
{
	Dictionary *temp = NULL;
	for (int i = 0; i < dict_len; ++i) {
		temp = dictionary+i;
		if(temp->value == value)return i;

	}
	return -1;
}



uint8_t LcdUpdate(uint8_t *answer)
{
	uint8_t result = 0;
	if(currentParameters==NULL)
	{
		result =  ShowMainDisplay();
	}
	else if(currentParameters==Errors)
	{
		result =  ShowErrors();
	}
	else if(currentParameters == indication_rows)
	{
		ShowParameter(currentParameters+paramIndex);
		result =  1;
	}
	if(result)
	{
		memcpy(answer,lcdAnswer,80);
		return 1;
	}
	return 0;
}

static void Print(Row *parameter, uint8_t index)
{
	uint16_t temp = 0;
	if(editMode)
	{
		memcpy(lcdAnswer[index],editedValue,20);
		return;
	}
	switch (parameter->type) {
		case ROW_ETH_ADDR:
			sprintf(lcdAnswer[index]+parameter->param_pos,"%03d.%03d.%03d.%03d",*((uint16_t*)parameter->data),*(((uint16_t*)parameter->data)+1),*(((uint16_t*)parameter->data)+2),*(((uint16_t*)parameter->data)+3));
			break;
		case ROW_USHORT:
			sprintf(lcdAnswer[index]+parameter->param_pos,"%d",*((uint16_t*)parameter->data));
			break;
		case ROW_FLOAT:
			sprintf(lcdAnswer[index]+parameter->param_pos,"%f",*((float*)parameter->data));
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

static void AcceptEdit(Row *par)
{
	if(editMode==0)return;
	if(par->isEnum)
	{
		switch (par->type) {
			case ROW_USHORT:
				*((uint16_t*)par->data) = (uint16_t)currentEnumKey;
				break;
			case ROW_UINT:
				*((uint32_t*)par->data) = (uint32_t)currentEnumKey;
				break;
			case ROW_FLOAT:
				*((float*)par->data) = currentEnumKey;
				break;
			default:
				break;
		}
	}
	else
	{
		int tmpi = 0;
		float tmpf;
		switch (par->type) {
			case ROW_ETH_ADDR:
				ParseEthernetAddress(par);
				break;
			case ROW_FLOAT:
				sscanf(editedValue+par->param_pos, "%f", (float*)par->data);
				break;
			case ROW_UINT:
				sscanf(editedValue+par->param_pos, "%d", &tmpi);
				*((uint32_t*)(par->data)) = tmpi;
				break;
			case ROW_USHORT:
				sscanf(editedValue+par->param_pos, "%d", &tmpi);
				*((uint16_t*)(par->data)) = (uint16_t)tmpi;
				break;
			default:
				break;
		}
	}
	settingsSaveFRAM();
	editMode = 0;
	currentCursorPosition = -1;

}

// direction == 0 ->left
// direction == 1 ->right
// return position
static int ShiftCursor(uint8_t direction, Row *par, uint8_t stepSize)
{
	if(!editMode)return -1;
	if(par->isEnum)return -1;
	int tempPos = direction ? currentCursorPosition+stepSize : currentCursorPosition-stepSize;
	// Проверка на границы

	if(tempPos<par->param_pos)tempPos = par->param_pos;
	if(tempPos>19)tempPos = 19;
	while(editedValue[tempPos]==0 && tempPos>par->param_pos)
	{
		tempPos--;
	}
	switch (par->type) {
		case ROW_ETH_ADDR:
			if(editedValue[tempPos]=='.')
			{
				tempPos = direction ? tempPos+1 : tempPos-1;
			}
			break;
		default:
			break;
	}
	return tempPos;
}

static uint8_t OnNumberPressKey(uint8_t key)
{
	if(key>9)return 0;
	uint8_t i = 0;
	if(!editMode)return 0;
	if(currentParameters==NULL)return 0;
	Row *par = currentParameters+paramIndex;
	switch (par->type) {
		case ROW_ETH_ADDR:
			break;
		default:
			break;
	}
	editedValue[currentCursorPosition] = 48+key;
	currentCursorPosition = ShiftCursor(1, par, 1);
	ShowParameter(par);
	return 1;
}

static uint8_t OnDeletePressKey()
{
	uint8_t i;
	if(!editMode)return 0;
	if(currentParameters==NULL)return 0;
	Row *par = currentParameters+paramIndex;
	switch (par->type) {
		case ROW_ETH_ADDR:
			return 0;
		default:
			editedValue[par->param_len+par->param_pos-1]='0';
			for (i = currentCursorPosition; i < par->param_len+par->param_pos-1; ++i) {
				editedValue[i] = editedValue[i+1];
			}
			editedValue[19]=0;
			break;
	}
	ShowParameter(par);
	return 1;
}

static uint8_t OnDecPressKey()
{

	uint8_t i = 0;
	if(!editMode)return 0;
	if(currentParameters==NULL)return 0;
	Row *par = currentParameters+paramIndex;
	if(par->type != ROW_FLOAT)return 0;
	switch (par->type) {
		case ROW_ETH_ADDR:
			return 0;
		default:
			break;
	}
	editedValue[currentCursorPosition] = '.';
	currentCursorPosition = ShiftCursor(1, par, 1);
	ShowParameter(par);
	return 1;
}

static void ParseEthernetAddress(Row *par)
{
	int tmp;
	char *p = editedValue+par->param_pos;
	for (int i = 0; i < 4; ++i) {
		sscanf(p, "%d", &tmp);
		p = strchr(p, '.');
		if(p!=NULL)p=p+1;
		*((uint16_t*)par->data+i) = tmp%256;
	}
}

static uint8_t IsNumber(char *symbol)
{
	return *symbol>=48 && *symbol<=57;
}

static void DecIncSymbol(char *symbol, uint8_t dir)
{
	if(!IsNumber(symbol))return;
	if(dir)
	{
		*symbol = *symbol < 57 ? *symbol+1 : 48;
	}
	else
	{
		*symbol = *symbol > 48 ? *symbol-1 : 57;
	}
}

static void GetEditedValue(Row *par)
{
	if(par == NULL)return;
	switch (par->type) {
		case ROW_ETH_ADDR:
			sprintf(editedValue+par->param_pos,"%03d.%03d.%03d.%03d",*((uint16_t*)par->data),*(((uint16_t*)par->data)+1),*(((uint16_t*)par->data)+2),*(((uint16_t*)par->data)+3));
			break;
		case ROW_UINT:
			sprintf(editedValue+par->param_pos,"%0*d",par->param_len, *((uint32_t*)par->data));
			break;
		case ROW_USHORT:
			sprintf(editedValue+par->param_pos,"%0*d",par->param_len, *((uint16_t*)par->data));
			break;
		case ROW_FLOAT:
			sprintf(editedValue+par->param_pos,"%0*f",par->param_len, *((float*)par->data));
			break;
		default:
			break;
	}

}

