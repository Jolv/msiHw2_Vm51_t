/*------------------------------------------------------------------
* Bygger på "" - classe og fn tatt inn fra msiMk0_u0TT2
* Started modification for Mega 28/1-14, 1.SMS 2/2
* Raw and dirty mod. worked late at night 11/2-2014 
* Missing XBee transmission messy - must rework
* Initiation messy and solved with dirty timeout in SMS sending 
*---------------------------------------------------------------*/
#include <SoftwareSerial.h>
#include "smsIdxQ.h"
#include "SMS_def_file.h"
#include "CtrLed.h"
#include "IOpin_def_file.h"
#define DROPMON_active
#define DBG_Dropmon_print
//#define DBG_BMP_print
//#define PRINT_XBee_FRAME
//CtrLed::CtrLed(int fpin,bool fblinkIfBounceFlg)
CtrLed mtDi4LedIndic(mtDi4LedPin,true);
CtrLed bkDi4LedIndic(bkDi4LedPin,true);     
//* New digital input LED instantiated here 
// define digital input states DiSts for class and loop.
const int DiStsDown=0, DiStsOff=2, DiStsNewOn=3, DiStsOn=5,DiStsNewOff=6; 
// State-Change-Signal values for digital inputs to XBee
const int SDown=0,SDownOff=1, SOff=2,SNewOn=3; // OIC sts values and 
const int SNewOnOff=4,SOn=5,SNewOff=6; // change-signals should be separated!!
       
#define xbSerial Serial2     // Serial to XBee Coordinator
#define gprsSerial Serial3   // Serial to GPS-shield sending SMS
const int frameAryLen=40;    // See below
byte frameAry[frameAryLen];  // Storing XBee input frame received frame 
// Help variables for timing of loop processing
long tmeLoopEntry;             // time when loop is entered
long tmeLastLoopEntry;         // last reading of current time
int tmeIncrementLoopTrough;    // debug:time since last loop-pass-through
int maxtmeIncrementLoopTrough; // debug:for printing max time (debugging)
long loopCnt;                  //used for timing of loop
long loopMillisOld;            // for timing of loop
long tmeNextPyWrt;             // timer controlling output to Py
const long tmeIntrPyWrt = 60000; //<Init Define interval length
int DUM; // For different purposes
char sendSmsSts=0;   // Sending of sms step through several states
long sendSmsTmeOut;  // Variable used to hold timeout of a sending state <- - - - --
int sendSmsTxtIdx=0; //Indexing which SMS text to send
// Array for Byte-buffering values to be sent to Py
const int pyBufIxMax = 12;
int pyBuff[pyBufIxMax];
//====Definitions for controlling debug printing
#define spnt Serial.print
#define spntln Serial.println
#define dp spnt
#define dpl spntln
boolean monitFlag=false; //Turns on/off printed msg 
//-------------------------------
//==== Parameters for digital input to xBee End Node 
class digiInputFromXBee{			//<<< Rename to digiInputXBe
	String digiInputName; // String with name of input
	int inputBitNo;       // Digital input bit connected xBee
	bool inBit;		      // Temporary, holding input from xBee
	bool detectFlag;      // temporary holder of alarm state
	int diInSt;  // Digital Input state (used in switch/case state machine)
	// Def input bit conditions for Alarm (high or low)
	bool sensAlarmState; // "true" imply Alarm when
    	   // digital in  digital input high (switch closed)
           //{tmeOutSnsSts}={tmeSnsAlNewLim} + {tmeLoopEntry}
	long tmeOutSnsSts;   // Timeout for Sensor status
	long tmeSnsAlNewLim; // timeout limit
	CtrLed *switchLED;   // Pointer to LEDobject switched on/off
	int inputDetectedSmsIdx; //Index of SMS sent, seeVariableText.ino
	public:
 // Constructor prototype 
	digiInputFromXBee(int p_inputBitNo,
	bool p_sensAlarmState,
	long p_tmeSnsAlNewLim,
	String p_sensName,
	CtrLed *fSwitchLED,
	int smsDtcIx
	);
 //Functions	prototypes	
	void updateDigiSnsSts(long tmeLoopEntry, unsigned char frameAry[]);
	int getDigInValueAndBlink();
	void setSnsStsDown();
}; // end class digitalInputXBee
//==== Parameters for xBee EndNode

