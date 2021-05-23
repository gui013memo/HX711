

 /*
 ******************************************************************************
 * @Projeto        : SP1[Sistema_de_Pesagem]
 * @file           : Metodo de calibração e parametrização via HX711 24-bit ADC 
 *                   with amplifier (x32, x64, x128)
 * @Data           : 21/05/2021
 ******************************************************************************
*/

//####


typedef struct _hx711   
{
	GPIO_TypeDef* gpioSck;
	GPIO_TypeDef* gpioData;
	uint16_t pinSck;
	uint16_t pinData;
	int offset;
	int gain;
	float currentScale;
	// 1: channel A, gain factor 128
	// 2: channel B, gain factor 32
    // 3: channel A, gain factor 64
} HX711;



int HX711.offset = 0; // Valor adquirido com a plataforma sem peso, é o ZERO da balança.

int pesoConhecidoCru = 0; // valor obtido do HX711 com o peso conhecido sobre a balança.

float Calibration_Factor = 0;// Fator de calibracao 

int pesoConhecidoGrNominal = 2000; // Valor nominal do peso que sera utilizado 


Calibration_Factor = (HX711.offset - pesoConhecidoCru) /  2000;  


//////////////////////////////////   EXEMPLO   /////////////////////////////////////////////////
/*
 * 1 - Célula de carga utilizada:
 * Marca Alfa Instrumentos  
 * Modelo C-20
 * Sensitividade = 1.998 mV/v 
 *
 * 2 -
 *
 *
 *
 *

 = 8509440

2kg = 8943616
     

      califactor = 217.087997

*/
