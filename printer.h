#ifndef _PRINTER_H
#define _PRINTER_H

#include "common.h"

void PrinterInit(void);
void Print(char* message, ...);
void PrintAt(u16 y, char* message, ...);

void PrintLeftEdge(uint32_t width, uint32_t height);
void PrintRightEdge(uint32_t width, uint32_t height);
void PrintLocation(uint32_t width, uint32_t height);
void PrintLocationValidate(boolean width, boolean height);

#endif
