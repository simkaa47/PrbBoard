/*
 * modbus.c
 *
 *  Created on: 2 дек. 2022 г.
 *      Author: Serafim
 */
#include <modbus.h>
#include <settings.h>
#include <stdio.h>
#include <control_bit.h>
#include <string.h>
#include "cmsis_os.h"

static uint16_t *holding_pointer;//Указатель на holding регистры
static uint16_t *reading_pointer;//Указатель на reading регистры
static uint16_t holding_size;// Размер holding регистров
static uint16_t reading_size;// Размер reading регистров

extern Settings_Struct settings;
extern Meas_Data meas_data;
extern osSemaphoreId writeMemorySemaphoreHandle;


static uint8_t CheckRequestLength(uint8_t* request_pointer,int request_length, ModbusSource source);
static uint8_t CheckMbAddr(uint8_t *request,ModbusSource source);
static uint8_t IsCorrectMbCommand(uint8_t *request,ModbusSource source);
static int SetInvalidCommand(uint8_t *request, uint8_t *answer, uint8_t code, ModbusSource source);
static void CRC16_CALC(uint8_t* arr, uint8_t* checkSum, uint8_t messageLength);
static uint8_t CheckCrc(uint8_t* request_pointer,int request_length, ModbusSource source);
static int GetModbusAnswer(uint8_t* request, uint8_t* answer, ModbusSource source);
static int ReadRegisters(uint8_t *request, uint8_t *answer, uint16_t *memory_pointer, uint16_t memory_size, ModbusSource source);
static uint16_t GetWordFromBytes(uint8_t *arr, uint8_t pos);
static void InsertWordToMemory(uint16_t value, uint8_t* memory);
static void InsertWordsToMemory(uint16_t *source, uint8_t * destination, uint16_t count);
static int WriteSingleRegister(uint8_t* request, uint8_t* answer, ModbusSource source);
static int WrieMultiplyRegisters(uint8_t* request, uint8_t* answer,ModbusSource source);
static int WriteToHoldings(uint8_t* request, uint8_t* answer,ModbusSource source, int (*write)(uint8_t*,uint8_t*,ModbusSource));

void ModbusInit()
{
	holding_pointer = (uint16_t*)&settings;
	reading_pointer = (uint16_t*)&meas_data;
	holding_size = sizeof(Client_Info)/2;
	reading_size = sizeof(meas_data)/2;
	if(settings.retain.mb_addr==0)settings.retain.mb_addr = 1;
}


//Ф-я парсинга входящего буфера, если это Modbus, то возвращает кол-во байт
int ModbusParse(uint8_t *request, uint16_t req_length, uint8_t *answer, ModbusSource source)
{
	settings.non_retain.data[37]++;
	if(answer==NULL)return 0; // Проверка, выделилась ли память под ответ
	if(!CheckRequestLength(request,req_length,source))return 0; // Проверка корректности длины пакета
	if(!CheckMbAddr(request, source))return 0; // Проверка корректности адреса
	if(!IsCorrectMbCommand(request, source))// Проверка корректности функционального кода
	{
		return SetInvalidCommand(request, answer, 1, source);
	}
	if(!CheckCrc(request, req_length, source))
	{
		return SetInvalidCommand(request, answer, 3, source);
	}
	return GetModbusAnswer(request, answer, source);
}

//Проверка соответствия длины данных, передаваемых в буфере
static uint8_t CheckRequestLength(uint8_t* request_pointer,int request_length, ModbusSource source)
{
	if(source==RS485)return 1;
    int len_from_packet = (*(request_pointer+4))*256 + *(request_pointer+5);
    return len_from_packet+6 == request_length;
}

//Проверка соответсия адреса в сети Modbus сервера
static uint8_t CheckMbAddr(uint8_t *request,ModbusSource source)
{
	uint8_t addr = 0;
	if(source==ETHERNET)
	{
		addr =  *(request+6);
	}
	else if(source==RS485)
	{
		addr =  *(request);
	}
	return addr == settings.retain.mb_addr;
}

//Проверка корректности функционального кода
static uint8_t IsCorrectMbCommand(uint8_t *request,ModbusSource source)
{
	uint8_t cmd = 0;
	if(source==ETHERNET)
	{
		cmd = *(request+7);
	}
	else if(source==RS485)
	{
		cmd = *(request+1);
	}
	/*
	 * Функция возвращает 1, если номер команды корректен, 0 - если некорректен
	 * cmd - номер команды
	 * */
	switch (cmd) {
	    //case 1:
		case 3:
		case 4:
		case 6:
		//case 5:
        case 16:
            return 1;
		default:
			return 0;
	}
}

