/*
 * BHTuing.h
 *
 *  Created on: 31 May 2016
 *      Author: manu
 */

#ifndef BHTUNING_H_
#define BHTUNING_H_

#include "Arduino.h"
//#include "config.h"
#include "types.h"
//#include "def.h"
//#include "MultiWii.h"
//#include "EEPROM.h"

enum _bhTune_params {P, I, D};

class BHTUNINGclass{
private:


    // ****************************************************************
    // ***************** START OF CONFIGURABLE PARAMS *****************

    // PID loop to tune - ROLL, PITCH, YAW, PIDALT, PIDPOS, PIDPOSR, PIDNAVR, PIDLEVEL or PIDMAG
    uint8_t _bhTune_iKey;// = PITCH;

    // PID parameter to tune - P, I or D
    uint8_t _bhTune_iParam;// = P;

    // parameter value range - see maximums above
    float _bhTune_iValRange[2];// = {2, 4};

    // lock pitch & roll values
    uint8_t _bhTune_bLockPitchRoll;// = 0;

    // ****************** END OF CONFIGURABLE PARAMS ******************
    // ****************************************************************

    // check if setting exists

    uint8_t _bhTune_bSaved = 0;
    uint8_t _bhTune_iParamRangeDelta;
    uint8_t _bhTune_iParamRangeMin;

    uint8_t bhTune_CheckSettingIsValid();
	void bhTune_setConfVal(uint8_t iParamVal);
	void bhTune_setup();
	uint8_t bhTune_clampUserVal(float fParamVal);

public:
	BHTUNINGclass  (uint8_t iKey, _bhTune_params iParam, uint8_t ibLockPitchRoll);
	void setParams (uint8_t iKey, _bhTune_params iParam, uint8_t ibLockPitchRoll) ;

	uint8_t bhTune_loopSlow(int16_t rcDataAuxPotInput);
	void bhTune_save();

};




#define BH_TUNE

#endif /* BHTUNING_H_ */
