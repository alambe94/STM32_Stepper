/*
 * motor_a4988.c
 *
 *  Created on: 11-Apr-2018
 *      Author: medprime
 */

#include "motor_a4988.h"

#define MAX_MOTORS_A4988 5

static Motor_A4988_Struct_t* Motor_A4988_Struct_PTR_Array[MAX_MOTORS_A4988];
static uint8_t Attached_Motors_A4988 = 0;

void Motor_A4988_Init(Motor_A4988_Struct_t* Motor)
{
	GPIO_InitTypeDef GPIO_Init_Strut;

	GPIO_Init_Strut.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init_Strut.Speed = GPIO_SPEED_FREQ_MEDIUM;

	Motor->Port->BRR = (Motor->DIR_Pin | Motor->EN_Pin); //reset
	Motor->Port->BSRR = Motor->EN_Pin; //set EN  active low

	GPIO_Init_Strut.Pin = (Motor->STP_Pin | Motor->DIR_Pin | Motor->EN_Pin);
	HAL_GPIO_Init(Motor->Port, &GPIO_Init_Strut);

	GPIO_Init_Strut.Mode = GPIO_MODE_INPUT;

	if (Motor->Limit_Pin_Logic == RESET)
	{
		GPIO_Init_Strut.Pull = GPIO_PULLUP;
	}
	else
	{
		GPIO_Init_Strut.Pull = GPIO_PULLDOWN;
	}

	GPIO_Init_Strut.Pin = Motor->Limit_Pin;
	HAL_GPIO_Init(Motor->Port, &GPIO_Init_Strut);

	if (Motor->RPM != 0)
	{
		Motor->Step_Delay = ((1000 * 1000 * 60)
				/ (Motor->RPM * Motor->Steps_For_360)); /*(us_in_minute / RPM*Steps_For_360)  */
	}
	else
	{
		Motor->Step_Delay = 1000; //default
	}
	Motor->Current_Position = 0;
	Motor->Step_Delay_Complete_Flag = RESET;
	Motor->Limit_Flag = RESET;
	Motor->Running_Flag = RESET;
	Motor->Steps_To_Move = 0;
	Motor->Time_Stamp = 0;

	if (Attached_Motors_A4988 >= MAX_MOTORS_A4988)
	{
		_Error_Handler(__FILE__, __LINE__);		//Error
	}

	Motor_A4988_Struct_PTR_Array[Attached_Motors_A4988] = Motor;
	Attached_Motors_A4988++;
}

void Motor_A4988_Step(Motor_A4988_Struct_t* Motor)
{
	Motor->Port->BSRR = (Motor->STP_Pin); //set
	//Delay_us(1);
	Motor->Port->BRR = (Motor->STP_Pin); //reset
	//Motor->Port->ODR ^= (Motor->STP_Pin); //toggle
}

void Motor_A4988_Move_CW(Motor_A4988_Struct_t* Motor)
{
	Motor->Port->BSRR = Motor->DIR_Pin; //set dir

	Motor_A4988_Step(Motor);

	Motor->Current_Position++;
}

void Motor_A4988_Move_CCW(Motor_A4988_Struct_t* Motor)
{
	Motor->Port->BRR = Motor->DIR_Pin; //reset dir

	Motor_A4988_Step(Motor);

	Motor->Current_Position--;
}

void Motor_A4988_Stop(Motor_A4988_Struct_t* Motor)
{
	Motor->Port->BRR = Motor->STP_Pin;
	Motor->Running_Flag = RESET;
}

void Motor_A4988_Set_RPM(Motor_A4988_Struct_t* Motor, uint16_t RPM)
{
	Motor->RPM = RPM;
	if (Motor->RPM != 0)
	{
		Motor->Step_Delay = ((1000 * 1000 * 60) / (RPM * Motor->Steps_For_360)); /*(us_in_minute / RPM*Steps_For_360)  */
	}
}

