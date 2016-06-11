/*
 * BHTuning.cpp
 *
 *  Created on: 31 May 2016
 *      Author: manu
 */

/**
 * PID POTENTIOMETER TUNING (BH)
 * tune PID values using TX potentiometer
 *
   Maximum PID values (for reference when setting your range)
   -------------------------------------
   Param       P       I        D
   -------------------------------------
   ROLL        20.0    0.250    100
   PITCH       20.0    0.250    100
   YAW         20.0    0.250    100
   PIDALT      20.0    0.250    100
   PIDPOS       2.5    2.5
   PIDPOSR     25.0    2.50       0.250
   PIDNAVR     25.0    2.50       0.250
   PIDLEVEL    20.0    0.250    100
   PIDMAG      20.0
 */

#include "BHTuning.h"


//multiwii variables
conf_t conf;
//int16_t rcData[RC_CHANS];    // interval [1000;2000]



//#ifdef BH_TUNE
	BHTUNINGclass::BHTUNINGclass() {
		_bhTune_iKey = PITCH;
		_bhTune_iParam = P;
		_bhTune_iValRange[0] = 2;
		_bhTune_iValRange[1] = 4;
		_bhTune_iInputCh = AUX1;
		_bhTune_bLockPitchRoll = 0;
	}

	uint8_t BHTUNINGclass::bhTune_CheckSettingIsValid() {
		return
			(_bhTune_iKey > PIDMAG) ||
			(_bhTune_iParam > D) ||
			(_bhTune_iKey == PIDPOS && _bhTune_iParam == D) ||
			(_bhTune_iKey == PIDMAG && _bhTune_iParam != P)
			? false : true;
	}


   // SETUP
   void BHTUNINGclass::bhTune_setup() {
      if (bhTune_CheckSettingIsValid()) {
         _bhTune_iParamRangeMin = bhTune_clampUserVal(_bhTune_iValRange[0]);
         _bhTune_iParamRangeDelta = bhTune_clampUserVal(_bhTune_iValRange[1]) - _bhTune_iParamRangeMin;
      }
   }

   // LOOP
   uint8_t BHTUNINGclass::bhTune_loopSlow(int16_t rcDataAuxInput) {
      static uint8_t iParamVal;
      static uint8_t iParamValOld;
      if (bhTune_CheckSettingIsValid()) {


         // get param val (apply TX input to range)
         iParamVal = _bhTune_iParamRangeDelta * ((min(max(rcDataAuxInput, 1000), 2000) - 1000) * 0.001) + _bhTune_iParamRangeMin;

         // check if it's changed
         if (iParamVal != iParamValOld) {

            // update state vars
            _bhTune_bSaved = 0;
            iParamValOld = iParamVal;

            // pitch & roll locked
            if ((_bhTune_iKey == PITCH || _bhTune_iKey == ROLL) && _bhTune_bLockPitchRoll) {
               bhTune_setConfVal(iParamVal, PITCH);
               bhTune_setConfVal(iParamVal, ROLL);
            } else {
               bhTune_setConfVal(iParamVal, _bhTune_iKey);
            }
         }
      } else iParamVal=0;
      return iParamVal;
   }

   // APPLY MULTIPLIER, LIMIT
   uint8_t BHTUNINGclass::bhTune_clampUserVal(float fParamVal) {

      // max vals (multipliers applied)
      uint8_t aPMax[] = {200, 200, 200, 200, 250, 250, 250, 200, 200};
      uint8_t aIMax[] = {250, 250, 250, 250, 250, 250, 250, 250};
      uint8_t aDMax[] = {100, 100, 100, 100, 0, 250, 250, 100};

      // multipliers
      uint8_t aPMult[] = {10, 10, 10, 10, 100, 10, 10, 10, 10};
      uint16_t aIMult[] = {1000, 1000, 1000, 1000, 100, 100, 100, 1000};
      uint16_t aDMult[] = {1, 1, 1, 1, 0, 1000, 1000, 1};

      uint8_t _return;
      // multiply and limit
      switch (_bhTune_iParam) {
      case P:
    	  _return = min(max(0, fParamVal) * aPMult[_bhTune_iKey], aPMax[_bhTune_iKey]);
         break;
      case I:
    	  _return = min(max(0, fParamVal) * aIMult[_bhTune_iKey], aIMax[_bhTune_iKey]);
         break;
      case D:
    	  _return = min(max(0, fParamVal) * aDMult[_bhTune_iKey], aDMax[_bhTune_iKey]);
         break;
      }
      return _return;
   }

   // SAVE
   void BHTUNINGclass::bhTune_save() {
//     if (!f.ARMED && !bhTune_bSettingInvalid && !bhTune_bSaved) {
     if (bhTune_CheckSettingIsValid() && !_bhTune_bSaved) {
         _bhTune_bSaved = 1;
         //writeParams(0);
      }
   }

   // UPDATE PARAM
   void BHTUNINGclass::bhTune_setConfVal(uint8_t iParamVal, uint8_t iTuneKey) {
      switch (_bhTune_iParam) {
      case P:
         conf.pid[iTuneKey].P8 = iParamVal;
         break;
      case I:
         conf.pid[iTuneKey].I8 = iParamVal;
         break;
      case D:
         conf.pid[iTuneKey].D8 = iParamVal;
         break;
      }
   }

//#endif

/*
MULTIPLIERS
              P       I       D
ROLL         10    1000       1
PITCH        10    1000       1
YAW          10    1000       1
PIDALT       10    1000       1
PIDPOS      100     100       0
PIDPOSR      10     100    1000
PIDNAVR      10     100    1000
PIDLEVEL     10    1000       1
PIDMAG       10
*/