//Формирует телеграмму ошиобного запроса
static int SetInvalidCommand(uint8_t *request, uint8_t *answer, uint8_t code, ModbusSource source)
{
	uint8_t func_code = source ? *(request+1) : *(request+1);
	func_code = set_bit(func_code, 7);
	if(source==RS485)
	{
		uint8_t check_sum[2]={0};// временный массив для контрольной суммы
		memcpy(answer,request,1);
		*(answer+1) = func_code;//Функциональный код с измененным битом
		*(answer+2) = code;//Функциональный код с измененным битом
		CRC16_CALC(answer,check_sum,3);
		*(answer+3) = check_sum[0];
		*(answer+4) = check_sum[1];
	}
	else if(source==ETHERNET)
	{
		memcpy(answer,request,8);
		*(answer+4) = 0;
		*(answer+5) = 3;
		*(answer+7) |= 128;
		*(answer+8) = code;
	}

	uint8_t len = source==RS485 ? 5 : 9;
	return len;
}

static void CRC16_CALC(uint8_t* arr, uint8_t* checkSum, uint8_t messageLength)
{
	/*Функция расчета контрольной суммы
		 * arr - указатель на массив байт, по которым нужно посчитать CRC
		 * checkSum - указатель на массив из 2 байт, куда нужно положить CRC
		 * messageLength - ко-во байт, по которым нужно считать CRC	 *
		 * */
		uint16_t reg = 0xFFFF;
		uint16_t polinom = 0xA001;
		for(int i=0;i < messageLength;i++)
		{
			reg^=*(arr+i);
			for (int j = 0; j < 8; j++)
			{
				if ((reg&1)==1) {
					reg>>=1;
					reg^=polinom;
				}
				else reg>>=1;
			}
		}
		*(checkSum+1)=(uint8_t)(reg>>8);
		*checkSum = (uint8_t)(reg & 0x00FF);
}

//Проверка контрольной суммы запроса
static uint8_t CheckCrc(uint8_t* request_pointer,int request_length, ModbusSource source)
{
	if(source==ETHERNET)return 1;
	uint8_t check_sum[2]={0};// временный массив для контрольной суммы
	if(request_length<4)return 0;
	CRC16_CALC(request_pointer, check_sum, request_length-2);
	return request_pointer[request_length-2]==check_sum[0] && request_pointer[request_length-1]==check_sum[1];
}

//Возвращает кол-во байт, котрые нужно передать обратно
static int GetModbusAnswer(uint8_t* request, uint8_t* answer, ModbusSource source)
{
	uint8_t func_code = source==RS485 ? *(request+1) : *(request+7);

	switch (func_code) {
		case READ_HOLDING_REGS:
			return ReadRegisters(request, answer, holding_pointer, holding_size, source);
		case READ_INPUT_REGS:
			return ReadRegisters(request, answer, reading_pointer, reading_size, source);
		case WRITE_SINGLE_REG:
			return WriteToHoldings(request, answer, source, &WriteSingleRegister);
		case WRITE_MULTIPLY_REGS:
			return WriteToHoldings(request, answer, source, &WrieMultiplyRegisters);
		default:
			break;
	}
	return 0;
}

//Ф-я чтения регистров из памяти memory_pointer размером memory_size
static int ReadRegisters(uint8_t *request, uint8_t *answer, uint16_t *memory_pointer, uint16_t memory_size, ModbusSource source)
{
	uint16_t  maxAddr = memory_size-1;
	uint16_t start_reg = GetWordFromBytes(request,source==RS485?2:8);
	uint16_t reg_count = GetWordFromBytes(request,source==RS485?4:10);
	if((start_reg+reg_count-1)>maxAddr)return SetInvalidCommand(request, answer, 2, source); //проверка правильности адресации
	if(source==ETHERNET)
	{
		memcpy(answer,request,8);
		InsertWordToMemory(reg_count*2+3, answer+4);//Длина сообщения
		*(answer+8) = reg_count*2;//количесво байт далее
		InsertWordsToMemory(memory_pointer+start_reg,answer+9,reg_count);
		return 9+reg_count*2;
	}
	else
	{
		memcpy(answer,request,2);
		*(answer+2) = reg_count*2;//количесво байт далее
		InsertWordsToMemory(memory_pointer+start_reg,answer+3,reg_count);//Данные
		uint8_t check_sum[2]={0};// временный массив для контрольной суммы
		CRC16_CALC(answer,check_sum,3+reg_count*2);
		int offset = 3+reg_count*2;
		*(answer+offset)=check_sum[0];
		*(answer+offset+1)=check_sum[1];
		return 5+reg_count*2;

	}
	return 0;
}

