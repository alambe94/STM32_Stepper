
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "motor_28byj.h"
#include "motor_a4988.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
Motor_28BYJ_Struct_t Motor_X, Motor_Y;
Motor_A4988_Struct_t Motor_Z;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void Motor_X_Config(void)
{
	Motor_X.Port = GPIOA;
	Motor_X.Limit_Pin = GPIO_PIN_4;
	Motor_X.Pin_0 = GPIO_PIN_0;
	Motor_X.Pin_1 = GPIO_PIN_1;
	Motor_X.Pin_2 = GPIO_PIN_2;
	Motor_X.Pin_3 = GPIO_PIN_3;

	Motor_X.Limit_Pin_Logic=RESET;//low when pressed

	Motor_X.RPM = 10;
	Motor_X.Steps_For_360 = 4096;

	Motor_28BYJ_Init(&Motor_X);
}

void Motor_X_Move_To_Position(int16_t Position)
{
	Motor_28BYJ_Move_To_Position(&Motor_X, Position);
}

void Motor_X_Move_Steps(int16_t Steps)
{
	Motor_28BYJ_Move_Steps(&Motor_X, Steps);
}

void Motor_X_Move_Degrees(int16_t degrees)
{
	Motor_28BYJ_Move_Degrees(&Motor_X, degrees);
}

void Motor_X_Set_RPM(uint8_t RPM)
{
	Motor_28BYJ_Set_RPM(&Motor_X, RPM);
}

void Motor_X_Run()
{
	Motor_28BYJ_Run(&Motor_X);
}

void Motor_Y_Config(void)
{
	Motor_Y.Port = GPIOB;
	Motor_Y.Limit_Pin = GPIO_PIN_11;
	Motor_Y.Pin_0 = GPIO_PIN_12;
	Motor_Y.Pin_1 = GPIO_PIN_13;
	Motor_Y.Pin_2 = GPIO_PIN_14;
	Motor_Y.Pin_3 = GPIO_PIN_15;

	Motor_Y.Limit_Pin_Logic=RESET;//low when pressed

	Motor_Y.RPM = 10;
	Motor_Y.Steps_For_360 = 4096;

	Motor_28BYJ_Init(&Motor_Y);
}

void Motor_Y_Move_To_Position(int16_t Position)
{
	Motor_28BYJ_Move_To_Position(&Motor_Y, Position);
}

void Motor_Y_Move_Steps(int16_t Steps)
{
	Motor_28BYJ_Move_Steps(&Motor_Y, Steps);
}

void Motor_Y_Move_Degrees(int16_t degrees)
{
	Motor_28BYJ_Move_Degrees(&Motor_Y, degrees);
}

void Motor_Y_Set_RPM(uint8_t RPM)
{
	Motor_28BYJ_Set_RPM(&Motor_Y, RPM);
}

void Motor_Y_Run()
{
	Motor_28BYJ_Run(&Motor_Y);
}



void Motor_Z_Config(void)
{
	Motor_Z.Port      = GPIOB;
	Motor_Z.Limit_Pin = GPIO_PIN_2;
	Motor_Z.DIR_Pin   = GPIO_PIN_0;
	Motor_Z.STP_Pin   = GPIO_PIN_1;
	Motor_Z.EN_Pin    = GPIO_PIN_10;

	Motor_Z.Limit_Pin_Logic=RESET;//low when pressed
	Motor_Z.RPM = 100;
	Motor_Z.Steps_For_360 = 200*16;//

	Motor_A4988_Init(&Motor_Z);
}

void Motor_Z_Move_To_Position(int16_t Position)
{
	Motor_A4988_Move_To_Position(&Motor_Z, Position);
}

void Motor_Z_Move_Steps(int16_t Steps)
{
	Motor_A4988_Move_Steps(&Motor_Z, Steps);
}

void Motor_Z_Move_Degrees(int16_t degrees)
{
	Motor_A4988_Move_Degrees(&Motor_Z, degrees);
}

void Motor_Z_Set_RPM(uint16_t RPM)
{
	Motor_A4988_Set_RPM(&Motor_Z, RPM);
}

void Motor_Z_Run()
{
	Motor_A4988_Run(&Motor_Z);
}
void Motor_Z_Enable()
{
	Motor_A4988_Enable(&Motor_Z);
}


void HAL_SYSTICK_Callback()
{

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	Motor_X_Config();
	Motor_Y_Config();
	Motor_Z_Config();

	Motor_X_Set_RPM(5);
	Motor_Y_Set_RPM(5);
	Motor_Z_Set_RPM(5);


	//Motor_X_Move_Degrees(360);
	//Motor_Y_Move_Degrees(-360);

	static int32_t temp1 = 360*50;

	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);

	//Motor_Z_Move_Steps(200*16);
	//Motor_Y_Move_Steps(4096);
	//Motor_X_Move_Steps(4096);




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

		static uint32_t time_stamp=0;

		if (HAL_GetTick()-time_stamp>10)
		{
			static int32_t temp = 0;

			time_stamp=HAL_GetTick();
			temp = TIM4->CNT;
			Motor_X_Move_Steps(temp);
			TIM4->CNT=0;

			temp = TIM3->CNT;
			Motor_Y_Move_Steps(temp);
			TIM3->CNT=0;

			temp = TIM2->CNT;
			Motor_Z_Move_Steps(temp);
			TIM2->CNT=0;
		}
		/*
		//Motor_Z_Move_Steps(4096);
		if(Motor_Z.Running_Flag == RESET)
		{
			temp1=(-1)*temp1;
		    Motor_Z_Move_Degrees(temp1);
		    HAL_Delay(10);
		}
        */
		Motor_28BYJ_Run_All();
		Motor_A4988_Run_All();


	}
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
