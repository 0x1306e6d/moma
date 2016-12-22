#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

#include "common.h"
#include "logger.h"

#define BLU_TX_PIN_1 GPIO_Pin_2
#define BLU_RX_PIN_1 GPIO_Pin_3
#define BLU_TX_PIN_2 GPIO_Pin_9
#define BLU_RX_PIN_2 GPIO_Pin_10

void Bluetooth_Configuration(void);

#endif
