// Constructor and all fn in here
// Constructor
xBePara::xBePara(
  String pnameXbe,
  int prxQidx,
  int psnltQidx,
  long padrLB,
  long ptmOutLim,
  int pLedPinXB
  // boolean pinUseFlg in here <err<<
  )
{
	nameXbe=pnameXbe;
	xbReceiveSmsIdx=prxQidx;
	xbSilenSmsIdx=psnltQidx;
	adrLB=padrLB;
	tmOutLim=ptmOutLim;
	LedPinXB=pLedPinXB;
	xbSts=0;
	tmOutRx=tmOutLim;
	tmOutSlnt=tmOutLim;
	tmOutFlagRx=false;
	tmOutFlagSlnt=false;
	firstPassThrough=true
//  inUseFlg=pinUseFlg; // <err<< rett opp for inUse control
	 ;
}
// <err<<
// in ::received & ::timeout it should
// tested if an xBe is in use. To
// achieve this an inUse Flag must be
// included in the constructor parameters.
// must be properly analyzed before impl.

boolean xBePara::received(){
	long xBAdrFrame=0; // Temporary address of xBee
	// xbStsSignal here
	// Assemble xBee address from 4 bytes in frameAry[]
	for(int i=5;i<9;i++) xBAdrFrame=xBAdrFrame*256+frameAry[i];
	if(xBAdrFrame==adrLB){ // Address of "this" xBee
// her setter you in inUseFlg test
// hvis ikke inUse returner med false.
		digitalWrite(LedPinXB,HIGH);
		tmOutSlnt=tmOutLim+tmeLoopEntry; // Set new timeout for missing xBee frame
		tmOutFlagSlnt=false;
		if(!tmOutFlagRx){
			// Serial.print("tmOutRx is FLASE, tmOutLim=");
			// Serial.println(tmOutLim);
			if (firstPassThrough)
			{
				firstPassThrough=false;
				tmOutRx=tmOutLim+tmeLoopEntry; // Set new timeout for missing xBee frame
				
				if(monitFlag){
                	Serial.print("First Pass,  tmOutLim=");
					Serial.print(tmOutLim);
					Serial.print("  tmOutRx=");
					Serial.println(tmOutRx);
                    } // if(monitFlag
			}
			
			if (tmeLoopEntry>tmOutRx)
			{	tmOutFlagRx=true;
				sendSmsQ.push(xbReceiveSmsIdx);//
				Serial.print("-Stable rx from= "); Serial.println(nameXbe);
			}  // if (tmeLoopEntry>
		} // if(tmOutFlag
		return true;
	}// if(xBAdr . ..
	else{
	return false;}
}// received(


boolean xBePara::timedOut(){
	// Check first if this XBee is in use <err<<
	// if not return false (ie. no Timeout)
	//tmOutFlagSlnt
	if(!tmOutFlagSlnt){

		if (tmeLoopEntry>tmOutSlnt)
		{
			/*
			tmOutRx=tmOutLim+tmeLoopEntry; // Set new timeout for missing xBee frame
			Serial.print("tmOutLim=");
			Serial.print(tmOutLim);
			Serial.print("  tmOutRx=");
			Serial.println(tmOutRx);
			*/
			tmOutFlagSlnt=true;
			firstPassThrough=true;
			tmOutFlagRx=false;		
			sendSmsQ.push(xbSilenSmsIdx);
			digitalWrite(LedPinXB,LOW);
			Serial.print("  -Timeout: No signal from:  ");
			Serial.println(nameXbe);
			return true;
		}
	}
	else
	{
		return false;
	}
}