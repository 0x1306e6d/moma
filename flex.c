#include "flex.h"
#include "printer.h"
#include "timer.h"
#include "button.h"
#include "bluetooth.h"

void PressLeftMouse(void);
void ReleaseLeftMouse(void);
void PressRightMouse(void);
void ReleaseRightMouse(void);

boolean IsIndexFoldEvent(void);
boolean IsIndexStreekEvent(void);
boolean IsMiddleFoldEvent(void);
boolean IsMiddleStreekEvent(void);
uint32_t GetIndexFlexValue(void);
uint32_t GetIndexFlexDefaultValue(void);
void SetIndexFlexDefaultValue(uint32_t value);
uint32_t GetIndexFoldThreshold(void);
void SetIndexFoldThreshold(uint32_t value);

uint32_t GetMiddleFlexValue(void);
uint32_t GetMiddleFlexDefaultValue(void);
void SetMiddleFlexDefaultValue(uint32_t value);
uint32_t GetMiddleFoldThreshold(void);
void SetMiddleFoldThreshold(uint32_t value);

__IO uint32_t FlexSensorValue[2];
__IO uint32_t FlexSensorDefaultValue[2];
__IO uint32_t FlexSensorFoldThreshold[2];

boolean m_index_folding = false;
boolean m_middle_folding = false;

void FlexSensorInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// GPIO Configuration
	GPIO_InitStructure.GPIO_Pin = FLEX_INDEX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(FLEX_INDEX_GPIO_TYPE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FLEX_MIDDLE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(FLEX_MIDDLE_GPIO_TYPE, &GPIO_InitStructure);

	// DMA Configuration
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) FlexSensorValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);

	// ADC Configuration
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
	{
		;
	}
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
	{
		;
	}
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void StartFlexSensorCalibration(void)
{
	uint32_t value = 0;

	// Left Default Value
	PrintAt(1, "Are you stretch your second finger?");
	while (!IsButtonClicking(BUTTON1))
	{
		value = GetIndexFlexValue();
		PrintAt(2, "flex value : %d", value);
		DelayMillis(100);
	}
	SetIndexFlexDefaultValue(value);
	PrintAt(2, "FlexLeftDefault : %d", GetIndexFlexDefaultValue());
	DelayMillis(1000);
	value = 0;

	// Left Fold Threshold
	PrintAt(3, "Are you fold your second finger?");
	while (!IsButtonClicking(BUTTON1))
	{
		value = GetIndexFlexValue();
		PrintAt(4, "flex value : %d", value);
		DelayMillis(100);
	}
	SetIndexFoldThreshold(value);
	PrintAt(4, "FlexLeftThreshold : %d", GetIndexFoldThreshold());
	DelayMillis(1000);
	value = 0;

	// Right Default Value
	PrintAt(1, "Are you stretch your third finger");
	while (!IsButtonClicking(BUTTON1))
	{
		value = GetMiddleFlexValue();
		PrintAt(2, "flex value : %d", value);
		DelayMillis(100);
	}
	SetMiddleFlexDefaultValue(value);
	PrintAt(2, "FlexRightDefault : %d", GetMiddleFlexDefaultValue());
	DelayMillis(1000);
	value = 0;

	// Right Fold Threshold
	PrintAt(3, "Are you fold your third finger?");
	while (!IsButtonClicking(BUTTON1))
	{
		value = GetMiddleFlexValue();
		PrintAt(4, "flex value : %d", value);
		DelayMillis(100);
	}
	SetMiddleFoldThreshold(value);
	PrintAt(4, "FlexRightThreshold : %d", GetMiddleFoldThreshold());
	DelayMillis(1000);
	value = 0;

}

void HandleFlexSensor(void)
{
	// Fold index finger
	if (!m_index_folding && IsIndexFoldEvent())
	{
		PressLeftMouse();
		m_index_folding = true;
	}
	// Streek index finger
	if (m_index_folding && IsIndexStreekEvent())
	{
		ReleaseLeftMouse();
		m_index_folding = false;
	}

	// Fold middle finger
	if (!m_middle_folding && IsMiddleFoldEvent())
	{
		PressRightMouse();
		m_middle_folding = true;
	}
	// Streek middle finger
	if (m_middle_folding && IsMiddleStreekEvent())
	{
		ReleaseRightMouse();
		m_middle_folding = false;
	}
}

void PressLeftMouse(void)
{
	Print("PressLeft at %d", GetCurrentTimeMillis());
	BluetoothWriteString("p l\r\n");
}

void ReleaseLeftMouse(void)
{
	Print("ReleaseLeft at %d", GetCurrentTimeMillis());
	BluetoothWriteString("r l\r\n");
}

void PressRightMouse(void)
{
	Print("PressRight at %d", GetCurrentTimeMillis());
	BluetoothWriteString("p r\r\n");
}

void ReleaseRightMouse(void)
{
	Print("ReleaseRight at %d", GetCurrentTimeMillis());
	BluetoothWriteString("r r\r\n");
}

boolean IsIndexFoldEvent(void)
{
	uint32_t indexFlexValue = GetIndexFlexValue();
	return indexFlexValue != 0 && (indexFlexValue <= GetIndexFoldThreshold());
}

boolean IsIndexStreekEvent(void)
{
	uint32_t indexFlexValue = GetIndexFlexValue();
	return indexFlexValue != 0
			&& (indexFlexValue >= (GetIndexFlexDefaultValue() * 0.9));
}

boolean IsMiddleFoldEvent(void)
{
	uint32_t middleFlexValue = GetMiddleFlexValue();
	return middleFlexValue != 0 && (middleFlexValue <= GetMiddleFoldThreshold());
}

boolean IsMiddleStreekEvent(void)
{
	uint32_t middleFlexValue = GetMiddleFlexValue();
	return middleFlexValue != 0
			&& (middleFlexValue >= (GetMiddleFlexDefaultValue() * 0.9));
}

uint32_t GetIndexFlexValue(void)
{
	return FlexSensorValue[0];
}

uint32_t GetIndexFlexDefaultValue(void)
{
	return FlexSensorDefaultValue[0];
}

void SetIndexFlexDefaultValue(uint32_t value)
{
	FlexSensorDefaultValue[0] = value;
}

uint32_t GetIndexFoldThreshold(void)
{
	return FlexSensorFoldThreshold[0];
}

void SetIndexFoldThreshold(uint32_t value)
{
	FlexSensorFoldThreshold[0] = value;
}

uint32_t GetMiddleFlexValue(void)
{
	return FlexSensorValue[1];
}

uint32_t GetMiddleFlexDefaultValue(void)
{
	return FlexSensorDefaultValue[1];
}

void SetMiddleFlexDefaultValue(uint32_t value)
{
	FlexSensorDefaultValue[1] = value;
}

uint32_t GetMiddleFoldThreshold(void)
{
	return FlexSensorFoldThreshold[1];
}

void SetMiddleFoldThreshold(uint32_t value)
{
	FlexSensorFoldThreshold[1] = value;
}
