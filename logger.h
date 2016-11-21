#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"
#include "lcd.h"

void initalizeLogger();
void log(u8* message, ...);

#endif
