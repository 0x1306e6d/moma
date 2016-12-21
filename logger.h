#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "common.h"
#include "lcd.h"

#define MAX_OUTPUT_SIZE 5

void Logger_Configuration(void);
void Log(u8* message, ...);
void LogAt(u16 y, u8* message, ...);

#endif
