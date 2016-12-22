#include "flex.h"

__IO uint32_t FlexSensorBuffer[2];
__IO uint32_t FlexDefaultValue[2];
__IO uint32_t FlexClickThreshold[2];

uint32_t GetLeftClickThreshold(void);
void SetLeftClickThreshold(uint32_t value);
uint32_t GetLeftDefaultFlexSensorValue(void);
void SetLeftDefaultFlexSensorValue(uint32_t value);

uint32_t GetRightClickThreshold(void);
void SetRightClickThreshold(uint32_t value);
uint32_t GetRightDefaultFlexSensorValue(void);
void SetRightDefaultFlexSensorValue(uint32_t value);

void FlexSensor_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// GPIO Configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// DMA Configuration
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) FlexSensorBuffer;
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

void Start_FlexSensor_Initializer(void)
{
	uint32_t value = 0;

	// Left Default Value
	LogAt(1, "Are you stretch your second finger?");
	while (!IsButton1Clicking())
	{
		value = GetLeftFlexSensorValue();
		LogAt(2, "flex value : %d", value);
		DelayMilliSeconds(100);
	}
	SetLeftDefaultFlexSensorValue(value);
	LogAt(2, "FlexLeftDefault : %d", GetLeftDefaultFlexSensorValue());
	DelayMilliSeconds(1000);
	value = 0;

	// Left Fold Threshold
	LogAt(3, "Are you fold your second finger?");
	while (!IsButton1Clicking())
	{
		value = GetLeftFlexSensorValue();
		LogAt(4, "flex value : %d", value);
		DelayMilliSeconds(100);
	}
	SetLeftClickThreshold(value);
	LogAt(4, "FlexLeftThreshold : %d", GetLeftClickThreshold());
	DelayMilliSeconds(1000);
	value = 0;

	// Right Default Value
	LogAt(1, "Are you stretch your third finger");
	while (!IsButton1Clicking())
	{
		value = GetRightFlexSensorValue();
		LogAt(2, "flex value : %d", value);
		DelayMilliSeconds(100);
	}
	SetRightDefaultFlexSensorValue(value);
	LogAt(2, "FlexRightDefault : %d", GetRightDefaultFlexSensorValue());
	DelayMilliSeconds(1000);
	value = 0;

	// Right Fold Threshold
	LogAt(3, "Are you fold your third finger?");
	while (!IsButton1Clicking())
	{
		value = GetRightFlexSensorValue();
		LogAt(4, "flex value : %d", value);
		DelayMilliSeconds(100);
	}
	SetRightClickThreshold(value);
	LogAt(4, "FlexRightThreshold : %d", GetRightClickThreshold());
	DelayMilliSeconds(1000);
	value = 0;

}

uint32_t GetLeftFlexSensorValue(void)
{
	return FlexSensorBuffer[0];
}

uint32_t GetRightFlexSensorValue(void)
{
	return FlexSensorBuffer[1];
}

boolean IsLeftClickStart(void)
{
	return GetLeftFlexSensorValue() != 0
			&& GetLeftFlexSensorValue() <= GetLeftClickThreshold();
}

boolean IsLeftClickEnd(void)
{
	return GetLeftFlexSensorValue() != 0
			&& GetLeftFlexSensorValue()
					>= (GetLeftDefaultFlexSensorValue() * 0.9);
}

boolean IsRightClickStart(void)
{
	return GetRightFlexSensorValue() != 0
			&& GetRightFlexSensorValue() <= GetRightClickThreshold();
}

boolean IsRightClickEnd(void)
{
	return GetRightFlexSensorValue() != 0
			&& GetRightFlexSensorValue()
					>= (GetRightDefaultFlexSensorValue() * 0.9);
}

uint32_t GetLeftClickThreshold(void)
{
	return FlexClickThreshold[0];
}

void SetLeftClickThreshold(uint32_t value)
{
	FlexClickThreshold[0] = value;
}

uint32_t GetLeftDefaultFlexSensorValue(void)
{
	return FlexDefaultValue[0];
}

void SetLeftDefaultFlexSensorValue(uint32_t value)
{
	FlexDefaultValue[0] = value;
}

uint32_t GetRightClickThreshold(void)
{
	return FlexClickThreshold[1];
}

void SetRightClickThreshold(uint32_t value)
{
	FlexClickThreshold[1] = value;
}

uint32_t GetRightDefaultFlexSensorValue(void)
{
	return FlexDefaultValue[1];
}

void SetRightDefaultFlexSensorValue(uint32_t value)
{
	FlexDefaultValue[1] = value;
}
