#include "button.h"

void ButtonInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

boolean IsButtonClicking(uint16_t button)
{
	return !((*(volatile unsigned *) 0x40011408) & button);
}
