/*
 * keyboard_action.h
 *
 *  Created on: 18 янв. 2023 г.
 *      Author: Serafim
 */

#ifndef INC_KEYBOARD_ACTION_H_
#define INC_KEYBOARD_ACTION_H_

#include <main.h>

int OnKeyPress(uint8_t *req,uint8_t req_length, uint8_t *answer);
uint8_t LcdUpdate(uint8_t *answer);

typedef enum
{
	ROW_USHORT,
	ROW_UINT,
	ROW_FLOAT,
	ROW_BINARY_12,
	ROW_ETH_ADDR

}row_type;

typedef struct
{
	float value;
	char *name;

}Dictionary;

typedef struct
{
	const char *name;
	uint8_t isEdited;
	uint8_t name_len;
	uint8_t param_len;
	uint8_t param_pos;
	uint8_t isEnum;
	uint8_t *data;
	Dictionary *enums;
	uint8_t enums_len;
	row_type type;

}Row;

typedef struct
{
	uint8_t num;
	char *errorName;
}Error_Indication_Struct;

#endif /* INC_KEYBOARD_ACTION_H_ */
