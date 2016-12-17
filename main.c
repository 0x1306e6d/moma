#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "lcd.h"

#include "logger.h"
#include "timer.h"

// flash load "C:\Users\USER\Desktop\monday\motionMouse\Debug\motionMouse.axf"
// flash load "C:\Users\USER\Desktop\monday\motionMouse\flashclear.axf"

#define true 1
#define false 0
#define boolean uint8_t

#define HC_SR04_ECHO GPIO_Pin_8
#define HC_SR04_TRIGGER GPIO_Pin_9

void HC_SR04_Configuration(void);
void Request_HC_SR04(void);
void FlexSensor_Configurationr(void);
uint32_t GetLeftFlexSensorValue(void);
uint32_t GetLeftDefaultFlexSensorValue(void);
uint32_t GetRightFlexSensorValue(void);
uint32_t GetRightDefaultFlexSensorValue(void);

__IO uint32_t FlexSensorBuffer[2];
__IO uint32_t FlexDefaultValue[2];

uint32_t StartTime = 0;

int main() {
	boolean leftClicking = false;
	boolean rightClicking = false;

	SystemInit();

	initalizeLogger();
	Timer_Configuration();
	log("Timer configuration");

	FlexSensor_Configurationr();
	log("FlexSensor configuration");

	HC_SR04_Configuration();
	log("HC_SR04 configuration");
	Request_HC_SR04();

	while (true) {
//		log("left %d", GetLeftFlexSensorValue());
//		log("default left %d", GetLeftDefaultFlexSensorValue());
//		log("right %d", GetRightFlexSensorValue());
//		log("default right %d", GetRightDefaultFlexSensorValue());
		if (!leftClicking
				&& GetLeftFlexSensorValue()
						>= (GetLeftDefaultFlexSensorValue() * 1.1)) {
			leftClicking = true;
		}
		if (leftClicking
				&& GetLeftFlexSensorValue()
						>= (GetLeftDefaultFlexSensorValue() * 1.02)) {
			//log("Left light on");
			leftClicking = false;
		}

		if (!rightClicking
				&& GetRightFlexSensorValue()
						>= (GetRightDefaultFlexSensorValue() * 1.1)) {
			rightClicking = true;
		}
		if (rightClicking
				&& GetRightFlexSensorValue()
						>= (GetRightDefaultFlexSensorValue() * 1.02)) {
			//log("Right light on");
			rightClicking = false;
		}
	}
}

void HC_SR04_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// Echo : PC8
	GPIO_InitStructure.GPIO_Pin = HC_SR04_ECHO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Trigger : PC9
	GPIO_InitStructure.GPIO_Pin = HC_SR04_TRIGGER;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);

	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Request_HC_SR04(void) {
	GPIO_SetBits(GPIOC, HC_SR04_TRIGGER);
	DelayMilliSeconds(10);
	GPIO_ResetBits(GPIOC, HC_SR04_TRIGGER);
}

void FlexSensor_Configurationr() {
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// RCC configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) FlexSensorBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1)) {
		;
	}
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1)) {
		;
	}

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	FlexSensorBuffer[0] = 0;
	while (GetLeftFlexSensorValue() == 0) {
		;
	}
	FlexDefaultValue[0] = GetLeftFlexSensorValue();

	FlexSensorBuffer[1] = 0;
	while (GetRightFlexSensorValue() == 0) {
		;
	}
	FlexDefaultValue[1] = GetRightFlexSensorValue();

	log("Left : %d", FlexDefaultValue[0]);
	log("Right : %d", FlexDefaultValue[1]);
}

uint32_t GetLeftFlexSensorValue(void) {
	return FlexSensorBuffer[0];
}

uint32_t GetLeftDefaultFlexSensorValue(void) {
	return FlexDefaultValue[0];
}

uint32_t GetRightFlexSensorValue(void) {
	return FlexSensorBuffer[1];
}

uint32_t GetRightDefaultFlexSensorValue(void) {
	return FlexDefaultValue[1];
}

void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line8);
		log("Interrupt get time=%d", (GetCurrentTimeMillis() - StartTime));
		StartTime = 0;

		Request_HC_SR04();
	}
}
