/*
 * ledFlasherClass.h
 *
 *  Created on: 27 May 2016
 *      Author: manu
 */

#ifndef LEDFLASHERCLASS_H_
#define LEDFLASHERCLASS_H_
/*
//usage example
ledFlasherClass led1(12, 100, 400, 555, 1);
ledFlasherClass led2(13, 350, 350, 555, 1);
ledFlasherClass myLED(ledpin,322UL,111UL,644,1);

void setup()
{
}

void loop()
{
	led1.Update();
	led2.Update();
}
 */

enum ledstates {
	_LOW,
	_HIGH,
	_LOWLOW
};

class ledFlasherClass
{
private :
	// Class Member Variables
	// These are initialized at startup
	const uint8_t  _ledPin;      // the number of the LED pin
	uint16_t _OnTime;     // milliseconds of on-time
	uint16_t _OffTime;    // milliseconds of off-time
	uint16_t _LongOffTime;    // milliseconds of off-time
	uint8_t  _repeatOnOff;

	// These maintain the current state
	uint8_t  _ledState;             		// ledState used to set the LED
	uint8_t  _repeatOnOffCount;
	uint32_t _previousMillis;  	// will store last time LED was updated

public:
	ledFlasherClass(uint8_t ledpin, uint16_t on, uint16_t off, uint16_t longoff, uint8_t repeatoff);
	void Update();
	void setOnOffTime(uint16_t on, uint16_t off, uint16_t longoff, uint8_t repeatoff);
};

#endif /* LEDFLASHERCLASS_H_ */
