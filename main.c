#include "common.h"
#include "logger.h"
#include "timer.h"
#include "flex.h"
#include "hc_sr04.h"

// flash load "C:\Users\USER\Desktop\monday\motionMouse\Debug\motionMouse.axf"
// flash load "C:\Users\USER\Desktop\monday\motionMouse\flashclear.axf"

uint32_t m_distance = 0;
uint32_t m_distance_top = 0;

int main() {
	uint32_t distance = 0;
	uint32_t distance_top = 0;
	SystemInit();
	Logger_Configuration();

	Timer_Configuration();
	FlexSensor_Configuration();

	HC_SR04_Configuration();

	while (true) {
		distance = 0;
		distance_top = 0;

		Request_HC_SR04_Top();
		while (GPIO_ReadInputDataBit(GPIOD, HC_SR04_ECHO_TOP) == Bit_RESET) {
			;
		}
		while (GPIO_ReadInputDataBit(GPIOD, HC_SR04_ECHO_TOP) == Bit_SET) {
			distance_top++;
		}
		if (distance_top > 0) {
			m_distance_top = distance_top;
		}
		LogAt(11, "HC_SR04_TOP : %d", m_distance_top);

		Request_HC_SR04();
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO) == Bit_RESET) {
			;
		}
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO) == Bit_SET) {
			distance++;
		}
		if (distance > 0) {
			m_distance = distance;
		}
		LogAt(14, "HC_SR04 : %d", m_distance);

		DelayMilliSeconds(33);
	}
}
