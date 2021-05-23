/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "leds.h"

#include <stdio.h>
#include "Logo.h"
#include "ssd1306.h"
//#include "buttons.h"
#include "hx711.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

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
	MX_I2C3_Init();
	/* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(OLED_VCC_GPIO_Port, OLED_VCC_Pin, SET);

	HAL_Delay(50);

	OLED_INIT();

	HX711 HX1;
	HX1.gpioData = DT_GPIO_Port;
	HX1.pinData = DT_Pin;
	HX1.gpioSck = SCK_GPIO_Port;
	HX1.pinSck = SCK_Pin;
	HX1.offset = 0;
	HX1.gain = 1;
	HX1.currentScale = 0;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	SSD1306_Draw_intrologo(Logo_GO_bmp);
	//SSD1306_DrawMenus(1);
	char HX_Value_Decimal[10];

	float knownWeight = 2000;
	int rawKnownWeight = 0;
	float caliFactor = 0;

	SSD1306_GotoXY(0, 0);
	SSD1306_Puts("Capturando", &Font_16x26, 1);
	SSD1306_GotoXY(0, 30);
	SSD1306_Puts("Zero", &Font_16x26, 1);
	SSD1306_UpdateScreen();

	SSD1306_Putc('.', &Font_16x26, 1);
	HAL_Delay(50);
	SSD1306_UpdateScreen();

	SSD1306_Putc('.', &Font_16x26, 1);
	HAL_Delay(50);
	SSD1306_UpdateScreen();

	SSD1306_Putc('.', &Font_16x26, 1);
	HAL_Delay(50);
	SSD1306_UpdateScreen();

	HX1.offset = HX711_Average_Value(HX1, 10);



	SSD1306_Clear();
	SSD1306_GotoXY(0, 0);
	SSD1306_Puts("Zero Obtido:", &Font_11x18, 1);

	sprintf(HX_Value_Decimal, "%d", HX1.offset);
	SSD1306_GotoXY(0, 30);
	SSD1306_Puts(HX_Value_Decimal, &Font_16x26, 1);
	SSD1306_UpdateScreen();

	while (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
		;
	HAL_Delay(150);

	while (1) {
		/* USER CODE END WHILE */

		SSD1306_Clear();
		SSD1306_GotoXY(12, 0);
		SSD1306_Puts("Insira", &Font_16x26, 1);
		SSD1306_GotoXY(25, 32);
		SSD1306_Puts("2 kg", &Font_16x26, 1);

		SSD1306_UpdateScreen();

		while (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
			;
		HAL_Delay(100);

		rawKnownWeight = HX711_Average_Value(HX1, 10);

		SSD1306_Clear();
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("Valor Obtido:", &Font_11x18, 1); //...51808

		sprintf(HX_Value_Decimal, "%d", rawKnownWeight);
		SSD1306_GotoXY(0, 30);
		SSD1306_Puts(HX_Value_Decimal, &Font_16x26, 1);
		SSD1306_UpdateScreen();

		while (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
			;
		HAL_Delay(100);

		caliFactor = (rawKnownWeight - HX1.offset) / knownWeight;
		SSD1306_Clear();

		sprintf(HX_Value_Decimal, "%f", caliFactor);
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("califactor:", &Font_11x18, 1);

		SSD1306_GotoXY(0, 30);
		SSD1306_Puts(HX_Value_Decimal, &Font_11x18, 1);
		SSD1306_UpdateScreen();

		while (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
			;
		HAL_Delay(100);

		SSD1306_Clear();

		while (1) {
			//SSD1306_Clear();
			HX1.currentScale =  (HX711_Average_Value(HX1, 10) - HX1.offset) / caliFactor;

			sprintf(HX_Value_Decimal, "%f", HX1.currentScale);
			SSD1306_GotoXY(0, 0);
			SSD1306_Puts("Gr:", &Font_11x18, 1);
			SSD1306_GotoXY(0, 30);
			SSD1306_Puts(HX_Value_Decimal, &Font_16x26, 1);
			SSD1306_UpdateScreen();

			/*while (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
			 {
			 HX1 = HX711_Tare(HX1, 10);
			 }
			 */
		}

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 80;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C3_Init(void) {

	/* USER CODE BEGIN I2C3_Init 0 */

	/* USER CODE END I2C3_Init 0 */

	/* USER CODE BEGIN I2C3_Init 1 */

	/* USER CODE END I2C3_Init 1 */
	hi2c3.Instance = I2C3;
	hi2c3.Init.ClockSpeed = 400000;
	hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c3.Init.OwnAddress1 = 0;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c3.Init.OwnAddress2 = 0;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C3_Init 2 */

	/* USER CODE END I2C3_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(OLED_VCC_GPIO_Port, OLED_VCC_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, LD_G_Pin | LD_O_Pin | LD_R_Pin | LD_B_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin : OLED_VCC_Pin */
	GPIO_InitStruct.Pin = OLED_VCC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(OLED_VCC_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : DT_Pin */
	GPIO_InitStruct.Pin = DT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(DT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SCK_Pin */
	GPIO_InitStruct.Pin = SCK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SCK_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD_G_Pin LD_O_Pin LD_R_Pin LD_B_Pin */
	GPIO_InitStruct.Pin = LD_G_Pin | LD_O_Pin | LD_R_Pin | LD_B_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
