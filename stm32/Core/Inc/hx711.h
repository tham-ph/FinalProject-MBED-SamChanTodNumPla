#ifndef HX711_H_
#define HX711_H_

#include "stm32f4xx_hal.h"

typedef struct _hx711
{
	GPIO_TypeDef* gpioSck;
	GPIO_TypeDef* gpioData;
	uint16_t pinSck;
	uint16_t pinData;
	int offset;
	int gain;
	int gramRatio;
	// 1: channel A, gain factor 128
	// 2: channel B, gain factor 32
    // 3: channel A, gain factor 64
} hx711_t;


void HX711_Init(hx711_t *hx711, GPIO_TypeDef* gpioSck, uint16_t pinSck, GPIO_TypeDef* gpioData, uint16_t pinData, int gain);
void HX711_Tare(hx711_t *hx711, uint8_t times);
int HX711_Value(hx711_t *hx711);
int HX711_AverageValue(hx711_t *hx711, uint8_t times);
int HX711_GetWeight(hx711_t *hx711);
void HX711_Calibrate(hx711_t *hx711, int gramRatio);

#endif /* HX711_H_ */
