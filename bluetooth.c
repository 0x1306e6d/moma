#include <stdarg.h>
#include "bluetooth.h"
#include "printer.h"

void WaitForTransmissionComplete(USART_TypeDef* USARTx);

void BluetoothInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// GPIO Configuration
	// USART Rx
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_RX_TYPE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_TYPE, &GPIO_InitStructure);

	// USART Tx
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_TX_TYPE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_TYPE, &GPIO_InitStructure);

	// USART Configuration
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = (USART_Mode_Tx | USART_Mode_Rx);

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = (USART_Mode_Tx | USART_Mode_Rx);

	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	// NVIC Configuration
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_ClearPendingIRQ(USART1_IRQn);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_ClearPendingIRQ(USART2_IRQn);
}

void BluetoothWriteString(char *message, ...)
{
	int i;
	char buffer[30];
	va_list vl;

	va_start(vl, message);
	vsnprintf(buffer, sizeof(buffer), message, vl);
	va_end(vl);

	for (i = 0; buffer[i] != 0; i++)
	{
		USART_SendData(USART2, buffer[i]);
		WaitForTransmissionComplete(USART2);
	}

	Print("WriteString : %s", buffer);
}

void WaitForTransmissionComplete(USART_TypeDef *USARTx)
{
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
	{
		;
	}
}

void USART1_IRQHandler(void)
{
	char c;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		c = (char) USART_ReceiveData(USART1);

		USART_SendData(USART2, c);
		WaitForTransmissionComplete(USART2);

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

		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
