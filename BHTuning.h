/*
 * BHTuing.h
 *
 *  Created on: 31 May 2016
 *      Author: manu
 */

#ifndef BHTUNING_H_
#define BHTUNING_H_

#include "Arduino.h"

void bhTune_setup();
uint8_t bhTune_loopSlow(int16_t rcDataAuxInput);
void bhTune_save();

uint8_t bhTune_clampUserVal(float fParamVal);
void bhTune_setConfVal(uint8_t iParamVal, uint8_t iTuneKey);

#define BH_TUNE

#endif /* BHTUNING_H_ */
