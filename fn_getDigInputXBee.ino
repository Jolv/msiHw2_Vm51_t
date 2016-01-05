
// Constructor
digiInputFromXBee::digiInputFromXBee(int p_inputBitNo,
              bool p_sensAlarmState,
              long p_tmeSnsAlNewLim,
              String p_sensName,
			  CtrLed *fSwitchLED,
			  int smsDtcIx )
{
digiInputName=p_sensName;
inBit=false;
detectFlag=false;
inputBitNo=p_inputBitNo;
sensAlarmState=p_sensAlarmState;
tmeSnsAlNewLim=p_tmeSnsAlNewLim;
diInSt=DiStsDown;
//inputStsLED=5;
switchLED=fSwitchLED;
inputDetectedSmsIdx=smsDtcIx;
} //digitalSensorInput Constructor

int digiInputFromXBee::getDigInValueAndBlink()
{
	int DiStsCngeSignal=0; // Digital input Change Signal
	inBit= bitRead(frameAry[17],inputBitNo); // Read bit
	if(inBit==sensAlarmState)
	{detectFlag=true;
	}else
	{detectFlag=false;
	} //if(inBit==

	switch(diInSt){ // digital_Input_State
		case DiStsDown: // Rename > DiStsReset ??
		diInSt=DiStsOff;
		DiStsCngeSignal=SDownOff;
		break;
		
		case DiStsOff:
		if (detectFlag)//Alarm situation detected
		{
			diInSt=DiStsNewOn;  DiStsCngeSignal=SNewOn;
			switchLED->startBlinkLED();
			tmeOutSnsSts=tmeSnsAlNewLim + tmeLoopEntry;
		}
		break;
		
		case DiStsNewOn:
		if (!detectFlag)
		{
			diInSt=DiStsOff; DiStsCngeSignal=SNewOnOff;
			switchLED->turnOffLED();
			break;
		}
		if (tmeLoopEntry>tmeOutSnsSts)// Stable alarm detection
		{
			diInSt=DiStsOn; DiStsCngeSignal=SOn;
			switchLED->turnOnLED();
			sendSmsQ.push(inputDetectedSmsIdx);
			//dpl(" >diInSt=DiStsNewOn");
		}
		break;
		case DiStsOn:
		if (!detectFlag) // an
		{	diInSt=DiStsNewOff; DiStsCngeSignal=SNewOff;
			dpl("diInSt=DiStsNewOff");
			tmeOutSnsSts=tmeSnsAlNewLim + tmeLoopEntry;// A different timeout is better
		}
		break;
		case DiStsNewOff:
		if (detectFlag)
		{diInSt=DiStsOn;
			break;
		}
		if (tmeLoopEntry>tmeOutSnsSts)// Stable alarm-OFF detection
		{ diInSt=DiStsOff; DiStsCngeSignal=SOff;
			switchLED->turnOffLED();
		}
	}
	//}// if(frameRxIoData - - -
	
	return DiStsCngeSignal;
}// fn updateDigi


void digiInputFromXBee::setSnsStsDown(){
	diInSt=DiStsDown;  // An old inconcistency?? OIC
}
