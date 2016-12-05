#include "logger.h"
#include "timer.h"

int lastPosition = 0;

void initalizeLogger() {
	LCD_Init();
	LCD_Clear(BLACK);
}

void log(u8* message, ...) {
	u8 buffer[100];

	va_list vl;
	va_start(vl, message);

	if (lastPosition > MAX_OUTPUT_SIZE) {
		lastPosition = 0;
	}

	vsnprintf(buffer, sizeof(buffer), message, vl);
	va_end(vl);

	LCD_ShowString(30, lastPosition * 20, "                    ", WHITE,
	BLACK);
	LCD_ShowNum(0, lastPosition * 20, GetCurrentTimeMillis(), 30, WHITE, BLACK);
	LCD_ShowString(30, lastPosition * 20, buffer, WHITE, BLACK);
	lastPosition++;
}
