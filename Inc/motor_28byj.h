/*
 * motor_28byj.h
 *
 *  Created on: 11-Apr-2018
 *      Author: medprime
 */

#ifndef MOTOR_28BYJ_H_
#define MOTOR_28BYJ_H_

#include"stm32f1xx_hal.h"
#include "delay_us.h"

#define CLOCK_WISE 1
#define COUNTER_CLOCK_WISE 0

typedef  uint8_t Motor_28BYJ_t;

typedef struct Motor_28BYJ_Struct
{
	GPIO_TypeDef* Port;

	uint16_t Pin_0;
	uint16_t Pin_1;
	uint16_t Pin_2;
	uint16_t Pin_3;

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

	int8_t   Sequence ;

	uint32_t Time_Stamp;

} Motor_28BYJ_Struct_t;



void Motor_28BYJ_Run(Motor_28BYJ_Struct_t* Motor);
void Motor_28BYJ_Init(Motor_28BYJ_Struct_t* Motor);
void Motor_28BYJ_Stop(Motor_28BYJ_Struct_t* Motor);
void Motor_28BYJ_Set_RPM(Motor_28BYJ_Struct_t* Motor, uint16_t RPM);
void Motor_28BYJ_Move_To_Position(Motor_28BYJ_Struct_t* Motor, int32_t position);
void Motor_28BYJ_Move_Steps(Motor_28BYJ_Struct_t* Motor, int32_t steps);
void Motor_28BYJ_Move_Degrees(Motor_28BYJ_Struct_t* Motor, int32_t degrees);
void Motor_28BYJ_Run_All();
void Motor_28BYJ_Check_LMT_All();



#endif /* MOTOR_28BYJ_H_ */
