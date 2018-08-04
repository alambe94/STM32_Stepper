/*
 * motor_28byj.c
 *
 *  Created on: 11-Apr-2018
 *      Author: medprime
 */

#include"motor_28byj.h"

#define MAX_MOTORS_28BYJ 5

static Motor_28BYJ_Struct_t* Motor_28BYJ_Struct_PTR_Array[MAX_MOTORS_28BYJ];
static uint8_t Attached_Motors_28BYJ = 0;

void Motor_28BYJ_Init(Motor_28BYJ_Struct_t* Motor) {
	GPIO_InitTypeDef GPIO_Init_Strut;

	GPIO_Init_Strut.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init_Strut.Speed = GPIO_SPEED_FREQ_MEDIUM;

	Motor->Port->BRR = (Motor->Pin_0 | Motor->Pin_1 | Motor->Pin_2
			| Motor->Pin_3); //reset

	GPIO_Init_Strut.Pin = (Motor->Pin_0 | Motor->Pin_1 | Motor->Pin_2
			| Motor->Pin_3);
	HAL_GPIO_Init(Motor->Port, &GPIO_Init_Strut);

	GPIO_Init_Strut.Mode = GPIO_MODE_INPUT;

	if (Motor->Limit_Pin_Logic == RESET) {
		GPIO_Init_Strut.Pull = GPIO_PULLUP;
	} else {
		GPIO_Init_Strut.Pull = GPIO_PULLDOWN;
	}

	GPIO_Init_Strut.Pin = Motor->Limit_Pin;
	HAL_GPIO_Init(Motor->Port, &GPIO_Init_Strut);

	if (Motor->RPM != 0) {
		Motor->Step_Delay = ((1000 * 1000 * 60)
				/ (Motor->RPM * Motor->Steps_For_360)); /*(us_in_minute / RPM*Steps_For_360)  */
	} else {
		Motor->Step_Delay = 1000; //default
	}
	Motor->Current_Position = 0;
	Motor->Step_Delay_Complete_Flag = RESET;
	Motor->Limit_Flag = RESET;
	Motor->Running_Flag = RESET;
	Motor->Sequence = 0;
	Motor->Steps_To_Move = 0;
	Motor->Time_Stamp = 0;

	if (Attached_Motors_28BYJ >= MAX_MOTORS_28BYJ) {
		_Error_Handler(__FILE__, __LINE__);		//Error
	}

	Motor_28BYJ_Struct_PTR_Array[Attached_Motors_28BYJ] = Motor;
	Attached_Motors_28BYJ++;
}

void Motor_28BYJ_Step(Motor_28BYJ_Struct_t* Motor) {
	uint16_t BRR_temp = 0; //reset
	uint16_t BSRR_temp = 0; //set

	switch (Motor->Sequence) {
	case 0: {
		BSRR_temp = Motor->Pin_0;
		BRR_temp = Motor->Pin_1 | Motor->Pin_2 | Motor->Pin_3;
		break;
	}
	case 1: {
		BSRR_temp = Motor->Pin_0 | Motor->Pin_1;
		BRR_temp = Motor->Pin_2 | Motor->Pin_3;
		break;
	}
	case 2: {
		BSRR_temp = Motor->Pin_1;
		BRR_temp = Motor->Pin_0 | Motor->Pin_2 | Motor->Pin_3;
		break;
	}
	case 3: {
		BSRR_temp = Motor->Pin_1 | Motor->Pin_2;
		BRR_temp = Motor->Pin_0 | Motor->Pin_3;
		break;
	}
	case 4: {
		BSRR_temp = Motor->Pin_2;
		BRR_temp = Motor->Pin_0 | Motor->Pin_1 | Motor->Pin_3;
		break;
	}
	case 5: {
		BSRR_temp = Motor->Pin_2 | Motor->Pin_3;
		BRR_temp = Motor->Pin_0 | Motor->Pin_1;
		break;
	}
	case 6: {
		BSRR_temp = Motor->Pin_3;
		BRR_temp = Motor->Pin_0 | Motor->Pin_1 | Motor->Pin_2;
		break;
	}
	case 7: {
		BSRR_temp = Motor->Pin_0 | Motor->Pin_3;
		BRR_temp = Motor->Pin_1 | Motor->Pin_2;
		break;
	}
	default: {
		BSRR_temp = 0;
		BRR_temp = Motor->Pin_0 | Motor->Pin_1 | Motor->Pin_2 | Motor->Pin_3;
		break;
	}
	}

	Motor->Port->BRR = BRR_temp;
	Motor->Port->BSRR = BSRR_temp;

}

void Motor_28BYJ_Move_CW(Motor_28BYJ_Struct_t* Motor) {
	Motor->Sequence++;
	if (Motor->Sequence > 7) {
		Motor->Sequence = 0;
	}
	Motor_28BYJ_Step(Motor);

	Motor->Current_Position++;
}

