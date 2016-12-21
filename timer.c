#include "timer.h"

void Timer_Configuration()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	RCC_LSEConfig(RCC_LSE_ON);
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	RTC_WaitForLastTask();
	RTC_SetPrescaler(32); // period : 1ms
	RTC_WaitForLastTask();
}

void DelayMilliSeconds(uint32_t time)
{
	uint32_t current = GetCurrentTimeMillis();
	while (GetCurrentTimeMillis() < (current + time))
	{
		;
	}
}

uint32_t GetCurrentTimeMillis()
{
	return RTC_GetCounter();
}
