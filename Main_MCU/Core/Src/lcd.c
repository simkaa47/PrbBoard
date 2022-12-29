/*
 * lcd.c
 *
 *  Created on: Dec 15, 2022
 *      Author: Serafim
 */

#include <main.h>
#include <string.h>
#include <lcd.h>
#include <control_bit.h>



static void sendByte(uint8_t byte, int isData);

void lcd_init()
{
	HAL_GPIO_WritePin(LCDLED_GPIO_Port, LCDLED_Pin, SET);// Включаем подсветку
	HAL_GPIO_WritePin(LCDR_GPIO_Port, LCDR_Pin, RESET);
	HAL_Delay(15);

	sendByte(0x33, 0); // шлем в одном байте два 0011
	HAL_Delay(1);

	sendByte(0x32, 0); // шлем в одном байте  00110010
	HAL_Delay(1);

	sendByte(DATA_BUS_4BIT_PAGE0, 0); // включаем режим 4 бит
	HAL_Delay(1);
	sendByte(DISPLAY_OFF, 0); // выключаем дисплей
	HAL_Delay(1);
	sendByte(CLEAR_DISPLAY, 0); // очищаем дисплей
	HAL_Delay(2);
	sendByte(ENTRY_MODE_SET, 0); //ставим режим смещение курсора экран не смещается
	HAL_Delay(1);
	sendByte(DISPLAY_ON, 0);// включаем дисплей и убираем курсор
	HAL_Delay(1);



}

static void sendByte(uint8_t byte, int isData)
{
	HAL_GPIO_WritePin(LCD0_GPIO_Port, LCD_PIN_MASK, RESET);

	if(isData == 1) HAL_GPIO_WritePin(LCDA_GPIO_Port, LCDA_Pin, SET); // если данные ставмим A
	else HAL_GPIO_WritePin(LCDA_GPIO_Port, LCDA_Pin, RESET);		   // иначе скидываем A

	// ставим старшую тетраду на порт
	if(byte & 0x80) HAL_GPIO_WritePin(LCD3_GPIO_Port, LCD3_Pin, SET);
	if(byte & 0x40) HAL_GPIO_WritePin(LCD2_GPIO_Port, LCD2_Pin, SET);
	if(byte & 0x20) HAL_GPIO_WritePin(LCD1_GPIO_Port, LCD1_Pin, SET);
	if(byte & 0x10) HAL_GPIO_WritePin(LCD0_GPIO_Port, LCD0_Pin, SET);

	// поднимаем пин E
	HAL_GPIO_WritePin(LCDE_GPIO_Port, LCDE_Pin, SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCDE_GPIO_Port, LCDE_Pin, RESET); // сбрасываем пин Е

	//обнуляем все пины дисплея кроме RS
	HAL_GPIO_WritePin(LCD0_GPIO_Port, (LCD_PIN_MASK & ~LCDA_Pin), RESET);

	// поднимаем пин E
	HAL_GPIO_WritePin(LCDE_GPIO_Port, LCDE_Pin, SET);
	if(byte & 0x08) HAL_GPIO_WritePin(LCD3_GPIO_Port, LCD3_Pin, SET);
	if(byte & 0x04) HAL_GPIO_WritePin(LCD2_GPIO_Port, LCD2_Pin, SET);
	if(byte & 0x02) HAL_GPIO_WritePin(LCD1_GPIO_Port, LCD1_Pin, SET);
	if(byte & 0x01) HAL_GPIO_WritePin(LCD0_GPIO_Port, LCD0_Pin, SET);
	HAL_GPIO_WritePin(LCDE_GPIO_Port, LCDE_Pin, RESET); // сбрасываем пин Е

	HAL_Delay(1);
}


void sendStr( char *str, int row , int position )
{
	char start_address;

	switch (row)
	{

		case 0:
			start_address = 0x0; // 1 строка
			break;

		case 1:
			start_address = 0x40; // 2 строка
			break;

		case 3:
			start_address = 0x14; // 3 строка
			break;

		case 3:
			start_address = 0x54; // 4 строка
			break;

	}

	start_address += position; // к началу строки прибавляем позицию в строке

	sendByte((start_address |= SET_DDRAM_ADDRESS), 0); // ставим курсор на начало нужной строки  в DDRAM

	HAL_Delay(4);
	while(*str != '\0'){

		sendByte(*str, 1);
		str++;
	}
}




