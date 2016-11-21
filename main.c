#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_adc.h"
#include "misc.h"
#include "lcd.h"
#include "Touch.h"
#include "stm32f10x_dma.h"

//flash load "C:\Users\USER\Desktop\term\Debug\term.axf"
//void usart1_interrupt_init(void);

uint32_t Read_Distance(void);
void delay();
int j;
int main() {

	uint32_t i;
	__IO uint32_t ADC_DualConvertedValueTab[2];
	GPIO_InitTypeDef GPIO_InitStructure;

	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//입력 맞음
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //echo
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  //tri
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	LCD_Init();
	Touch_Configuration();

	LCD_Clear(WHITE);

	while (1) {
		i = Read_Distance();
		if (i > 10)
			j = i;
		LCD_ShowNum(10, 10, j, 8, BLACK, WHITE);
		delay(5000);
	}
}

uint32_t Read_Distance(void) {

	__IO uint32_t disTime = 10;
	__IO uint8_t flag = 0;
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	delay(10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_9);

	//GPI//O_ResetBits(GPIOC,)

	while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == SET) {
		disTime++;
		delay(10);
	}

	return disTime;
}

void delay(int t) {
	int i;
	for (i = 0; i < t; i++)
		;
}

/*
 #include <stdio.h>

 #include "stm32f10x.h"
 #include "stm32f10x_rcc.h"
 #include "stm32f10x_gpio.h"
 #include "stm32f10x_dma.h"
 #include "stm32f10x_adc.h"
 #include "stm32f10x_usart.h"
 #include "misc.h"
 #include "lcd.h"

 #include "logger.h"

 // flash load "C:\Users\USER\Desktop\monday\motionMouse\flashclear.axf"
 // flash load "C:\Users\USER\Desktop\monday\motionMouse\Debug\motionMouse.axf"

 void configureRCC(void);
 void configureGPIO(void);
 void sleep(int period);

 int main() {
 uint16_t value = 0;

 SystemInit();
 LCD_Init();
 configureRCC();
 configureGPIO();

 while (1) {
 GPIO_SetBits(GPIOC, GPIO_Pin_9);
 GPIO_ResetBits(GPIOC, GPIO_Pin_7);
 sleep(10);
 GPIO_ResetBits(GPIOC, GPIO_Pin_9);

 while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == SET) {
 value++;
 sleep(10);
 }
 LCD_ShowNum(10, 10, value, 8, WHITE, BLACK);
 //		log("value : %d", value);
 value = 0;

 sleep(1000000);
 }
 return 0;
 }

 void configureRCC(void) {
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 }

 void configureGPIO(void) {
 GPIO_InitTypeDef GPIO_InitStructure;

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOC, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 }

 void sleep(int period) {
 while (period--) {
 ;
 }
 }
 */