class xBePara{
  private:	
	String nameXbe; // name of XBee, 5 char ¤
	long adrLB;    // Lower Address byte of end xBee ¤
	int xbSts;   // XBee status variable ¤
	int xbReceiveSmsIdx, xbSilenSmsIdx; // SMS no SMS
	long tmOutRx;   // Time out var. for stable reception
	long tmOutSlnt; // Time out var. for no signal
	long tmOutLim; // Time out limit ¤
	int ctFramesRx; // Counter for stable reception
	int LedPinXB; // counter limit ¤
	boolean tmOutFlagRx; // Signal time int. passed
	boolean tmOutFlagSlnt; //
	boolean firstPassThrough; 
	boolean inUseFlg;  // False if in here <err<<
 public:
 // Constructor template
	xBePara(
	String pnameXbe,
	int prxQidx,
	int psnltQidx,
	long padrLB,
	long ptmOutLim,
	int pLedPinXB
// boolean pinUseFlg // must in here <err<<
	);
	// Function templates.
	boolean received(); // Tests if frame received
	boolean receivedShort(); // For Drop Counter XBee
	boolean timedOut(); // Time out handling when XBe silent
};

//--------------------------------
//====Function definitions follow
boolean goodFrameIsFetched(){
  byte readChar;
  int inbufLen=0; 
  int inbufLen2;
  long checkSum=0;
  inbufLen=xbSerial.available();
  if(inbufLen<18) return false; // Too few char in buffer
  // read and analyst content of buffer
  // start with scanning for Start of Frame char.
  while(xbSerial.read()!= 0x7E) if(xbSerial.available()==0) return false;
  // Start of Frame char detected, fetch and calculate
  // no. of bytes in frame
  int frameLen=xbSerial.read();
  frameLen=frameLen*256+xbSerial.read();
  const int frameLenMinimum=5;
  if(frameLen<frameLenMinimum || frameLen>=frameAryLen) return false;
  // must be changed: minimum frame not necessary, and if
  // frame in buffer does not fit in array empty it and return false.
  // **********************************************************
  inbufLen2=xbSerial.available();
  int missingChar=frameLen+1-inbufLen2;
  if(missingChar>0)delay((missingChar*3/2)+3);
  // after waiting a complete frame is available in buffer
  for(int i=0;i<frameLen;i++) {
    frameAry[i]=xbSerial.read();
    checkSum+=frameAry[i];
  }// creating checksum finished
  checkSum+=xbSerial.read();
  byte checkByte=checkSum & 0xFF;
  if(checkByte==0xFF) {
    return true; //good frame received
  }else{
    /* dbg
    dpl(); dp("innBuffer="); dp(inbufLen2); dp(" "); 
     dp(" FrameLen= "); dp(frameLen); dp("  Restbuffer="); 
     dp(tXBe.available()); dp(" missingChar= "); 
     dp(missingChar); dpl(" ");
     for(int i=0;i<frameLen;i++) {
     dp(i); dp("="); dp(frameAry[i],HEX); dp(" ");
     }
	 
     */
    while(xbSerial.available()>0) checkByte= xbSerial.read(); //checkByte as dummy
    frameAry[0]=0; // set bad frame indication
    return false;
  } 
}// end of goodFrameFetched
//========instantiate data classes
// Start with initiating 
xBePara musTrpXb("musTrpXb",		// debug name of XBee
	Sms_MusTraXBeeTransmitting,	// Mcr: Index of SMS
	Sms_MusTraXBeeSilent,		// Mcr: Index of SMS
	0x40712379,					// Lower adr of XBee
	10000,					    // time out for no signal
	mtXbLedPin);	        	// Mcr: output pin for LED
xBePara blmKaXb("blmKaXb",
	Sms_BlomKa_xBee_is_transm,
	Sms_BlomKa_xBee_is_SILENT,
	0x408AD916,
	60000,		// Timeout of 8000 gave errors????
	bkXbLedPin);
