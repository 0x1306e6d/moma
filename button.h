#ifndef _BUTTON_H
#define _BUTTON_H

#include "common.h"

#define BUTTON1 GPIO_Pin_11

void ButtonInit(void);
boolean IsButtonClicking(uint16_t button);

#endif
