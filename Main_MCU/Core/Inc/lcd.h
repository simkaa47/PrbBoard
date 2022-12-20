/*
 * lcd.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Serafim
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#define CLEAR_DISPLAY 0x1
#define RETURN_HOME 0x2
#define ENTRY_MODE_SET 0x6 // mode cursor shift rihgt, display non shift
#define DISPLAY_ON 0xC // non cursor
#define DISPLAY_OFF 0x8
#define CURSOR_SHIFT_LEFT 0x10
#define CURSOR_SHIFT_RIGHT 0x14
#define DISPLAY_SHIFT_LEFT 0x18
#define DISPLAY_SHIFT_RIGHT 0x1C
#define DATA_BUS_4BIT_PAGE0 0x28
#define DATA_BUS_4BIT_PAGE1 0x2A
#define DATA_BUS_8BIT_PAGE0 0x38
#define SET_CGRAM_ADDRESS 0x40 // usage address |= SET_CGRAM_ADDRESS
#define SET_DDRAM_ADDRESS 0x80

#define     LCD_PIN_MASK   (LCDA_Pin | LCDE_Pin | LCD3_Pin | LCD2_Pin | LCD1_Pin | LCD0_Pin)

void lcd_init();//  Инициализация дисплея
void sendStr( char *str, int row , int position );

#endif /* INC_LCD_H_ */
