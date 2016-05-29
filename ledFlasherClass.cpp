/*
 * ledFlasherClass.cpp
 *
 *  Created on: 27 May 2016
 *      Author: manu
 */
#include "Arduino.h"
#include "ledFlasherClass.h"

  // Constructor - creates a Flasher
  // and initializes the member variables and state

ledFlasherClass::ledFlasherClass(uint8_t pin, uint16_t on, uint16_t off, uint16_t longoff, uint8_t repeatof)
  {
	_ledPin = pin;
	pinMode(_ledPin, OUTPUT);
/*
	OnTime = on;
	OffTime = off;

	ledState = LOW;
	previousMillis = 0;
*/
	setOnOffTime(on, off, longoff, repeatof);
  }
void ledFlasherClass::setOnOffTime(uint16_t on, uint16_t off, uint16_t longoff, uint8_t repeatof)
{
	_OnTime = on;
	_OffTime = off;
	_LongOffTime = longoff;
	_repeatOnOff = repeatof;

	_ledState = _LOW;
	_previousMillis = 0;
	_repeatOnOffCount = 1;
}

void ledFlasherClass::Update()
{
  // check to see if it's time to change the state of the LED
  unsigned long currentMillis = millis();

	  if((_ledState == _HIGH) && (currentMillis - _previousMillis >= _OnTime))
	  {
		  _ledState = _LOW;  // Turn it off
		  _previousMillis = currentMillis;  // Remember the time
		  digitalWrite(_ledPin, _ledState);  // Update the actual LED
	  }
	  else if ((_ledState == _LOW) && (currentMillis - _previousMillis >= _OffTime))
	  {
		  if (_repeatOnOffCount>=_repeatOnOff) {
			  _ledState = _LOWLOW;
			  _repeatOnOffCount = 1;
		  } else {
			  _repeatOnOffCount +=1;
			  _ledState = _HIGH;  // turn it on
//			  _previousMillis = currentMillis;   // Remember the time
			  digitalWrite(_ledPin, _ledState);	  // Update the actual LED
		  }
		  _previousMillis = currentMillis;   // Remember the time
	  }
	  else if ((_ledState == _LOWLOW) && (currentMillis - _previousMillis >= _LongOffTime))
	  {
		  _ledState = _HIGH;  // turn it on
		  _previousMillis = currentMillis;   // Remember the time
		  digitalWrite(_ledPin, _ledState);	  // Update the actual LED
	  }
}

