void insertSmsVariableText(int smsIdx)
{
	switch (smsIdx){	
	case Sms_DUMMY1:
		gprsSerial.println(Sms_DUMMY1_text);
		break;
	//case 2:
		//smsTxtToGPRS.println(F("A text from ARDU is SENDT no:2 "));//the content of the message
		//break;
	case Sms_fromMusTra_Di04:
		gprsSerial.println(Sms_fromMusTra_Di04_txt);
		break;
	case Sms_MusTraXBeeTransmitting:
		gprsSerial.println(Sms_MusTraXBeeTransmitting_txt);
		break;
	case Sms_MusTraXBeeSilent:
		gprsSerial.println(Sms_MusTraXBeeSilent_txt); 
		break;		
	case Sms_BlomKa_water_res_empty:
		gprsSerial.println(Sms_BlomKa_water_res_empty_txt); 
		break;		
	case Sms_BlomKa_xBee_is_transm:
		gprsSerial.println(Sms_BlomKa_xBee_is_transm_txt); 
		break;
	case Sms_BlomKa_xBee_is_SILENT:
		gprsSerial.println(Sms_BlomKa_xBee_is_SILENT_txt); 
		break;
	case Sms_dropCnt_Xbee_is_transm:
		gprsSerial.println(Sms_dropCnt_Xbee_is_transm_txt); 
		break;
	case Sms_dropCnt_Xbee_is_SILENT:
		gprsSerial.println(Sms_dropCnt_Xbee_is_SILENT_txt); 
		break;
	case Sms_DropMon_is_tooWet:
		gprsSerial.println(Sms_DropMon_is_tooWet_txt); 
		break;
	case Sms_DropMon_is_tooDry:
		gprsSerial.println(Sms_DropMon_is_tooDry_txt); 
		break;
	case Sms_tsBMP_Xbee_is_transm:
		gprsSerial.println(Sms_tsBMP_Xbee_is_transm_txt); 
		break;
	case Sms_tsBMP_Xbee_is_SILENT:
		gprsSerial.println(Sms_tsBMP_Xbee_is_SILENT_txt); 
		break;



		
	////case 90:
	    ////smsTxtToGPRS.println(F("A text from ARDU is SENDT no:90 "));//the content of the message
		////dpl("nyMUuS  SMS no 90 sent");
		////break;
	
	default:
		gprsSerial.println(F("Unknown SMSindex in Queue "));//the content of the message
	}
	
	
	
}