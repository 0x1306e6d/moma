#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

#include "common.h"

#define USART1_TX_PIN GPIO_Pin_9
#define USART1_TX_TYPE GPIOA
#define USART1_RX_PIN GPIO_Pin_10
#define USART1_RX_TYPE GPIOA

#define USART2_TX_PIN GPIO_Pin_2
#define USART2_TX_TYPE GPIOA
#define USART2_RX_PIN GPIO_Pin_3
#define USART2_RX_TYPE GPIOA

void BluetoothInit(void);
void BluetoothWriteString(char* message, ...);

#endif
