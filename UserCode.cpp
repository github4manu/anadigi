/*
 * UserCode.cpp
 *
 *  Created on: 31 May 2016
 *      Author: manu
 */

#include "config.h"
#include "UserCode.h"
#include "BHTuning.h"

// USER CODE HOOKS (BH)

void userSetup() {
   #ifdef BH_TUNE
      bhTune_setup();
   #endif
}

//void userLoopFast() {}

uint8_t userLoopSlow(int16_t rcDataAuxInput) {
   #ifdef BH_TUNE
      bhTune_loopSlow(rcDataAuxInput);
   #endif
}

void userLoop1Hz() {
   #ifdef BH_TUNE
      bhTune_save();
   #endif
}




