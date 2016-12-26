#ifndef _TIMER_H
#define _TIMER_H

#include "common.h"

void TimerInit(void);
uint32_t GetCurrentTimeMillis(void);

void DelayMillis(uint32_t time);

#endif
