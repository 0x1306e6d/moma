#include "common.h"
#include "logger.h"
#include "timer.h"
#include "flex.h"
#include "hc_sr04.h"
#include "button.h"
#include "bluetooth.h"

// flash load "C:\Users\USER\Desktop\monday\motionMouse\Debug\motionMouse.axf"
// flash load "C:\Users\USER\Desktop\monday\motionMouse\flashclear.axf"

void doLeftClick(void);
void doRightClick(void);
void ReceiveUSART2(void);

uint32_t m_screen_height = 0;
uint32_t m_screen_width = 0;
uint32_t m_distance_lr = 0;
uint32_t m_distance_tb = 0;

char usart2_buffer[100];
uint32_t usart2_buffer_index = 0;

int main()
{
	uint32_t distance_lr = 0;
	uint32_t distance_tb = 0;
	boolean clicking_left = false;
	boolean clicking_right = false;

	SystemInit();
	Logger_Configuration();
	Timer_Configuration();
	Button_Configuration();

	Bluetooth_Configuration();
	HC_SR04_Configuration();
	FlexSensor_Configuration();

	Start_FlexSensor_Initializer();

	while (true)
	{
		distance_lr = 0;
		distance_tb = 0;

		// Left Click
		if (clicking_left && IsLeftClickEnd())
		{
			doLeftClick();
			clicking_left = false;
		}
		if (!clicking_left && IsLeftClickStart())
		{
			clicking_left = true;
		}

		// Right Click
		if (clicking_right && IsRightClickEnd())
		{
			doRightClick();
			clicking_right = false;
		}
		if (!clicking_right && IsRightClickStart())
		{
			clicking_right = true;
		}

		// Left-Right HC_SR04
		Request_HC_SR04_LR();
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO_LR) == Bit_RESET)
		{
			;
		}
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO_LR) == Bit_SET)
		{
			distance_lr++;
		}
		if (distance_lr > 0)
		{
			m_distance_lr = distance_lr;
		}
		LogAt(11, "HC_SR04 LR : %d", m_distance_lr);

		// Top-Bottom HC_SR04
		Request_HC_SR04_TB();
		while (GPIO_ReadInputDataBit(GPIOD, HC_SR04_ECHO_TB) == Bit_RESET)
		{
			;
		}
		while (GPIO_ReadInputDataBit(GPIOD, HC_SR04_ECHO_TB) == Bit_SET)
		{
			distance_tb++;
		}
		if (distance_tb > 0)
		{
			m_distance_tb = distance_tb;
		}
		LogAt(13, "HC_SR04 TB : %d", m_distance_tb);

		DelayMilliSeconds(33);
	}
}

void doLeftClick(void)
{
//	USART_WriteString(USART2, "c l");
	Log("Left click at %d", GetCurrentTimeMillis());
}

void doRightClick(void)
{
//	USART_WriteString(USART2, "c r");
	Log("Right click at %d", GetCurrentTimeMillis());
}

void USART1_IRQHandler(void)
{
	char c;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		c = (char) USART_ReceiveData(USART1);

		USART_SendData(USART2, c);
		WaitForTransmissionComplete(USART2);

		Log("USART1 [%c]", c);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void USART2_IRQHandler(void)
{
	char c;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		c = (char) USART_ReceiveData(USART2);

		USART_SendData(USART1, c);
		WaitForTransmissionComplete(USART1);

		Log("USART2 [%c]", c);

//		usart2_buffer[usart2_buffer_index] = c;
//		usart2_buffer_index++;
//		if (c == 0 || c == '\n')
//		{
//			ReceiveUSART2();
//			usart2_buffer_index = 0;
//		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

void ReceiveUSART2(void)
{
	char opcode = usart2_buffer[0];
	usart2_buffer[usart2_buffer_index] = 0;
	Log("recv2:%s", usart2_buffer);

	if (opcode == 'x')
	{
		m_screen_width = atoi(usart2_buffer + 2);
		Log("Width : %d", m_screen_width);
	}
	else if (opcode == 'y')
	{
		m_screen_height = atoi(usart2_buffer + 2);
		Log("Height : %d", m_screen_height);
	}
}
