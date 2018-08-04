/*
 * motor_ap4988.h
 *
 *  Created on: 12-Apr-2018
 *      Author: medprime
 */

#ifndef MOTOR_A4988_H_
#define MOTOR_A4988_H_

#include"stm32f1xx_hal.h"
#include"delay_us.h"


#define CLOCK_WISE         1
#define COUNTER_CLOCK_WISE 0

typedef struct Motor_A4988_Struct_t
{
	GPIO_TypeDef* Port;

	uint16_t STP_Pin;
	uint16_t DIR_Pin;
	uint16_t EN_Pin;


	uint16_t Limit_Pin;
	uint8_t  Limit_Pin_Logic;// HIGH/LOW  SET/RESET
	uint8_t  Limit_Flag;


	uint16_t RPM;
	uint16_t Steps_For_360;

	uint32_t Step_Delay;
	uint8_t  Step_Delay_Complete_Flag;

	uint8_t  Running_Flag;

	int32_t  Current_Position;
	int32_t  Steps_To_Move;

	uint32_t  Time_Stamp;

} Motor_A4988_Struct_t;



void Motor_A4988_Run(Motor_A4988_Struct_t* Motor);
void Motor_A4988_Init(Motor_A4988_Struct_t* Motor);
void Motor_A4988_Set_RPM(Motor_A4988_Struct_t* Motor, uint16_t RPM);
void Motor_A4988_Move_Steps(Motor_A4988_Struct_t* Motor, int32_t Steps);
void Motor_A4988_Move_Degrees(Motor_A4988_Struct_t* Motor, int32_t Degrees);
void Motor_A4988_Move_To_Position(Motor_A4988_Struct_t* Motor, int32_t Position);
void Motor_A4988_Enable(Motor_A4988_Struct_t* Motor);
void Motor_A4988_Disable(Motor_A4988_Struct_t* Motor);
void Motor_A4988_Run_All();
void Motor_A4988_Check_LMT_All();




#endif /* MOTOR_A4988_H_ */
