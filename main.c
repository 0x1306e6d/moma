#include "common.h"
#include "logger.h"
#include "timer.h"
#include "flex.h"

// flash load "C:\Users\USER\Desktop\monday\motionMouse\Debug\motionMouse.axf"
// flash load "C:\Users\USER\Desktop\monday\motionMouse\flashclear.axf"

#define HC_SR04_ECHO GPIO_Pin_8
#define HC_SR04_TRIGGER GPIO_Pin_9

void HC_SR04_Configuration(void);
void Request_HC_SR04(void);

int main() {
	SystemInit();
	Logger_Configuration();

	Timer_Configuration();
	FlexSensor_Configuration();

	HC_SR04_Configuration();
	Request_HC_SR04();

	while (true) {

	}
}

void HC_SR04_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	Log("Start HC_SR04 Configuration");

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	Log("HC_SR04 RCC Configure");

	// Echo : PC8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	Log("HC_SR04 ECHO Configure");

	// Trigger : PC9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	Log("HC_SR04 TRIGGER Configure");

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);

	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	Log("HC_SR04 EXTI Configure");

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	Log("HC_SR04 NVIC Configure");

	Log("End HC_SR04 Configuration");
}

void Request_HC_SR04(void) {
	LogAt(12, "Request HC_SR04 at %d", GetCurrentTimeMillis());
	GPIO_SetBits(GPIOC, HC_SR04_TRIGGER);
	DelayMilliSeconds(10);
	GPIO_ResetBits(GPIOC, HC_SR04_TRIGGER);
}

void EXTI9_5_IRQHandler(void) {
	uint32_t distance = 0;

	if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO) == Bit_SET) {
			distance++;
		}
		if (distance > 0) {
			LogAt(13, "HC_SR04. Distance=%d", distance);
		}
		EXTI_ClearITPendingBit(EXTI_Line8);

		Request_HC_SR04();
	}
}