void Motor_A4988_Run(Motor_A4988_Struct_t* Motor)
{

	if (Get_us() - (Motor->Time_Stamp) >= Motor->Step_Delay)
	{
		Motor->Time_Stamp = Get_us();

		if (Motor->Running_Flag == SET)
		{
			if (Motor->Steps_To_Move > 0)
			{
				Motor_A4988_Move_CW(Motor);
				Motor->Steps_To_Move--;
			}
			else if (Motor->Steps_To_Move < 0)
			{
				Motor_A4988_Move_CCW(Motor);
				Motor->Steps_To_Move++;
			}
			else
			{
				Motor_A4988_Stop(Motor);
				Motor_A4988_Disable(Motor);
			}
		}
		else
		{
			Motor_A4988_Stop(Motor);
			Motor_A4988_Disable(Motor);
		}

	}
}

void Motor_A4988_Check_LMT(Motor_A4988_Struct_t* Motor)
{
	if (HAL_GPIO_ReadPin(Motor->Port, Motor->Limit_Pin)
			== Motor->Limit_Pin_Logic)
	{
		Motor->Limit_Flag = SET;
	}
	else
	{
		Motor->Limit_Flag = RESET;
	}
}

void Motor_A4988_Move_Steps(Motor_A4988_Struct_t* Motor, int32_t Steps)
{

	Motor->Steps_To_Move = Steps;

	if (Motor->Steps_To_Move != 0)
	{
		Motor->Running_Flag = SET;

		Motor_A4988_Enable(Motor);
	}

}

void Motor_A4988_Move_Degrees(Motor_A4988_Struct_t* Motor, int32_t Degrees)
{

	Motor->Steps_To_Move = Degrees * (Motor->Steps_For_360) / 360;

	if (Motor->Steps_To_Move != 0)
	{
		Motor->Running_Flag = SET;

		Motor_A4988_Enable(Motor);
	}

}

void Motor_A4988_Move_To_Position(Motor_A4988_Struct_t* Motor, int32_t Position)
{

	int32_t tempo = Position - Motor->Current_Position;

	Motor->Steps_To_Move = tempo;

	if (Motor->Steps_To_Move != 0)
	{
		Motor->Running_Flag = SET;

		Motor_A4988_Enable(Motor);
	}

}

void Motor_A4988_Enable(Motor_A4988_Struct_t* Motor)
{
	Motor->Port->BRR = Motor->EN_Pin; //reset EN  active low
}

void Motor_A4988_Disable(Motor_A4988_Struct_t* Motor)
{
	Motor->Port->BSRR = Motor->EN_Pin; //set EN  active low
}

void Motor_A4988_Run_All()
{
	Motor_A4988_Struct_t* PTR = NULL;

	for (uint8_t i = 0; i < Attached_Motors_A4988; i++)
	{
		PTR = Motor_A4988_Struct_PTR_Array[i];

		if (Get_us() - (PTR->Time_Stamp)
				>= PTR->Step_Delay)
		{
			PTR->Time_Stamp = Get_us();

			if (PTR->Running_Flag == SET)
			{
				if (PTR->Steps_To_Move > 0)
				{
					Motor_A4988_Move_CW(PTR);
					PTR->Steps_To_Move--;
				}
				else if (PTR->Steps_To_Move < 0)
				{
					Motor_A4988_Move_CCW(PTR);
					PTR->Steps_To_Move++;
				}
				else
				{
					Motor_A4988_Stop(PTR);
					Motor_A4988_Disable(PTR);
				}
			}
			else
			{
				Motor_A4988_Stop(PTR);
				Motor_A4988_Disable(PTR);
			}
		}

	}
}

void Motor_A4988_Check_LMT_All()
{
	Motor_A4988_Struct_t* PTR = NULL;

	for (uint8_t i = 0; i < Attached_Motors_A4988; i++)
	{
		PTR = Motor_A4988_Struct_PTR_Array[i];

		if (HAL_GPIO_ReadPin(PTR->Port,
				PTR->Limit_Pin)
				== PTR->Limit_Pin_Logic)
		{
			PTR->Limit_Flag = SET;
		}
		else
		{
			PTR->Limit_Flag = RESET;
		}
	}
}
