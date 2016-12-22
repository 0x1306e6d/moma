#include "common.h"
#include "logger.h"
#include "timer.h"
#include "flex.h"
#include "hc_sr04.h"
#include "button.h"
#include "bluetooth.h"

// flash load "C:\Users\USER\Desktop\monday\motionMouse\Debug\motionMouse.axf"
// flash load "D:\monday\motionMouse\Debug\motionMouse.axf"
// flash load "C:\Users\USER\Desktop\monday\motionMouse\flashclear.axf"

void doLeftClick(void);
void doRightClick(void);
void MoveMouse(uint32_t x, uint32_t y);
void ReceiveUSART2(void);
void Start_HC_SR04_Initializer(void);

uint32_t m_screen_height = 1020;
uint32_t m_screen_width = 1980;
uint32_t m_height_left_edge = 0;
uint32_t m_width_left_edge = 0;
uint32_t m_height_right_edge = 0;
uint32_t m_width_right_edge = 0;

uint32_t m_distance_lr = 0;
uint32_t m_distance_lr_before = 0;
uint32_t m_distance_tb = 0;
uint32_t m_distance_tb_before = 0;

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
	Start_HC_SR04_Initializer();

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
			if (m_distance_lr_before == 0)
			{
				m_distance_lr_before = m_distance_lr;
			}
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
			if (m_distance_tb_before == 0)
			{
				m_distance_tb_before = m_distance_tb;
			}
		}
		LogAt(13, "HC_SR04 TB : %d", m_distance_tb);

		if (((m_distance_lr < (m_distance_lr_before * 1.5))
				&& (m_distance_lr > (m_distance_lr_before * 0.5)))
				&& ((m_distance_tb < (m_distance_tb_before * 1.5))
						&& (m_distance_tb > (m_distance_tb_before * 0.5))))
		{
			uint32_t x = ((double) (m_distance_lr - m_width_left_edge) / (double) (m_width_right_edge - m_width_left_edge) * m_screen_width);
			uint32_t y = ((double) (m_distance_tb - m_height_left_edge) / (double) (m_height_right_edge - m_height_left_edge) * m_screen_height);

			MoveMouse(x, y);
			m_distance_lr_before = m_distance_lr;
			m_distance_tb_before = m_distance_tb;
		}

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

void MoveMouse(uint32_t x, uint32_t y)
{
	// USART_WriteString(USART2, "m %d %d", x, y);
	Log("Move Mouse %d %d", x, y);
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

void Start_HC_SR04_Initializer(void)
{
	uint32_t value_tb = 0;
	uint32_t value_lr = 0;

	LogAt(1, "Are you left top edge?");
	while (!IsButton1Clicking())
	{
		value_tb = 0;
		value_lr = 0;

		// Top-Bottom HC_SR04
		Request_HC_SR04_TB();
		while (GPIO_ReadInputDataBit(GPIOD, HC_SR04_ECHO_TB) == Bit_RESET)
		{
			;
		}
		while (GPIO_ReadInputDataBit(GPIOD, HC_SR04_ECHO_TB) == Bit_SET)
		{
			value_tb++;
		}
		if (value_tb > 0)
		{
			LogAt(2, "top-bottom : %d", value_tb);
		}

		// Left-Right HC_SR04
		Request_HC_SR04_LR();
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO_LR) == Bit_RESET)
		{
			;
		}
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO_LR) == Bit_SET)
		{
			value_lr++;
		}
		if (value_lr > 0)
		{
			LogAt(3, "left-right : %d", value_lr);
		}

		DelayMilliSeconds(100);
	}

	m_height_left_edge = value_tb;
	m_width_left_edge = value_lr;
	LogAt(4, "left x:%d", m_width_left_edge);
	LogAt(5, "left y:%d", m_height_left_edge);

	DelayMilliSeconds(1000);
	
	LogAt(1, "Are you right bottom edge?");
	while (!IsButton1Clicking())
	{
		value_tb = 0;
		value_lr = 0;

		// Top-Bottom HC_SR04
		Request_HC_SR04_TB();
		while (GPIO_ReadInputDataBit(GPIOD, HC_SR04_ECHO_TB) == Bit_RESET)
		{
			;
		}
		while (GPIO_ReadInputDataBit(GPIOD, HC_SR04_ECHO_TB) == Bit_SET)
		{
			value_tb++;
		}
		if (value_tb > 0)
		{
			LogAt(2, "top-bottom : %d", value_tb);
		}

		// Left-Right HC_SR04
		Request_HC_SR04_LR();
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO_LR) == Bit_RESET)
		{
			;
		}
		while (GPIO_ReadInputDataBit(GPIOC, HC_SR04_ECHO_LR) == Bit_SET)
		{
			value_lr++;
		}
		if (value_lr > 0)
		{
			LogAt(3, "left-right : %d", value_lr);
		}

		DelayMilliSeconds(100);
	}

	m_height_right_edge = value_tb;
	m_width_right_edge = value_lr;
	LogAt(4, "right x:%d", m_width_right_edge);
	LogAt(5, "right y:%d", m_height_right_edge);
}
