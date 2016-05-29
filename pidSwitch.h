/*
 * pidSwitch.h
 *
 *  Created on: 25 May 2016
 *      Author: manu
 */

#ifndef PIDSWITCH_H_
#define PIDSWITCH_H_

#include "Arduino.h"

#define SWITCHDELAY 333

enum rc_switch_states {
	PREVIOUS,
	CURRENT,
	DELTA
};

enum rc_switch_pos {
	posLOW,
	posMID,
	posHIGH,
};

enum rc_switch_dir {
	dirUP,
	dirDOWN,
	dir0
};

enum pid_enum {
	P,
	I,
	D,
	X
};

class PIDSWITCHclass{
private:
	int16_t _potrawval; //= 0;    // variable to read the value from the analog pin
	uint8_t switchrapidtogglecnt; //= 0;  // state when switch is rapidely <400 ms toggled
	uint8_t switchrapidtoggle; //= false;  // state when switch is rapidely <400 ms toggled
	int16_t swval[2]; //= {1500,1500};    // current / previous variable to read the value from the analog pin
	uint8_t swdir[2]; //= {dir0,dir0};    // current / previous variable to hold switch direction
	uint32_t swpostime[3]; //={0,0,0}; // current, previous, delta millisec since switch changed position 49 days and 17 hours....
	rc_switch_pos swpos[2]; //= {posMID,pos0};    // current / previous variable to hold switch position

public:
	PIDSWITCHclass();
	uint8_t switchchange; //= false;  // 1x trigger if switch has changed
	uint8_t pidpage[2]; //= {0,1}; //previous,current default page = 1, reset=0
	pid_enum pid; // P, I or D
	void check(int16_t potrawval);
};


#endif /* PIDSWITCH_H_ */
