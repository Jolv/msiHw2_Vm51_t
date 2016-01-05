/* 
* CtrLed.h
*
* Created: 30.03.2014 20:29:07
* Author: D630
*/


#ifndef __CTRLED_H__
#define __CTRLED_H__
#include "Arduino.h"

class CtrLed
{
//variables
	int pinLED; // IO-pin which LED is connected to
	long mille; // Temporary variable for time
	long tmOut; // time for next change of state
	   // if blinking is used the on/off interval is
	long offT;  // total off time for LED
	long onT;   // total on time of LED
	bool blinkIfBounceFlg; // if false 
	 // startBlink() and updateBlink() will be ignored
	 // Used to indicate an unstabele input condition
	bool blinkFlg;
	bool onOffFlipFlop;

public:
CtrLed(int fpin,bool fbounceFlg);
void setUpLED(); // used to set up LED pin
void turnOnLED(); // turns LED permanent on
void turnOffLED(); // turns LED permanent OFF
void startBlinkLED(); // turns on blinking
void updateBlinkLED(); // must be called in every loop()
         // to make blinking happened
//=======================================
protected:
private:
/*
//functions
public:
	CtrLed();
	~CtrLed();
protected:
private:
	CtrLed( const CtrLed &c );
	CtrLed& operator=( const CtrLed &c );
*/
}; //CtrLed

#endif //__CTRLED_H__
