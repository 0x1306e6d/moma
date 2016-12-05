#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"

void Timer_Configuration(void);
uint32_t GetCurrentTimeMillis(void);

#endif