xBePara drpMonA_Xbe("dropCnt",  //* New XBee LED in here
	Sms_dropCnt_Xbee_is_transm,
	Sms_dropCnt_Xbee_is_SILENT,
	0x408AD905,
	60100,drpMonAXbLedPin); // DroCnt transmits every 20s, 3 frames required

xBePara tsBMP_Xbe("tsBMP",  //* New XBee LED in here
	Sms_tsBMP_Xbee_is_transm,
	Sms_tsBMP_Xbee_is_SILENT,
	0x40B91267,
	60100,tsXbLedPin); // DroCnt transmits every 20s, 3 frames required

// New XBee is instantiated here	
digiInputFromXBee musTrpD4(4,	// Input bitno. in XBee frame
	true,		// state of input causing an alarm				
	15000,		// timeout for 'bouncing period'
	"musTrpD4",	// debug name og digital input
	&mtDi4LedIndic,	// reference to LED obj. 
	Sms_fromMusTra_Di04); // Mcr: Index of SMS
digiInputFromXBee blmKaD4(4,
	true,
	5000,
	"plaVaD4",
	&bkDi4LedIndic,
	Sms_BlomKa_water_res_empty);

smsIdxQ sendSmsQ(monitorQueLEDwh); // Param def. which LED should blink when something in queue

int totalNumOfMomCt=0; //<sim total number of moments counted for printing ONLY
// Tensio-tests
int tensio1LowByte=9999, tensio1HighByte=8888, tensio1Val=0;
int teLowByte2=0, teHighByte2=0, teVal2=0;
int tensioValA, tempValA;

//==========================================================
void setup(){
// SketchVersion
  Serial.begin(115200);  
  dp("-Name of program version: ");
  dpl("msiHw2_Vm51_t"); 
// Set up hardware
// Test LED-indicator
	initPinsdown(53,45,-2);
	initPinsdown(50,44,-2);
	initPinsdown(43,37,-2);
	pinMode(onLedPin,OUTPUT);
	digitalWrite(onLedPin,HIGH);
	
// Initiate LED for digital input that shall Blink  
  mtDi4LedIndic.setUpLED();
  bkDi4LedIndic.setUpLED();
//* New digital input LED for in here 
// LEDs for output from XBee 
  pinMode(bkXbLedPin,OUTPUT);
  pinMode(mtXbLedPin,OUTPUT); // initialize green LED
  pinMode(drpMonAXbLedPin,OUTPUT); // Initiate drop count XBee
  pinMode(tsXbLedPin,OUTPUT); // Initiate tensiometer XBee
//* New XBee LED in here  
  pinMode(sndSmsLedPin,OUTPUT); // LED On when sending SMS
  xbSerial.begin(9600);// Serial2, RX2->pin2 tx, TX3->pin3 rx XBee
  gprsSerial.begin(19200); //Serial3, RX3->pin7 TX3->pin8 SeeedGPRS V.1
  sendSmsQ.initiate(); // Open diode for Output 
 // Control interval between initially writes to Python
  tmeNextPyWrt = millis() + 10000; // also when Reset-btn used
 // Used to help debuging
  spntln();  spntln("-Setup finished");
  loopCnt=0;
  }// end of setup

