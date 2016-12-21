#ifndef _HC_SR04_H
#define _HC_SR04_H

#include "common.h"
#include "logger.h"
#include "timer.h"

#define HC_SR04_ECHO GPIO_Pin_8
#define HC_SR04_TRIGGER GPIO_Pin_9
#define HC_SR04_ECHO_TOP GPIO_Pin_12
#define HC_SR04_TRIGGER_TOP GPIO_Pin_13

void HC_SR04_Configuration(void);
void Request_HC_SR04(void);
void Request_HC_SR04_Top(void);

#endif
