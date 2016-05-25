#include "Arduino.h"

//#define CURRENT 0
//#define RC_SWITCH_STATE         PREVIOUS,CURRENT,NEXT //1,2,3

enum rc_switch_states {
	PREVIOUS,
	CURRENT,
	DELTA
};

enum rc_switch_pos {
	posLOW,
	posMID,
	posHIGH,
	pos0
};

enum rc_switch_dir {
	dirUP,
	dirDOWN,
	dir0
};

//****************************************************************************************
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 9;
int potpin = 0;  // analog pin used to connect the potentiometer

int potrawval = 0;    // variable to read the value from the analog pin
//int potmapval = 0;
uint8_t switchchange = false;  // 1x trigger if switch has changed
uint8_t switchrapidtoggle = false;  // state when switch is rapidely <400 ms toggled
uint8_t switchrapidtogglecnt = 0;  // state when switch is rapidely <400 ms toggled
uint8_t pidpage[2] = {0,1}; //previous,current default page = 1, reset=0
int16_t swval[2] = {1500,1500};    // current / previous variable to read the value from the analog pin
uint8_t swpos[2] = {posMID,pos0};    // current / previous variable to hold switch position
uint8_t swdir[2] = {dir0,dir0};    // current / previous variable to hold switch direction
uint32_t swpostime[3]={0,0,0}; // current, previous, delta millisec since switch changed position 49 days and 17 hours....
unsigned long time=0;

#define RXHYSTERESIS 2

void RxCalcHysterisis (int16_t xVal)
{
	if (xVal>RXHYSTERESIS) xVal-=RXHYSTERESIS;
	else if (xVal<-RXHYSTERESIS) xVal+=RXHYSTERESIS;
	else (xVal)=0;
}

void RxSwitchDiscreteValues (int16_t& xVal, uint8_t& xPos)
{
	if ((xVal>=1200) && (xVal<=1800)) {xVal=1500; xPos=posMID;}
	else if (xVal<1200) {xVal=1000; xPos=posLOW;}
	else {xVal=2000; xPos=posHIGH;}
	//return xVal;

}

int foo(int& bar1, int bar2){
bar1 = bar1 + bar2;
return (bar2 - bar1);
}

void subOne(){
int v1 = 10;
int v2 = 100;
int v3 = foo(v1,v2);
Serial.println(v1);
Serial.println(v3 );
}


// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  //pinMode(potpin, INPUT_PULLUP);

 //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {    ; }// wait for serial port to connect. Needed for Leonardo only

  // prints title with ending line break
  Serial.println("ASCII Table ~ Character Map");

  swpos[CURRENT] = posLOW;
  swpos[PREVIOUS] = swpos[CURRENT];
  switchchange = true;
  swdir[CURRENT] = dir0;
  swdir[PREVIOUS] = swdir[CURRENT];
  swpostime[CURRENT] = millis();
  swpostime[PREVIOUS] = swpostime[CURRENT];
  swpostime[DELTA] = swpostime[CURRENT] - swpostime[PREVIOUS];
  switchrapidtoggle = false;
}
// first visible ASCIIcharacter '!' is number 33:
int thisByte = 33;
// you can also write ASCII characters in single quotes.
// for example. '!' is the same as 33, so you could also use this:
//int thisByte = '!';


// the loop routine runs over and over again forever:
void loop() {
  //digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)



//read and check switch pod / direction / time
  potrawval = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
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
	 if (swpostime[DELTA]<450) {
		 switchrapidtoggle = true;
		 if ((swdir[CURRENT] == dirUP && swdir[PREVIOUS] == dirUP) || (swdir[CURRENT] == dirDOWN && swdir[PREVIOUS] == dirDOWN)) {
			 pidpage[CURRENT] +=1;
		 }
		 if ((swdir[CURRENT] == dirUP && swdir[PREVIOUS] == dirDOWN) || (swdir[CURRENT] == dirDOWN && swdir[PREVIOUS] == dirUP)) {
			 switchrapidtogglecnt +=1;
		 }
	 } else {
	     switchrapidtoggle = false;
		 switchrapidtogglecnt = 0;
		 if (pidpage[CURRENT] > 0) pidpage[PREVIOUS]=pidpage[CURRENT];
		 pidpage[CURRENT]=0;
	 }

  } else {
	 switchchange = false;
	 swdir[PREVIOUS] = swdir[CURRENT];
	 swval[PREVIOUS] = swval[CURRENT];
	 swpos[PREVIOUS] = swpos[CURRENT];
	 swpostime[PREVIOUS] = swpostime[CURRENT];
  }

  if (switchrapidtoggle) {
	  digitalWrite(led, HIGH);
  } else {
	  digitalWrite(led, LOW);
  }
/*
  if (switchchange) {
	  if ((switchrapidtoggle) && (swpos[CURRENT] != swpos[PREVIOUS])) {
			 digitalWrite(led, HIGH);    // turn the LED on
		  switchrapidtogglecnt += 1;
	  } else {
		  ;
	  }
  } else {
	  switchrapidtogglecnt = 0; // reset
	     digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW

  }
*/

  //do your actions
  if (switchchange)
  {
//	Serial.print(swpos[CURRENT],DEC); Serial.print(":");
//    Serial.print(swpos[PREVIOUS], DEC); Serial.print(":");
//    Serial.print(swdir[CURRENT],DEC); Serial.print(":");
//    Serial.print(swdir[PREVIOUS], DEC); Serial.print(":");
    Serial.print(swval[CURRENT], DEC); Serial.print(":");
//    Serial.print(swval[PREVIOUS], DEC); Serial.print(":");
//    Serial.print(swpostime[CURRENT], DEC); Serial.print(":");
//    Serial.print(swpostime[PREVIOUS], DEC); Serial.print(":");
    Serial.print(swpostime[DELTA], DEC); Serial.print(":");
    Serial.print(switchrapidtogglecnt, DEC); Serial.print(":");
    Serial.print(pidpage[PREVIOUS], DEC); Serial.print(":");
    Serial.print(pidpage[CURRENT], DEC); Serial.print(":");
    Serial.println("");
  } else {
    ;
  }

  int ku = ((constrain(swval[0],1000,2000) -1000) << 1) >> 6 ; // * 20/640;
  int kp = ku + 8; //range: 1.0-3.7


  //digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
}


