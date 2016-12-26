#ifndef _FLEX_H
#define _FLEX_H

#include "common.h"

#define FLEX_INDEX_GPIO_PIN GPIO_Pin_5
#define FLEX_INDEX_GPIO_TYPE GPIOA

#define FLEX_MIDDLE_GPIO_PIN GPIO_Pin_6
#define FLEX_MIDDLE_GPIO_TYPE GPIOA

void FlexSensorInit(void);
void StartFlexSensorCalibration(void);
void HandleFlexSensor(void);

#endif
