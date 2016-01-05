/*
* CtrLed.cpp
*
* Created: 30.03.2014 20:29:07
* Author: D630
*/


#include "CtrLed.h"

// default constructor
CtrLed::CtrLed(int fpin,bool fblinkIfBounceFlg)
{
	pinLED=fpin;
	blinkIfBounceFlg=fblinkIfBounceFlg;
	offT=330;
	onT=70;
	blinkFlg=false;
	onOffFlipFlop=true;
} //CtrLed

void CtrLed::setUpLED()
{
	pinMode(pinLED,OUTPUT);
}
void CtrLed::turnOffLED()
{
	digitalWrite(pinLED,LOW);
	blinkFlg=false;
} //turnOffLED
void CtrLed::turnOnLED(){
	digitalWrite(pinLED,HIGH);
	blinkFlg=false;
}
void CtrLed::startBlinkLED(){
	blinkFlg=true;
}
void CtrLed::updateBlinkLED(){
	if (blinkIfBounceFlg)
	{ 	if (blinkFlg)
		{
			mille=millis();
			if (mille>tmOut)
			{
				if (onOffFlipFlop)
				{onOffFlipFlop=false;
				digitalWrite(pinLED,HIGH);
				tmOut=mille+offT;	
				} 
				else
				{onOffFlipFlop=true;
				digitalWrite(pinLED,LOW);
				tmOut=mille+onT;
				}
				
			}
		}
	}
	
}
