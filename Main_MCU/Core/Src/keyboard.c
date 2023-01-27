/*
 * keyboard.c
 *
 *  Created on: Dec 22, 2022
 *      Author: Serafim
 */
#include <keyboard.h>
#include <cmsis_os.h>
#include <string.h>
#include <stdio.h>
#include <lcd.h>
#include <keyboard_action.h>


uint8_t out_key_pin;
uint8_t key_input_mask[COLUMNS_COUNT][ROWS_COUNT]={0};
uint8_t key_input_filtered_mask[COLUMNS_COUNT][ROWS_COUNT]={0};
uint8_t key_input_old_mask[COLUMNS_COUNT][ROWS_COUNT]={0};
uint8_t key_sence_count[COLUMNS_COUNT][ROWS_COUNT]={0};
uint8_t positive_fronts[COLUMNS_COUNT][ROWS_COUNT]={0};

const char *keys[6][6] = {{NULL,NULL,NULL,NULL,NULL,NULL},
							{NULL,NULL,NULL,NULL,NULL,NULL},
							{"Esc","Left","F4","F3","F2","F1"},
							{NULL,"Up","Delete","NumPad7","NumPad4","NumPad1"},
							{"Down",NULL,"NumPad0","NumPad8","NumPad5","NumPad2"},
							{"Enter","Right","Dec","NumPad9","NumPad6","NumPad3"}};
char lcd[4][20];

uint8_t row_pointer = 0;
int counter = 0;


static void Tact_pins(void);
static void SendToLcd();
static void Key_Read(void);
static void GetPositiveFront(void);
static void key_filter(uint8_t x, uint8_t y);
static void key_filter_all();
static void PrintSymbol(const char *str);


void keyscan_thread(void *argument)
{
	int result = 0;
	int time;
	int last_time;

	result = OnKeyPress("Enter", strlen("Enter"), lcd);
	if(result)
	{
		SendToLcd();
	}
	while(1)
	{
		Tact_pins();
		Key_Read();
		key_filter_all();
		GetPositiveFront();
		for (int i = 0; i < 6; ++i) {
			for (int j = 0; j < 6; ++j) {
				if(positive_fronts[i][j])
				{
					if(keys[i][j]!=NULL)
					{
						result = OnKeyPress(keys[i][j], strlen(keys[i][j]), lcd);
						if(result)
						{
							SendToLcd();
						}
					}
				}
			}
		}
		time = HAL_GetTick();
		if((time-last_time)>500)
		{
			LcdUpdate();
			last_time = time;
		}
		osDelay(2);
	}
}

static void Tact_pins(void)
{
	out_key_pin = (out_key_pin+1)%COLUMNS_COUNT;
	switch(out_key_pin)
	{
		case 0:
			C0_HIGH;
			C1_LOW;
			C2_LOW;
			C3_LOW;
			C4_LOW;
			C5_LOW;
			break;
		case 1:
			C0_LOW;
			C1_HIGH;
			C2_LOW;
			C3_LOW;
			C4_LOW;
			C5_LOW;
			break;
		case 2:
			C0_LOW;
			C1_LOW;
			C2_HIGH;
			C3_LOW;
			C4_LOW;
			C5_LOW;
			break;
		case 3:
			C0_LOW;
			C1_LOW;
			C2_LOW;
			C3_HIGH;
			C4_LOW;
			C5_LOW;
			break;
		case 4:
			C0_LOW;
			C1_LOW;
			C2_LOW;
			C3_LOW;
			C4_HIGH;
			C5_LOW;
			break;
		case 5:
			C0_LOW;
			C1_LOW;
			C2_LOW;
			C3_LOW;
			C4_LOW;
			C5_HIGH;
			break;
	}
}


static void Key_Read(void)
{
	key_input_mask[out_key_pin][0] = R0_IN;
	key_input_mask[out_key_pin][1] = R1_IN;
	key_input_mask[out_key_pin][2] = R2_IN;
	key_input_mask[out_key_pin][3] = R3_IN;
	key_input_mask[out_key_pin][4] = R4_IN;
	key_input_mask[out_key_pin][5] = R5_IN;
}

static void key_filter_all()
{
	for (int x = 0; x < COLUMNS_COUNT; ++x) {
		for (int y = 0; y < ROWS_COUNT; ++y) {
			key_filter(x, y);
		}
	}
}

static void key_filter(uint8_t x, uint8_t y)
{
	if(key_input_mask[x][y])
	{
		key_sence_count[x][y]= key_sence_count[x][y]>=KEY_SENCE ? KEY_SENCE : key_sence_count[x][y]+1;
	}
	else{
		key_sence_count[x][y]= key_sence_count[x][y]<=0 ? 0 : key_sence_count[x][y]-1;
	}
	if(key_sence_count[x][y]==KEY_SENCE)key_input_filtered_mask[x][y] = 1;
	if(key_sence_count[x][y]==0)key_input_filtered_mask[x][y] = 0;
}

static void GetPositiveFront()
{
	for (int col = 0; col < COLUMNS_COUNT; ++col) {
		for (int row = 0; row < ROWS_COUNT; ++row) {
			positive_fronts[col][row] = key_input_filtered_mask[col][row]==1 && key_input_old_mask[col][row]==0 ? 1:0;
			key_input_old_mask[col][row] = key_input_filtered_mask[col][row];
		}
	}
}

static void PrintSymbol(const char *str)
{
	counter++;
	sprintf(lcd[row_pointer], "%s  %d",str,counter);
	row_pointer = row_pointer>=3 ? 0 : row_pointer+1;
	SendToLcd();
}

static void SendToLcd()
{
	sendStr(lcd[0], 0, 0);
	sendStr(lcd[1], 1, 0);
	sendStr(lcd[2], 2, 0);
	sendStr(lcd[3], 3, 0);
}


