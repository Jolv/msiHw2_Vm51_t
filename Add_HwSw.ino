/*
========================================================================
About logging to file with a Python program      d140806

This program produces many output strings. i)Most of them for monitoring
of execution, but one ii)designed for logging to file.
All strings are read and parsed with a simple Python program
(Python is particularly good for parsing etc)
i) All strings for monitoring and debugging SHOULD start with a "-"
2) String for time-stamping and storing to file starts with a ">"


=====================================================
ADDING A NEW XBEE UNIT:
1) Configure the XBee using X-CTU
- Give it a name according to the naming standard <%1 (reference 1 to this text)
- Configure the unit as a Router, check software version for compatibility
- notice the the lower address in HEX
- configure digital inputs and notice which input bit used

2) Create XBee software modules in Multi Sensor Input (MSI) Software
- Fin pin and color for Receiving/Silent LED, %1> (see ref. 1)
  Define it with 'const int nnn'
- In Setup() enter a 'pinMode(nnn,OUTPUT)'
- Define SMS texts to be sent when the XBee receives or goes silent
  and enter this in SMS_def file
- Enter a .timedOut() statement near the end of loop()

3) Create the handling of the new signals in loop()

_______________________________________________________
ADDING A DIGITAL INPUT TO AN EXISTING XBEE
The procedure is very similar to adding a new XBee
except:
- after adding a new LED a CtrLED object is instantiated
- at the end of loop() a call to function
  .updateBlinkLED() is made 
  
========================================================================





========================================================================
NOT IN USE
In this file the naming convention in this project is described

sssSss|Uu|Ddd|Ty

sssSss :<Subsystem name >:  musTrap
Uuu :<unit, 
			Dx: Digital input>: D4
			Xb: XBee unit
Ddd	:<Device>: LedGn: Green LED
			
Ty:<Type>: 	Pin: output og input pin				
		>:	Ob , Object 
In use:
	musTrpD4: Subsystem mouse trap, digital input 4
	musTrpXbe: Subsystem mouse trap, XBee
	musTrpD4LedCc

	blmKaD4
	blmKaXbe

gnLedPin2=7;  // plaVaD4 GN LED
const int blmKaD4LedGnPin=7; //BlomKa, Digital input 4, Green LED, pin used
*/
