#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"
#include "lcd.h"

#define MAX_OUTPUT_SIZE 15

void initalizeLogger();
void log(u8* message, ...);

#endif
