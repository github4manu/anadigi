/*
 * UserCode.h
 *
 *  Created on: 31 May 2016
 *      Author: manu
 */

#ifndef USERCODE_H_
#define USERCODE_H_

#include "Arduino.h"

void userSetup();
//void userLoopFast();
uint8_t userLoopSlow(int16_t rcDataAuxInput);
void userLoop1Hz();

#endif /* USERCODE_H_ */