void loop(){ //000000000000000000000000000000000
//==== Begin with debug sequence
  loopCnt++;
  if(loopCnt>100000){
    loopCnt=0;
//    dp("  100k loops took "); dp((millis()-loopMillisOld));  
//    dp(" msec  ");
//    dpl(maxtmeIncrementLoopTrough);
    maxtmeIncrementLoopTrough=0;
    loopMillisOld=millis();  
	} // if(loopCnt>
   if(tmeIncrementLoopTrough>maxtmeIncrementLoopTrough) { 
     maxtmeIncrementLoopTrough=tmeIncrementLoopTrough;  }
//==== end debugg sequence

  tmeLoopEntry=millis();          // set loop-pass time
  tmeIncrementLoopTrough=tmeLoopEntry-tmeLastLoopEntry;
  tmeLastLoopEntry=tmeLoopEntry;
  while(goodFrameIsFetched()){   // read frame from xBee End Poing
	  //dpl(" Meth:Good frame read");
	  
	  if (ioDataSampleRxIndFrame())
	  {
		  if (musTrpXb.received())
		  {
			  DUM=musTrpD4.getDigInValueAndBlink();
		  }//if (bleMus.received(blmKaXb

		  if (blmKaXb.received())
		  {
			 DUM=blmKaD4.getDigInValueAndBlink();
		  }// if(frameIsFrom(xBplaVa
       //* New XBee is tested here by .received() fn.
       //* New digital input entered here		  
	  } //if (ioDataSampleRxIndFrame
	  
	  //  Next check for drop counters
	  if (ZigBeeReceivePacket()) //this frame carries several "data bytes"
	  {
#ifdef DROPMON_active
	 	if (drpMonA_Xbe.received())
#else
        if(0) //never execute the if sequence
#endif
		   {// Yes, a frame with data bytes received
			//Read Pressure values
			   teLowByte2 = frameAry[12];
			   teHighByte2 = frameAry[13];
			   teHighByte2 = teHighByte2 << 8;
			   teVal2 = teHighByte2 + teLowByte2;
			   pyBuff[0] = teVal2;
			   Serial.print("drpMonA_Xbe. received, P: "); //<<<
			   Serial.print(teVal2);
#ifdef DBG_Dropmon_print
			   // print debug text here
#endif
		   // temperature values
			   teLowByte2 = frameAry[14];
			   teHighByte2 = frameAry[15];
			   teHighByte2 = teHighByte2 << 8;
			   teVal2 = teHighByte2 + teLowByte2;
			   Serial.print(" T:");
			   Serial.println(teVal2);
			   pyBuff[1] = teVal2;
#ifdef DBG_Dropmon_print
			   // print debug text here
#endif
		} //  if (drpMonA_Xbe.received


// Handling of tensiometer input follows
if (tsBMP_Xbe.received())
{// Yes, a from tensiom received
//Read Pressure values
teLowByte2= frameAry[12];
teHighByte2= frameAry[13];
teHighByte2=teHighByte2<<8;
teVal2=teHighByte2+teLowByte2;
pyBuff[2]= teVal2; // write pressure to pyBuff
#ifdef DBG_BMP_print
	Serial.print("tensioLB2 ="); Serial.print(teLowByte2,HEX);
	Serial.print(" tensioHB2 ="); Serial.print(teHighByte2,HEX);
	Serial.print(" tensio1Val2 ="); Serial.print(teVal2,HEX);
	Serial.print(" = "); Serial.println(teVal2);
#endif // if(tsBMP
	Serial.print("tsBMP_Xbe. received, P: "); //<<<
	Serial.print(teVal2);
// REad temperature values
teLowByte2= frameAry[14];
teHighByte2= frameAry[15];
teHighByte2=teHighByte2<<8;
teVal2=teHighByte2+teLowByte2;
pyBuff[3]= teVal2; // write temperature to pyBuff
Serial.print(" T:");
Serial.println(teVal2);

#ifdef DBG_BMP_print
	Serial.print("tempLB2 ="); Serial.print(teLowByte2,HEX);
	Serial.print(" tempHB2 ="); Serial.print(teHighByte2,HEX);
	Serial.print(" tempVal2 ="); Serial.print(teVal2,HEX);
	Serial.print(" = "); Serial.println(teVal2);
#endif

#ifdef PRINT_XBee_FRAME
Serial.print("tsXBee frame: ");
for (int fj=0;fj<20;fj++)
  {	  Serial.print(" "); 
	  Serial.print(frameAry[fj],HEX); }
#ifdef DBG_Dropmon_print
// just a test of nesting macro commands
#endif
   Serial.println();
#endif

} //if (tsBMP_Xbe.received


   } //  if (ZigBeeReceivePacket
  } // while(goodFrameIsFetched(
// *** No more frames received in this loop() pass through

// Check it time to write to Py
  if (millis()>tmeNextPyWrt)
  {   //Reset timer
	  tmeNextPyWrt = tmeIntrPyWrt+millis();
	  // Transmitt to python
	  Serial.print(">,b"); Serial.print(",");
	  for (int j = 0; j < 6; j++)
	  {
		  Serial.print(pyBuff[j]);
		  Serial.print(',');
	  } // for int j 
	Serial.println();
  }

  if (musTrpXb.timedOut())
  { //musTrpD4.setSnsStsDown(); // if you want an SMS when
	// bleMus starts receiving again when alarm is on.  
  } // (bleMus.timedOu  
  if (blmKaXb.timedOut())
  {  // if you want to set sensor state to off when XB go silent
	 // plaVaD4.setSnsStsDown(); // an old inconsistency: OIC ?
  } //(blomKa.timedOut

  if (drpMonA_Xbe.timedOut())
  { // just to get a message if it stops <err<<
    // SomeHow to wet and too dry LEDS
	// should be turned off when Xbe 
	// stops transmitting!
  }

// Timeout for for tsBMP_Xbe inn here!!!!
  tsBMP_Xbe.timedOut();

  //* New XBee must test on '.timedOut' here
  
  // *** CHECK if a new message has arrived, and than push it on queue
  // The following switch controls the sending of sms from the *GPRS-shield
  // sSendSts
  // = 0, check for  msg. in queue,if so initIate sending sms
  // = 1, set telephone number
  // = 2 sendSMSContent,
  // = 3 finish sending
  // = 4 wait before a nxt msg can be sent.
  // Temporary solution:
  // if value in queue < 1000 it is an SMS index
  // if value in queue > 1000 it is a numerical value
  // 1000 is subtracted and the numerical value is
  // sent in an sms
  
  if (tmeLastLoopEntry<80000) // prevent too many Sms at start up
  { while(!sendSmsQ.isempty()) sendSmsTxtIdx = sendSmsQ.pop();
  }
  
  if (tmeLoopEntry>sendSmsTmeOut) // Time for checking sms sending process
  {  // look on file msgInSMSUt_30.ino for AT sequence
	  switch (sendSmsSts){
		case 0: // check for msg and initiate sending SMS
		  if(sendSmsQ.isempty())
		  {   // no msg received set time for next check
			  sendSmsTmeOut=tmeLoopEntry+50;
		  } else{ // msg from queue, fetch msg and initiate sending of sms
			  sendSmsTxtIdx = sendSmsQ.pop();
			  digitalWrite(sndSmsLedPin,HIGH);
			  if(monitFlag){
			    dpl(); dp("Msg in SMS-Q, msg no.:"); 
			    dpl(sendSmsTxtIdx); 
			    } //if(monitFlag
			  gprsSerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
			  sendSmsSts+=1; // set next state
			  if(tmeLastLoopEntry<15000) sendSmsSts=0; // *** dirty debugging
			  sendSmsTmeOut=tmeLoopEntry+300; // wait millis before starting nest state
		  } // if(sendSmsQ.isempty
		  break;
		case 1: // send telephone number to GPS module
		  gprsSerial.println("AT + CMGS = \"+4790020237\"");//send sms message, remember a country code
		  sendSmsTmeOut=tmeLoopEntry+300; // wait millis before starting nest state
		  sendSmsSts+=1; // set next state
		  //dp("Case 1, SmsSts = ");  dpl(sendSmsSts);
		  break;
		case 2:
		  insertSmsVariableText(sendSmsTxtIdx);
		  sendSmsTmeOut=tmeLoopEntry+300; // wait millis before starting nest state
		  sendSmsSts+=1; // set next state
		  //dp("Case 2, SmsSts = "); dpl(sendSmsSts);
		  break;
		case 3:
		  gprsSerial.println((char)26);//the ASCII code of the ctrl+z is 26
		  sendSmsTmeOut=tmeLoopEntry+300; // wait millis before starting nest state
		  sendSmsSts+=1; // set next state
		  //dp("Case 3, SmsSts = "); dpl(sendSmsSts);
		  break;
		case 4:
		  gprsSerial.println();
		  //dp(F("Case 4, SmsSts = "));   dpl(sendSmsSts);
		  sendSmsTmeOut=tmeLoopEntry+4000; // wait millis before starting nest state
		  digitalWrite(sndSmsLedPin,LOW);
		  sendSmsSts=0; // Make ready for sending next SMS
		  break;
	  }//switch
  }//if (tmeLoopEntry>sendSmsTmeOut
  
  mtDi4LedIndic.updateBlinkLED();
  bkDi4LedIndic.updateBlinkLED();
  //* New digital input LED for in here 

  //*************************************************
   
}// end of loop