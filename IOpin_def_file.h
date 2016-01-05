/* 
* IOpin_def_file.h
*
* Created: 07.06.2014 05:22:17
* Author: Jorolv
* 
* Overview of LED indicator panel in file:
* ... 08 AutoMa\HuVa\msi\LED-INDICATORS_name.xlsx
*/
#ifndef __IOPIN_DEF_FILE_H__
#define __IOPIN_DEF_FILE_H__

const int monitorQueLEDwh=45;   //? OBS! for dbg LED glowing if something in Q ?
const int sndSmsLedPin=53;	// Shows SMS transmission in progress
const int onLedPin=52;		// Indicates system is on
const int mtXbLedPin=51;	//  musTrp XBee active indicator - Ye
const int mtDi4LedPin=49;	// musTrp digital input Di04
const int bkXbLedPin=50;    // BlomKa XBee active indicator - Ye
const int bkDi4LedPin=48;   // BlomKa digital input Di04
const int drpMonAXbLedPin=43;	// Drop counter XBee pin
const int drpMonATooDryLedPin=41;	// Drop counter XBee pin
const int drpMonATooWetLedPin=39;	// Drop counter XBee pin
const int tsXbLedPin=37;            // Tensiometer XBee active indicator color ?? tsXBLedPin=37

//* New XBee LED in here
//* New digital input LED pins for in here


#endif //__IOPIN_DEF_FILE_H__
