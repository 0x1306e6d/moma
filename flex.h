#ifndef _FLEX_H
#define _FLEX_H

#include "common.h"
#include "logger.h"

void FlexSensor_Configuration(void);
uint32_t GetLeftFlexSensorValue(void);
uint32_t GetLeftDefaultFlexSensorValue(void);
uint32_t GetRightFlexSensorValue(void);
uint32_t GetRightDefaultFlexSensorValue(void);

#endif
