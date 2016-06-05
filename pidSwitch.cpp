/*
 * pidSwitch.cpp
 *
 *  Created on: 25 May 2016
 *      Author: manu
 */

#include "Arduino.h"
#include "pidSwitch.h"

void RxSwitchDiscreteValues (int16_t& xVal, rc_switch_pos& xPos)
{
	if ((xVal>=1200) && (xVal<=1800)) {xVal=1500; xPos=posMID;}
	else if (xVal<1200) {xVal=1000; xPos=posLOW;}
	else {xVal=2000; xPos=posHIGH;}
	//return xVal;

}

PIDSWITCHclass::PIDSWITCHclass()
{
	_potrawval = 0;
	//switchchange = false;  // 1x trigger if switch has changed
	_switchrapidtoggle = false;  // state when switch is rapidely <400 ms toggled
	_switchrapidtogglecnt = 0;  // state when switch is rapidely <400 ms toggled
	pidpage[PREVIOUS] = 0; pidpage[CURRENT] = 1; //previous,current default page = 1, reset=0
	_swval[PREVIOUS] = 1500; _swval[CURRENT] = 1500;    // current / previous variable to read the value from the analog pin
	_swpos[PREVIOUS] = posMID; _swpos[CURRENT] = posLOW;    // current / previous variable to hold switch position
	_swdir[PREVIOUS] = dir0; _swdir[CURRENT] = dir0;    // current / previous variable to hold switch direction
	_swpostime[PREVIOUS] = 0; _swpostime[CURRENT] = 0; _swpostime[DELTA] = 0; // current, previous, delta millisec since switch changed position 49 days and 17 hours....
	pid = X;

	_swpos[CURRENT] = posLOW;
	_swpos[PREVIOUS] = _swpos[CURRENT];
	switchchange = true;
	_swdir[CURRENT] = dir0;
	_swdir[PREVIOUS] = _swdir[CURRENT];
	_swpostime[CURRENT] = millis();
	_swpostime[PREVIOUS] = _swpostime[CURRENT];
	_swpostime[DELTA] = _swpostime[CURRENT] - _swpostime[PREVIOUS];
	_switchrapidtoggle = false;

	check(_potrawval); //initialize

}

void PIDSWITCHclass::check(int16_t potrawval)
{
	_swval[CURRENT] = map(potrawval,0,1023,1000,2000);
	RxSwitchDiscreteValues(_swval[CURRENT],_swpos[CURRENT]);
	if (_swpos[CURRENT]!=_swpos[PREVIOUS] && !switchchange)
	{
		 switchchange = true;
		 _swpostime[CURRENT] = millis();
		 _swpostime[DELTA] = _swpostime[CURRENT] - _swpostime[PREVIOUS];

		 if (_swpos[CURRENT] > _swpos[PREVIOUS])
		   { _swdir[CURRENT] = dirUP;}
		 else
		   {_swdir[CURRENT] = dirDOWN;}

		 //actions when delta > 400 milliseconds
	//	 if (millis() - swpostime[PREVIOUS]<450) {
		 if (_swpostime[DELTA]<SWITCHDELAY)
		 {
			 _switchrapidtoggle = true;
			 _switchrapidtogglecnt +=1;
			 if ((_swdir[CURRENT] == dirUP && _swdir[PREVIOUS] == dirUP) || (_swdir[CURRENT] == dirDOWN && _swdir[PREVIOUS] == dirDOWN))
			 {
				 pidpage[CURRENT] +=1;
			 }
			 else if ((_swdir[CURRENT] == dirUP && _swdir[PREVIOUS] == dirDOWN) || (_swdir[CURRENT] == dirDOWN && _swdir[PREVIOUS] == dirUP))
			 {
				 if (_switchrapidtogglecnt==1)
				 {
					 if ((_swpos[CURRENT] == posLOW && _swpos[PREVIOUS] == posMID))
					 {
						 pid = P;
					 } else if ((_swpos[CURRENT] == posMID && _swpos[PREVIOUS] == posLOW) || (_swpos[CURRENT] == posMID && _swpos[PREVIOUS] == posHIGH))
					 {
						 pid = I;
					 } else if ((_swpos[CURRENT] == posHIGH && _swpos[PREVIOUS] == posMID))
					 {
						 pid = D;
					 }
				 }
			  }
		 }
		 else
		 {
			 _switchrapidtoggle = false;
			 pid = X;
			 _switchrapidtogglecnt = 0;
			 if (pidpage[CURRENT] > 0) pidpage[PREVIOUS]=pidpage[CURRENT];
			 pidpage[CURRENT]=0;
		 }

	}
	else
	{
		 switchchange = false;
		 _swdir[PREVIOUS] = _swdir[CURRENT];
		 _swval[PREVIOUS] = _swval[CURRENT];
		 _swpos[PREVIOUS] = _swpos[CURRENT];
		 _swpostime[PREVIOUS] = _swpostime[CURRENT];
	}


}
