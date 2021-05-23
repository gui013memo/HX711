/*
 * leds.c
 *
 *  Created on: May 20, 2021
 *      Author: guilh
 */

#include "main.h"

void leds_blink_all(int delay) {
	HAL_GPIO_TogglePin(LD_B_GPIO_Port, LD_B_Pin);
	HAL_Delay(delay);

	HAL_GPIO_TogglePin(LD_R_GPIO_Port, LD_R_Pin);
	HAL_Delay(delay);

	HAL_GPIO_TogglePin(LD_O_GPIO_Port, LD_O_Pin);
	HAL_Delay(delay);

	HAL_GPIO_TogglePin(LD_G_GPIO_Port, LD_G_Pin);
	HAL_Delay(delay);
}
