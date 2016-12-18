#ifndef _TIMER_H
#define _TIMER_H

#include "common.h"
#include "logger.h"

void Timer_Configuration(void);
void DelayMilliSeconds(uint32_t time);
uint32_t GetCurrentTimeMillis(void);

#endif