void Motor_28BYJ_Move_CCW(Motor_28BYJ_Struct_t* Motor) {
	Motor->Sequence--;
	if (Motor->Sequence < 0) {
		Motor->Sequence = 7;
	}
	Motor_28BYJ_Step(Motor);

	Motor->Current_Position--;
}

void Motor_28BYJ_Stop(Motor_28BYJ_Struct_t* Motor) {
	Motor->Port->BRR = Motor->Pin_0 | Motor->Pin_1 | Motor->Pin_2
			| Motor->Pin_3;
	Motor->Running_Flag = RESET;
}

void Motor_28BYJ_Set_RPM(Motor_28BYJ_Struct_t* Motor, uint16_t RPM) {
	Motor->RPM = RPM;
	if (Motor->RPM != 0) {
		Motor->Step_Delay = ((1000 * 1000 * 60)
				/ (Motor->RPM * Motor->Steps_For_360)); /*(us in minute / RPM*Steps_For_360)  */
	}
}

void Motor_28BYJ_Run(Motor_28BYJ_Struct_t* Motor) {

	if (Get_us() - (Motor->Time_Stamp) >= Motor->Step_Delay) {
		Motor->Time_Stamp = Get_us();

		if (Motor->Running_Flag == SET) {
			if (Motor->Steps_To_Move > 0) {
				Motor_28BYJ_Move_CW(Motor);
				Motor->Steps_To_Move--;
			} else if (Motor->Steps_To_Move < 0) {
				Motor_28BYJ_Move_CCW(Motor);
				Motor->Steps_To_Move++;
			} else {
				Motor_28BYJ_Stop(Motor);
			}
		} else {
			Motor_28BYJ_Stop(Motor);
		}

	}
}

void Motor_28BYJ_Check_LMT(Motor_28BYJ_Struct_t* Motor) {
	if (HAL_GPIO_ReadPin(Motor->Port, Motor->Limit_Pin)
			== Motor->Limit_Pin_Logic) {
		Motor->Limit_Flag = SET;
	} else {
		Motor->Limit_Flag = RESET;
	}
}

void Motor_28BYJ_Move_Steps(Motor_28BYJ_Struct_t* Motor, int32_t Steps) {

	Motor->Steps_To_Move += Steps;

	Motor->Running_Flag = SET;
}

void Motor_28BYJ_Move_Degrees(Motor_28BYJ_Struct_t* Motor, int32_t Degrees) {

	Motor->Steps_To_Move += Degrees * (Motor->Steps_For_360) / 360;

	Motor->Running_Flag = SET;
}

void Motor_28BYJ_Move_To_Position(Motor_28BYJ_Struct_t* Motor, int32_t Position) {

	int32_t tempo = Position - Motor->Current_Position;

	Motor->Steps_To_Move += tempo;

	Motor->Running_Flag = SET;
}

void Motor_28BYJ_Run_All() {

	if (Attached_Motors_28BYJ >= MAX_MOTORS_28BYJ) {
		_Error_Handler(__FILE__, __LINE__);		//Error
	}

	Motor_28BYJ_Struct_t* PTR = NULL;

	for (uint8_t i = 0; i < Attached_Motors_28BYJ; i++) {
		PTR = Motor_28BYJ_Struct_PTR_Array[i];

		if (Get_us() - (PTR->Time_Stamp) >= PTR->Step_Delay) {
			PTR->Time_Stamp = Get_us();

			if (PTR->Running_Flag == SET) {
				if (PTR->Steps_To_Move > 0) {
					Motor_28BYJ_Move_CW(PTR);
					PTR->Steps_To_Move--;
				} else if (PTR->Steps_To_Move < 0) {
					Motor_28BYJ_Move_CCW(PTR);
					PTR->Steps_To_Move++;
				} else {
					Motor_28BYJ_Stop(PTR);
				}
			} else {
				Motor_28BYJ_Stop(PTR);
			}

		}
	}

}

void Motor_28BYJ_Check_LMT_All() {

	if (Attached_Motors_28BYJ >= MAX_MOTORS_28BYJ) {
		_Error_Handler(__FILE__, __LINE__);		//Error
	}

	Motor_28BYJ_Struct_t* PTR = NULL;

	for (uint8_t i = 0; i < Attached_Motors_28BYJ; i++) {

		PTR = Motor_28BYJ_Struct_PTR_Array[i];

		if (HAL_GPIO_ReadPin(PTR->Port, PTR->Limit_Pin)
				== PTR->Limit_Pin_Logic) {
			PTR->Limit_Flag = SET;
		} else {
			PTR->Limit_Flag = RESET;
		}
	}
}
