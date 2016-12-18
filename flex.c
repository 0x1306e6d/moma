#include "flex.h"

__IO uint32_t FlexSensorBuffer[2];
__IO uint32_t FlexDefaultValue[2];

void FlexSensor_Configuration(void) {
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	Log("Start FlexSensor Configuration");

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	Log("FlexSensor RCC Configure");

	// DMA Configuration
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
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
	Log("FlexSensor DMA Configure");

	// ADC Configuration
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	Log("FlexSensor ADC Configure");

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1)) {
		;
	}
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1)) {
		;
	}
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	// Calibration
	FlexSensorBuffer[0] = 0;
	while (FlexSensorBuffer[0] == 0) {
		;
	}
	FlexDefaultValue[0] = FlexSensorBuffer[0];
	Log("FlexLeft : %d", FlexDefaultValue[0]);

	FlexSensorBuffer[1] = 0;
	while (FlexSensorBuffer[1] == 0) {
		;
	}
	FlexDefaultValue[1] = FlexSensorBuffer[1];
	Log("FlexRight : %d", FlexDefaultValue[1]);

	Log("Finish FlexSensor Configuration");
}

uint32_t GetLeftFlexSensorValue(void) {
	return FlexSensorBuffer[0];
}

uint32_t GetLeftDefaultFlexSensorValue(void) {
	return FlexDefaultValue[0];
}

uint32_t GetRightFlexSensorValue(void) {
	return FlexSensorBuffer[1];
}

uint32_t GetRightDefaultFlexSensorValue(void) {
	return FlexDefaultValue[1];
}
