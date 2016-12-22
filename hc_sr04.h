#ifndef _HC_SR04_H
#define _HC_SR04_H

#include "common.h"
#include "logger.h"
#include "timer.h"

#define HC_SR04_ECHO_LR GPIO_Pin_4
#define HC_SR04_ECHO_TB GPIO_Pin_12
#define HC_SR04_TRIGGER_LR GPIO_Pin_5
#define HC_SR04_TRIGGER_TB GPIO_Pin_13

void HC_SR04_Configuration(void);
void Request_HC_SR04_LR(void);
void Request_HC_SR04_TB(void);

#endif
