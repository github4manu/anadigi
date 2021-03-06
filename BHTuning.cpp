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
   PIDPOSR     25.0    2.50     0.250
   PIDNAVR     25.0    2.50     0.250
   PIDLEVEL    20.0    0.250    100
   PIDMAG      20.0
 */

#include "BHTuning.h"


//multiwii variables
conf_t conf;
//int16_t rcData[RC_CHANS];    // interval [1000;2000]



//#ifdef BH_TUNE
BHTUNINGclass::BHTUNINGclass(uint8_t iKey, _bhTune_params iParam, uint8_t ibLockPitchRoll)
	: _bhTune_iKey(iKey), _bhTune_iParam(iParam), _bhTune_bLockPitchRoll(ibLockPitchRoll) {
	setParams(iKey, iParam, ibLockPitchRoll);
	bhTune_setup(); //_bhTune_iParamRangeDelta , _bhTune_iParamRangeMin
}

void BHTUNINGclass::setParams (uint8_t iKey, _bhTune_params iParam, uint8_t ibLockPitchRoll) {
	_bhTune_iKey = iKey;
	_bhTune_iParam = iParam;
	switch (_bhTune_iKey)
	{
	case PITCH: case ROLL: case YAW: case PIDALT: case PIDLEVEL: case PIDMAG:
		if (_bhTune_iParam==P) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 20.0;} ;
		if (_bhTune_iParam==I) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 0.250;} ;
		if (_bhTune_iParam==D) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 100;} ;
		break;
	case PIDPOSR: case PIDNAVR:
		if (_bhTune_iParam==P) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 25.0;} ;
		if (_bhTune_iParam==I) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 2.50;} ;
		if (_bhTune_iParam==D) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 0.25;} ;
		break;
	case PIDPOS:
		if (_bhTune_iParam==P) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 2.50;} ;
		if (_bhTune_iParam==I) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 2.50;} ;
		if (_bhTune_iParam==D) {_bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1] = 2.50;} ;
		break;
	default: _bhTune_iValRange[0] = 0.0; _bhTune_iValRange[1]=1.0; break;
	}
	bhTune_setup(); //_bhTune_iParamRangeDelta , _bhTune_iParamRangeMin
	_bhTune_bLockPitchRoll = ibLockPitchRoll;
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
   uint8_t BHTUNINGclass::bhTune_loopSlow(int16_t rcDataAuxPotInput) {
      uint8_t iParamVal;
      uint8_t iParamValOld;
      if (bhTune_CheckSettingIsValid()) {


         // get param val (apply TX input to range)
         iParamVal = _bhTune_iParamRangeDelta * ((min(max(rcDataAuxPotInput, 1000), 2000) - 1000) * 0.001) + _bhTune_iParamRangeMin;

         // check if it's changed
         if (iParamVal != iParamValOld) {

            // update state vars
            _bhTune_bSaved = 0;
            iParamValOld = iParamVal;

            // pitch & roll locked
            if ((_bhTune_iKey == PITCH || _bhTune_iKey == ROLL) && _bhTune_bLockPitchRoll) {
               bhTune_setConfVal(iParamVal);
               bhTune_setConfVal(iParamVal);
            } else {
               bhTune_setConfVal(iParamVal);
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
   void BHTUNINGclass::bhTune_setConfVal(uint8_t iParamVal) {
      switch (_bhTune_iParam) {
      case P:
         conf.pid[_bhTune_iKey].P8 = iParamVal;
         break;
      case I:
         conf.pid[_bhTune_iKey].I8 = iParamVal;
         break;
      case D:
         conf.pid[_bhTune_iKey].D8 = iParamVal;
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




