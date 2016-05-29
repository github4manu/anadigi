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
	switchrapidtoggle = false;  // state when switch is rapidely <400 ms toggled
	switchrapidtogglecnt = 0;  // state when switch is rapidely <400 ms toggled
	pidpage[PREVIOUS] = 0; pidpage[CURRENT] = 1; //previous,current default page = 1, reset=0
	swval[PREVIOUS] = 1500; swval[CURRENT] = 1500;    // current / previous variable to read the value from the analog pin
	swpos[PREVIOUS] = posMID; swpos[CURRENT] = posLOW;    // current / previous variable to hold switch position
	swdir[PREVIOUS] = dir0; swdir[CURRENT] = dir0;    // current / previous variable to hold switch direction
	swpostime[PREVIOUS] = 0; swpostime[CURRENT] = 0; swpostime[DELTA] = 0; // current, previous, delta millisec since switch changed position 49 days and 17 hours....
	pid = X;

	swpos[CURRENT] = posLOW;
	swpos[PREVIOUS] = swpos[CURRENT];
	switchchange = true;
	swdir[CURRENT] = dir0;
	swdir[PREVIOUS] = swdir[CURRENT];
	swpostime[CURRENT] = millis();
	swpostime[PREVIOUS] = swpostime[CURRENT];
	swpostime[DELTA] = swpostime[CURRENT] - swpostime[PREVIOUS];
	switchrapidtoggle = false;

	check(_potrawval); //initialize

}

void PIDSWITCHclass::check(int16_t potrawval)
{
	swval[CURRENT] = map(potrawval,0,1023,1000,2000);
	RxSwitchDiscreteValues(swval[CURRENT],swpos[CURRENT]);
	if (swpos[CURRENT]!=swpos[PREVIOUS] && !switchchange)
	{
		 switchchange = true;
		 swpostime[CURRENT] = millis();
		 swpostime[DELTA] = swpostime[CURRENT] - swpostime[PREVIOUS];

		 if (swpos[CURRENT] > swpos[PREVIOUS])
		   { swdir[CURRENT] = dirUP;}
		 else
		   {swdir[CURRENT] = dirDOWN;}

		 //actions when delta > 400 milliseconds
	//	 if (millis() - swpostime[PREVIOUS]<450) {
		 if (swpostime[DELTA]<SWITCHDELAY)
		 {
			 switchrapidtoggle = true;
			 switchrapidtogglecnt +=1;
			 if ((swdir[CURRENT] == dirUP && swdir[PREVIOUS] == dirUP) || (swdir[CURRENT] == dirDOWN && swdir[PREVIOUS] == dirDOWN))
			 {
				 pidpage[CURRENT] +=1;
			 }
			 else if ((swdir[CURRENT] == dirUP && swdir[PREVIOUS] == dirDOWN) || (swdir[CURRENT] == dirDOWN && swdir[PREVIOUS] == dirUP))
			 {
				 if (switchrapidtogglecnt==1)
				 {
					 if ((swpos[CURRENT] == posLOW && swpos[PREVIOUS] == posMID))
					 {
						 pid = P;
					 } else if ((swpos[CURRENT] == posMID && swpos[PREVIOUS] == posLOW) || (swpos[CURRENT] == posMID && swpos[PREVIOUS] == posHIGH))
					 {
						 pid = I;
					 } else if ((swpos[CURRENT] == posHIGH && swpos[PREVIOUS] == posMID))
					 {
						 pid = D;
					 }
				 }
			  }
		 }
		 else
		 {
			 switchrapidtoggle = false;
			 pid = X;
			 switchrapidtogglecnt = 0;
			 if (pidpage[CURRENT] > 0) pidpage[PREVIOUS]=pidpage[CURRENT];
			 pidpage[CURRENT]=0;
		 }

	}
	else
	{
		 switchchange = false;
		 swdir[PREVIOUS] = swdir[CURRENT];
		 swval[PREVIOUS] = swval[CURRENT];
		 swpos[PREVIOUS] = swpos[CURRENT];
		 swpostime[PREVIOUS] = swpostime[CURRENT];
	}


}