static uint16_t GetWordFromBytes(uint8_t *arr, uint8_t pos)
{
	/*Получает 16 битное число из 2x байт, и возвращает его
	 * arr -  указатель на массив байт, из которого нужно получить word
	 * pos - индекс первого байта в массиве
	 *
	 * */
	return (((uint16_t)(*(arr+pos)))<<8) + (uint16_t)(*(arr + pos+1));
}

static void InsertWordToMemory(uint16_t value, uint8_t* memory)
{
    InsertWordsToMemory(&value,memory,1);
}

static void InsertWordsToMemory(uint16_t *source, uint8_t * destination, uint16_t count)
{
    int i = 0;
    for(i=0;i<count*2;i+=2)
    {
        *(destination+i+1)=*((uint8_t*)source+i);
        *(destination+i)=*((uint8_t*)source+i+1);
    }
}


static int WriteSingleRegister(uint8_t* request, uint8_t* answer, ModbusSource source)
{
    uint16_t  maxAddr = holding_size-1;
    uint16_t addr = GetWordFromBytes(request,source==RS485?2:8);
    if(addr>maxAddr)return SetInvalidCommand(request, answer, 2, source); //проверка правильности адресации
    //если все нормально, то вставляем в пакет запрашиваемые данные
    if(source==ETHERNET)
    {
    	InsertWordsToMemory((uint16_t*)(request+10), ((uint8_t*)holding_pointer)+addr*2,1);
    	memcpy(answer, request,12);
    	return 12;
    }
    else
    {
    	InsertWordsToMemory((uint16_t*)(request+4), ((uint8_t*)holding_pointer)+addr*2,1);
    	memcpy(answer, request,8);
    	return 8;
    }
    settingsSaveFRAM();
    return 0;
}

static int WrieMultiplyRegisters(uint8_t* request, uint8_t* answer,ModbusSource source)
{
	uint16_t  maxAddr = holding_size-1;
	uint16_t start_reg = GetWordFromBytes(request,source==RS485?2:8);
	uint16_t reg_count = GetWordFromBytes(request,source==RS485?4:10);
    if((start_reg+reg_count-1)>maxAddr)return SetInvalidCommand(request, answer, 2, source); //проверка правильности адресации
    //если все нормально, то вставляем в пакет запрашиваемые данные
    if(source==ETHERNET)
    {
    	memcpy(answer, request,12);//сначала повторяющиеся с запросом данные
    	InsertWordToMemory(reg_count*2+4, answer+4);//Длина сообщения
    	InsertWordsToMemory((uint16_t*)(request+13), ((uint8_t*)holding_pointer)+start_reg*2,reg_count);
        return 12;
    }
    else
    {
    	memcpy(answer, request,6);//сначала повторяющиеся с запросом данные
    	uint8_t check_sum[2]={0};// временный массив для контрольной суммы
    	CRC16_CALC(answer,check_sum,6);
		*(answer+6)=check_sum[0];
		*(answer+7)=check_sum[1];
		InsertWordsToMemory((uint16_t*)(request+7), ((uint8_t*)holding_pointer)+start_reg*2,reg_count);
		return 8;
    }

    return 0;
}

static int WriteToHoldings(uint8_t* request, uint8_t* answer,ModbusSource source, int (*write)(uint8_t*,uint8_t*,ModbusSource))
{
	int result = 0;
	if(writeMemorySemaphoreHandle!=NULL)
	{
		if(osSemaphoreWait(writeMemorySemaphoreHandle, 1000)==osOK)
		{
			result =  (*write)(request,answer,source);
			settingsSaveFRAM();
			osSemaphoreRelease(writeMemorySemaphoreHandle);
		}
	}
	return result;
}





