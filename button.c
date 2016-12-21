#include "button.h"

void Button_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

boolean IsButton1Clicking(void)
{
	return !((*(volatile unsigned *) 0x40011408) & 0x00000800);
}
