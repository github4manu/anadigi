#include "Arduino.h"
#include "pidSwitch.h"
#include "ledFlasherClass.h"

#include "config.h"
#include "types.h"
#include "BHTuning.h"

//multiwii variables
uint32_t currentTime = 0;
uint16_t previousTime = 0;
uint16_t cycleTime = 0;     // this is the number in micro second to achieve a full loop, it can differ a little and is taken into account in the PID loop



//****************************************************************************************
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const uint8_t ledpin = 9;
const uint8_t potpin = 0;  // analog pin used to connect the potentiometer

#define RXHYSTERESIS 2
void RxCalcHysterisis (int16_t xVal)
{
	if (xVal>RXHYSTERESIS) xVal-=RXHYSTERESIS;
	else if (xVal<-RXHYSTERESIS) xVal+=RXHYSTERESIS;
	else (xVal)=0;
}

PIDSWITCHclass AUX1Switch;
int16_t rcRawDataAuxInput = 0;
uint8_t rcRawDataAuxOutput = 0;

BHTUNINGclass BHT(PITCH,P,0);
ledFlasherClass myLED(ledpin,322UL,111UL,644,1); //333,1000>>pidpage[PREVIOUS],500

// the setup routine runs once when you press reset:
void setup() {
 //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {    ; }// wait for serial port to connect. Needed for Leonardo only

  // prints title with ending line break
  Serial.println("ASCII Table ~ Character Map");

  // USER SETUP (BH)
  #ifdef USER_SETUP
       USER_SETUP
  #endif

}

// the loop routine runs over and over again forever:
void loop() {

//read and check switch pod / direction / time
  rcRawDataAuxInput = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  AUX1Switch.check(rcRawDataAuxInput);
  if ((AUX1Switch.switchchange) ) //&& (AUX1Switch.pidpage[AUX1Switch.PREVIOUS]!=AUX1Switch.pidpage[AUX1Switch.CURRENT]))
//  if ((AUX1Switch.pidpage[PREVIOUS]!=AUX1Switch.pidpage[CURRENT]))
  {
	  if (AUX1Switch.pid < AUX1Switch.X) {
		  myLED.setOnOffTime(250,400,644,AUX1Switch.pid+1);
	  } else {
		  myLED.setOnOffTime(55,300,844,AUX1Switch.pidpage[AUX1Switch.PREVIOUS]);
	  }

/*
	  switch (AUX1Switch.pid) {
	      case P:
	    	myLED.setOnOffTime(250,400,644,1);//myLED.setOnForTime(pidpage[PREVIOUS]*1000);
	        break;
	      case I:
	    	myLED.setOnOffTime(250,400,644,2);//myLED.setOnForTime(pidpage[PREVIOUS]*1000);
	        break;
	      case D:
	    	myLED.setOnOffTime(250,400,644,3);//myLED.setOnForTime(pidpage[PREVIOUS]*1000);
	        break;
	      case X:
	    	myLED.setOnOffTime(55,300,844,AUX1Switch.pidpage[PREVIOUS]);//myLED.setOnForTime(pidpage[PREVIOUS]*1000);
	        break;
	      default:
	        // if nothing else matches, do the default
	        // default is optional
	    	;
	      break;
	  }
*/
  }
//  else if (AUX1Switch.switchrapidtoggle)
//  {
//	  myLED.setOnOffTime(644,111,644,AUX1Switch.pidpage[PREVIOUS]);//myLED.setOnForTime(pidpage[PREVIOUS]*1000);
//  }
  myLED.Update();  //flash the led





  // USER LOOPS (BH)
  #ifdef USER_LOOP_FAST
     static uint32_t userLoopFastTime;
     if (currentTime > userLoopFastTime) {
        //userLoopFastTime = currentTime + 20000; // 50 Hz
        userLoopFastTime = currentTime + 10000; // 100 Hz
        USER_LOOP_FAST;
     }
  #endif
  #if defined(USER_LOOP_SLOW)
     static uint32_t userLoopSlowTime;
     if (currentTime > userLoopSlowTime) {
        userLoopSlowTime = currentTime + 100000; // 10 Hz
        USER_LOOP_SLOW;
     }
  #endif
  #if defined(USER_LOOP_1HZ)
     static uint32_t userLoop1HzTime;
     if (currentTime > userLoop1HzTime) {
        userLoop1HzTime = currentTime + 1000000; // 1 Hz
        USER_LOOP_1HZ;
     }
  #endif


     while(1) {
       currentTime = micros();
       cycleTime = currentTime - previousTime;
       #if defined(LOOP_TIME)
         if (cycleTime >= LOOP_TIME) break;
       #else
         break;
       #endif
     }
     previousTime = currentTime;



  //do your actions
  if (AUX1Switch.switchchange)
  {
	Serial.print(AUX1Switch.pid,DEC); Serial.print(":");
//	Serial.print(AUX1Switch.swpos[CURRENT],DEC); Serial.print(":");
//    Serial.print(AUX1Switch.swpos[PREVIOUS], DEC); Serial.print(":");
//    Serial.print(swdir[CURRENT],DEC); Serial.print(":");
//    Serial.print(swdir[PREVIOUS], DEC); Serial.print(":");
////    Serial.print(swval[CURRENT], DEC); Serial.print(":");
//    Serial.print(swval[PREVIOUS], DEC); Serial.print(":");
//    Serial.print(swpostime[CURRENT], DEC); Serial.print(":");
//    Serial.print(swpostime[PREVIOUS], DEC); Serial.print(":");
////    Serial.print(swpostime[DELTA], DEC); Serial.print(":");
////    Serial.print(switchrapidtogglecnt, DEC); Serial.print(":");
    Serial.print(AUX1Switch.pidpage[AUX1Switch.PREVIOUS], DEC); Serial.print(":");
    Serial.print(AUX1Switch.pidpage[AUX1Switch.CURRENT], DEC); Serial.print(":");
    Serial.print(rcRawDataAuxOutput, DEC); Serial.print(":");
    Serial.println("");
  } else {
    ;
  }

//  int ku = ((constrain(swval[0],1000,2000) -1000) << 1) >> 6 ; // * 20/640;
//  int kp = ku + 8; //range: 1.0-3.7

}


