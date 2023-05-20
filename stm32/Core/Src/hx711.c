#include "hx711.h"

void HX711_Init(hx711_t *hx711, GPIO_TypeDef* gpioSck, uint16_t pinSck, GPIO_TypeDef* gpioData, uint16_t pinData, int gain)
{
	hx711->gpioSck = gpioSck;
	hx711->pinSck = pinSck;
	hx711->gpioData = gpioData;
	hx711->pinData = pinData;
	hx711->gramRatio = 1;
	hx711->gain = gain;

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = hx711->pinSck;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(hx711->gpioSck, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = hx711->pinData;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(hx711->gpioData, &GPIO_InitStruct);

//	HAL_GPIO_WritePin(hx711.gpioSck, hx711.pinSck, GPIO_PIN_SET);
//	HAL_Delay(50);
//	HAL_GPIO_WritePin(hx711.gpioSck, hx711.pinSck, GPIO_PIN_RESET);

}

int HX711_AverageValue(hx711_t *hx711, uint8_t times)
{
    int sum = 0;
    for (int i = 0; i < times; i++)
    {
        sum += HX711_Value(hx711);
    }

    return sum / times;
}

int HX711_Value(hx711_t *hx711)
{
    int buffer;
    buffer = 0;

    while (HAL_GPIO_ReadPin(hx711->gpioData, hx711->pinData)==1);

    for (uint8_t i = 0; i < 24; i++)
    {
    	HAL_GPIO_WritePin(hx711->gpioSck, hx711->pinSck, GPIO_PIN_SET);

        buffer = buffer << 1 ;

        if (HAL_GPIO_ReadPin(hx711->gpioData, hx711->pinData))
        {
            buffer ++;
        }

        HAL_GPIO_WritePin(hx711->gpioSck, hx711->pinSck, GPIO_PIN_RESET);
    }

    for (int i = 0; i < hx711->gain; i++)
    {
    	HAL_GPIO_WritePin(hx711->gpioSck, hx711->pinSck, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(hx711->gpioSck, hx711->pinSck, GPIO_PIN_RESET);
    }


    buffer = buffer ^ 0x800000;

    return buffer;
}

int HX711_GetWeight(hx711_t *hx711) {
	int averageValue = HX711_AverageValue(hx711, 10);
	return (8227020 - averageValue) / hx711->gramRatio;
}

void HX711_Tare(hx711_t *hx711, uint8_t times)
{
    int sum = HX711_AverageValue(hx711, times);
    hx711->offset = sum;
}

void HX711_Calibrate(hx711_t *hx711, int gramRatio) {
	hx711->gramRatio = gramRatio;
}


