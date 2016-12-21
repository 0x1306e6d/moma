#include "logger.h"

int lastPosition = 0;

void Logger_Configuration(void)
{
	LCD_Init();
	LCD_Clear(BLACK);
}

void Log(u8* message, ...)
{
	u8 buffer[100];
	va_list vl;

	va_start(vl, message);
	vsnprintf(buffer, sizeof(buffer), message, vl);
	va_end(vl);

	LCD_ShowString(0, lastPosition * 20, "                                   ",
	WHITE, BLACK);
	LCD_ShowString(0, lastPosition * 20, buffer, WHITE, BLACK);

	if (lastPosition > MAX_OUTPUT_SIZE)
	{
		lastPosition = 0;
	}
	else
	{
		lastPosition++;
	}
}

void LogAt(u16 y, u8* message, ...)
{
	u8 buffer[100];
	va_list vl;

	va_start(vl, message);
	vsnprintf(buffer, sizeof(buffer), message, vl);
	va_end(vl);
	LCD_ShowString(0, y * 20, "                                   ",
	WHITE, BLACK);
	LCD_ShowString(0, y * 20, buffer, WHITE, BLACK);
}
