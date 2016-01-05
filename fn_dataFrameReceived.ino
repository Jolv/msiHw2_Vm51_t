
bool ioDataSampleRxIndFrame()
    {  // Global frameAry[]
	if(frameAry[0]==0x92)  // xBee sensor input frame type
// XBee frame type RxIoDataReceived has hex value=0x92
	{
		return true;
	}
	else{
		//dpl("not dataframe");
		return false;
	}
}
bool ZigBeeReceivePacket()
    {  // Global frameAry[]
	if(frameAry[0]==0x90)  // xBee data packet
// XBee frame type "ZigBee Receive Packet" has hex value=0x90
	{
		return true;
	}
	else{
		// dpl("not dataframe"); //irrelevant when several frame types om use
		return false;
	}
}