/*
* smsIdxQ.h
*
* Created: 16.12.2013 20:16:51
* Author: admin
*/
#ifndef __CMSGQUEUE_H__
#define __CMSGQUEUE_H__
#include "Arduino.h"

class smsIdxQ
{
private:
	int qSms[3];
	int qlen;
	int monitQueLedPin;
public:
	smsIdxQ(int pmonitQueLedPin);
	void initiate();
	boolean isempty();
	
	void push(int inStg);
	int pop();
	int qlength();
}; //smsIdxQ

#endif //__CMSGQUEUE_H__
