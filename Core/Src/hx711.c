#include "hx711.h"
#include <stdio.h>
#include "ssd1306.h"

#include "fonts.h"

float leitura_LPF = 8508430;

void HX711_Init(HX711 data) {
	/*GPIO_InitTypeDef GPIO_InitStruct;
	 GPIO_InitStruct.Pin = data.pinSck;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(data.gpioSck, &GPIO_InitStruct);

	 GPIO_InitStruct.Pin = data.pinData;
	 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	 GPIO_InitStruct.Pull = GPIO_PULLUP; // <-----------***
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(data.gpioData, &GPIO_InitStruct);
	 */
	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(data.gpioData, data.pinSck, GPIO_PIN_RESET);

}

int HX711_Average_Value(HX711 data, uint8_t times) {
	int sum = 0;
	for (int i = 0; i < times; i++) {
		sum += HX711_Value(data);
	}

	return sum / times;
}

int HX711_Value(HX711 data) {

	for (char i = 0; i < 3; i++) {

		int buffer;
		buffer = 0;
		char launch = 0;

		while (HAL_GPIO_ReadPin(data.gpioData, data.pinData) == 1)
			;

		for (uint8_t i = 0; i < 24; i++) {
			HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);

			buffer = buffer << 1;

			if (HAL_GPIO_ReadPin(data.gpioData, data.pinData)) {
				buffer++;
			}

			HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);

		}

		for (int i = 0; i < data.gain; i++) {
			HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);
			HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);
		}

		buffer = buffer ^ 0x800000;
		buffer = buffer & 0xFFFE00;

		return buffer;

		//leitura_LPF += (buffer - leitura_LPF) * 0.5;
		//return (int) leitura_LPF;

		/*
		 if (launch > 2) {
		 leitura_LPF += (buffer - leitura_LPF) * 0.05;
		 return (int) leitura_LPF;
		 } else {

		 return buffer;
		 launch++;
		 }
		 */
	}

}

HX711 HX711_Tare(HX711 data, uint8_t times) {
	int sum = HX711_Average_Value(data, times);
	data.offset = sum;
	data.currentScale = data.currentScale - data.offset;
	return data;
}

