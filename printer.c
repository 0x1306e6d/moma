#include <stdarg.h>
#include "printer.h"
#include "lcd.h"

#define MAX_OUTPUT_SIZE 5
#define CLEAR "                                   "

int m_last_position = 0;

void PrinterInit(void)
{
	LCD_Init();
	LCD_Clear(BLACK);
}

void Print(char* message, ...)
{
	u8 buffer[100];
	va_list vl;

	va_start(vl, message);
	vsnprintf(buffer, sizeof(buffer), message, vl);
	va_end(vl);

	LCD_ShowString(0, m_last_position * 20, CLEAR, WHITE, BLACK);
	LCD_ShowString(0, m_last_position * 20, buffer, WHITE, BLACK);

	if (m_last_position >= MAX_OUTPUT_SIZE)
	{
		m_last_position = 0;
	}
	else
	{
		m_last_position++;
	}
}

void PrintAt(u16 y, char* message, ...)
{
	u8 buffer[100];
	va_list vl;

	va_start(vl, message);
	vsnprintf(buffer, sizeof(buffer), message, vl);
	va_end(vl);

	LCD_ShowString(0, y * 20, CLEAR, WHITE, BLACK);
	LCD_ShowString(0, y * 20, buffer, WHITE, BLACK);
}

void PrintLeftEdge(uint32_t width, uint32_t height)
{
	PrintAt(7, "left edge x: %d y: %d", width, height);
}

void PrintRightEdge(uint32_t width, uint32_t height)
{
	PrintAt(8, "right edge x: %d y: %d", width, height);
}

void PrintLocation(uint32_t width, uint32_t height)
{
	PrintAt(10, "location x: %d y: %d", width, height);
}

void PrintLocationValidate(boolean width, boolean height)
{
	PrintAt(9, "valid width: %d, height: %d", width, height);
}
