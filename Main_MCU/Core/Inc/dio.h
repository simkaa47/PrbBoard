/*
 * dio.h
 *
 *  Created on: Dec 13, 2022
 *      Author: Serafim
 */

#ifndef INC_DIO_H_
#define INC_DIO_H_

#define INPUT_NUMBER 12
#define ANTIDREBEZG_CNT 5


typedef struct
{
	unsigned int uz_prob_fail :1;// Неисправность ПЧ1
	unsigned int sq_kovsh_prob_right_1 :1;// ковш справа 1
	unsigned int sq_kovsh_prob_right_2 :1;// ковш справа 2
	unsigned int sq_kovsh_prob_left_1 :1;// ковш слева 1
	unsigned int sq_kovsh_prob_left_2 :1;// ковш слева 2
	unsigned int sb_stop :1;// кнопка аварийный стоп
	unsigned int sb_make_proba :1;// кнопка "Отобрать пробу"
	unsigned int sb_vozvrat :1;// кнопка "Возврат"
	unsigned int sb_auto_local :1;// переключатель "Авто"
	unsigned int sq_kanistra :1;// датчик канистры
	unsigned int sq_kanistra_door :1;// датчик двери канистры
}DI;

typedef struct
{
	unsigned int uz_prob_forv :1;// Двигатель проботборника - вперед
	unsigned int uz_prob_rev :1;// Двигатель проботьборника - назад
	unsigned int prob_ready :1;// Возможность проботбора

}DO;

void Read_Inputs();
void Write_Outputs();


#endif /* INC_DIO_H_ */
