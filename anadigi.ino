#include "Arduino.h"

//****************************************************************************************
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 9;
int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin
int valmapped[3] = {0,0,0};    // variable to read the value from the analog pin
unsigned long time=0;

#define RXHYSTERESIS 2

void RxCalcHysterisis (int16_t xVal)
{
	if (xVal>RXHYSTERESIS) xVal-=RXHYSTERESIS;
	else if (xVal<-RXHYSTERESIS) xVal+=RXHYSTERESIS;
	else (xVal)=0;
}

void RxSwitchVals (int& xVal)
{
	if ((xVal>=1200) && (xVal<=1800)) xVal=1500;
	else if (xVal<1200) xVal=1000;
	else xVal=2000;
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
  //pinMode(buttonPin, INPUT_PULLUP);

 //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // prints title with ending line break
  Serial.println("ASCII Table ~ Character Map");
  valmapped[1]=0;
  valmapped[2]=valmapped[1];
  valmapped[0]=valmapped[1];
}
// first visible ASCIIcharacter '!' is number 33:
int thisByte = 33;
// you can also write ASCII characters in single quotes.
// for example. '!' is the same as 33, so you could also use this:
//int thisByte = '!';


// the loop routine runs over and over again forever:
void loop() {
  //digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)


  //delay(1000);               // wait for a second
    // prints value unaltered, i.e. the raw binary version of the
  // byte. The serial monitor interprets all bytes as
  // ASCII, so 33, the first number,  will show up as '!'
/*
  Serial.write(thisByte);

  Serial.print(", dec: ");
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.println(thisByte);
  // But you can declare the modifier for decimal if you want to.
  //this also works if you uncomment it:

  // Serial.print(thisByte, DEC);


  Serial.print(", hex: ");
  // prints value as string in hexadecimal (base 16):
  Serial.println(thisByte, HEX);

  Serial.print(", oct: ");
  // prints value as string in octal (base 8);
  Serial.println(thisByte, OCT);

  Serial.print(", bin: ");
  // prints value as string in binary (base 2)
  // also prints ending line break:
  Serial.println(thisByte, BIN);

  // if printed last visible character '~' or 126, stop:
  if(thisByte == 126) {     // you could also use if (thisByte == '~') {
    // This loop loops forever and does nothing
    while(true) {
      continue;
    }
  }
  // go on to the next character
  thisByte++;
*/


  int varSWITCH_PIN = 0;  //variable to store switch presses

  if (valmapped[1]!=valmapped[2])
  {
    time = millis() + 450;
    Serial.print(valmapped[1], DEC); Serial.print(":");
    Serial.print(valmapped[2], DEC); Serial.print(":");
    Serial.print(time, DEC); Serial.print(":");
    Serial.print(millis(), DEC); Serial.print(":");
    Serial.println(valmapped[2], DEC);

  } else {
    ;
  }

  if (time>millis()) {
//	Serial.print("-H-");
    valmapped[0]=valmapped[2];
          digitalWrite(led, HIGH);    // turn the LED on
  } else {
//	Serial.print("-L-");
        valmapped[0]=valmapped[1];
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    time = millis();
  }

  valmapped[2]=valmapped[1];
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  valmapped[1]=map(val,0,1023,1000,2000);
  RxSwitchVals(valmapped[1]);



  //delay(200);               // wait for a second

  int ku = ((constrain(valmapped[0],1000,2000) -1000) << 1) >> 6 ; // * 20/640;
  int kp = ku + 8; //range: 1.0-3.7


  //digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
}


