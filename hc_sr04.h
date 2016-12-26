#ifndef _HC_SR04_H
#define _HC_SR04_H

#include "common.h"

#define HC_SR04_ECHO_LR GPIO_Pin_4
#define HC_SR04_ECHO_TB GPIO_Pin_12
#define HC_SR04_TRIGGER_LR GPIO_Pin_5
#define HC_SR04_TRIGGER_TB GPIO_Pin_13

#define SONAR_WIDTH_ECHO_PIN GPIO_Pin_4
#define SONAR_WIDTH_ECHO_TYPE GPIOC
#define SONAR_WIDTH_TRIGGER_PIN GPIO_Pin_5
#define SONAR_WIDTH_TRIGGER_TYPE GPIOC

#define SONAR_HEIGHT_ECHO_PIN GPIO_Pin_12
#define SONAR_HEIGHT_ECHO_TYPE GPIOD
#define SONAR_HEIGHT_TRIGGER_PIN GPIO_Pin_13
#define SONAR_HEIGHT_TRIGGER_TYPE GPIOD

void SonarSensorInit(void);
void StartSonarSensorCalibration(void);
void HandleSonarSensor(void);

#endif
