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

void RTC_Configuration(void);
void GPIO_Configuration(void);
void configureFlexSensor(void);

uint32_t Read_Distance(void);
void delay(int i);

__IO uint32_t FlexSensorBuffer;
uint32_t defaultValue;

int main() {
	int i = 1000000;
	boolean clicking = false;
	uint32_t TimeVar;
	uint32_t TMM = 0, TSS = 0, TMSS = 0;
	boolean waiting = false;
	boolean listening = false;
	uint32_t listenTime = 0, delayTime = 0;

	SystemInit();
	initalizeLogger();

	configureFlexSensor();

	Timer_Configuration();
	log("Timer configuration");

	GPIO_Configuration();
	log("GPIO configure");

	while (i--) {
		;
	}
	defaultValue = FlexSensorBuffer;

	while (true) {
		if (clicking && FlexSensorBuffer <= (defaultValue * 1.02)) {
			log("light on");
			clicking = false;
		}
		if (clicking == false && FlexSensorBuffer >= (defaultValue * 1.1)) {
			clicking = true;
		}

		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		delay(100);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		log("start waiting");

		i = 1000000;
		while (i--) {
			if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == SET) {
				log("SET");
			}
		}

//		i = Read_Distance();
//		log("ReadDistance: %d", i);
//		if (!waiting) {
//			GPIO_SetBits(GPIOC, GPIO_Pin_9);
//			GPIO_ResetBits(GPIOC, GPIO_Pin_7);
//			delay(100);
//			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
//			waiting = true;
//			log("start waiting");
//		}
//		if (waiting) {
//			while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == SET) {
//				if (listenTime == 0) {
//					listenTime = RTC_GetCounter();
//					log("listenTime: %d", listenTime);
//				}
//			}
//			delayTime = RTC_GetCounter() - listenTime;
//			log("delayTime: %d", delayTime);
//			waiting = false;
//			listenTime = 0;
//			delayTime = 0;
//			delay(1000000);
//			log("start next sensor");
//		}
//		log("DataBit: %d", GPIO_ReadInputDataBit(GPIOE, GPIO_PinSource1));
//		if (waiting && !listening
//				&& GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == SET) {
//			listenTime = RTC_GetCounter();
//			listening = true;
//			log("start listening.");
//			log("listenTime: %d", listenTime);
//		}
//		if (listening && GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == RESET) {
//			delayTime = RTC_GetCounter() - listenTime;
//			log("delayTime: %d", delayTime);
//			listening = false;
//			waiting = false;
//			delay(1000000);
//			log("start next sensor");
//		}
	}

	return 0;
}

uint32_t Read_Distance(void) {
	__IO uint32_t disTime = 10;
	__IO uint8_t flag = 0;
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	delay(10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_9);

	while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == SET) {
		disTime++;
	}

	return disTime;
}

void GPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// Echo : PC7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Trigger : PC9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void configureFlexSensor() {
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// RCC configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &FlexSensorBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
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
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
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
}

void RTC_Configuration(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	RCC_LSEConfig(RCC_LSE_ON);
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	RTC_WaitForLastTask();
	RTC_SetPrescaler(32); // period : 1ms
	RTC_WaitForLastTask();
}

void delay(int i) {
	while (i--) {
		;
	}
}
