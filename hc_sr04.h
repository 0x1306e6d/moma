#ifndef _HC_SR04_H
#define _HC_SR04_H

#include "common.h"
#include "logger.h"

#define HC_SR04_ECHO GPIO_Pin_8
#define HC_SR04_TRIGGER GPIO_Pin_9

void HC_SR04_Configuration(void);
void Request_HC_SR04(void);
void HC_SR04_Work(void);

#endif
