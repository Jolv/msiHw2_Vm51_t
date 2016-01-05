/*SMS_def_file.h
 * Created: 22.03.2014 19:37:49
 *  Author: D630
 *
 * The ARDUINOS has limit RAM storage. Serial output text is
 * stored in RAM, also SMS text strings.
 * In this application many ong SMS text may be stored and this
 * may cause problems.
 * storing Serial output in program memory (or Flash) is done by 
 * Serial.println(F("SMS txt")); 
 * I like to collect all these texts and their indexes in one place
 * to get a good overview, and therefor define them in this file
 * of Macros.
 */ 

#ifndef SMS_DEF_FILE_H_
#define SMS_DEF_FILE_H_


#define Sms_DUMMY1 1
#define Sms_DUMMY1_text F("A text from ARDU is SENDT no:1 ")

//SmsIdx 4:	
#define Sms_fromMusTra_Di04 4
#define Sms_fromMusTra_Di04_txt F("Alarm fromMusTra Di04. Msg:4 ")

#define Sms_MusTraXBeeTransmitting 5
#define Sms_MusTraXBeeTransmitting_txt F("MusTra XBee is NOW  tansmitting. Msg:5 ")

#define Sms_MusTraXBeeSilent 6
#define Sms_MusTraXBeeSilent_txt F("MusTra XBee is SILENT. Msg:6 ")

#define Sms_BlomKa_water_res_empty 15
#define Sms_BlomKa_water_res_empty_txt F("BlomKa water reservoir is empty. Msg:15 ")

#define Sms_BlomKa_xBee_is_transm 16
#define Sms_BlomKa_xBee_is_transm_txt F("BlomKa xBee now transmitting. Msg:16 ")

#define Sms_BlomKa_xBee_is_SILENT 17
#define Sms_BlomKa_xBee_is_SILENT_txt F("BlomKa xBee is SILENT. Msg:17 ")

#define Sms_dropCnt_Xbee_is_transm 18
#define Sms_dropCnt_Xbee_is_transm_txt F("DropCnt_Xbee now transmitting. Msg:18 ")

#define Sms_dropCnt_Xbee_is_SILENT 19
#define Sms_dropCnt_Xbee_is_SILENT_txt F("DropCnt_Xbee is SILENT. Msg:19 ")

#define Sms_DropMon_is_tooWet 20
#define Sms_DropMon_is_tooWet_txt F("Sms_DropMon is tooWet. Msg:20")

#define Sms_DropMon_is_tooDry 21
#define Sms_DropMon_is_tooDry_txt F("Sms_DropMon is tooDry. Msg:20")

// allow some space for additional DropMonitoring sms-es

#define Sms_tsBMP_Xbee_is_transm 26
#define Sms_tsBMP_Xbee_is_transm_txt F("tsBMP_Xbee now transmitting. Msg:26 ")

#define Sms_tsBMP_Xbee_is_SILENT 27
#define Sms_tsBMP_Xbee_is_SILENT_txt F("tsBMP_Xbee is SILENT. Msg:27 ")


#endif /* SMS_DEF_FILE_H_ */
/*

SmsIdx 1:	 smsTxtToGPRS.println();//the content of the message
SmsIdx 2:	 smsTxtToGPRS.println(F("A text from ARDU is SENDT no:2 "));//the content of the message

SmsIdx 4:	 smsTxtToGPRS.println(F("Alarm fromMusTra Di04. Msg:4 "));
SmsIdx 5:	 smsTxtToGPRS.println(F("MusTra XBee is NOW  tansmitting. Msg:5 "));
SmsIdx 6:	 smsTxtToGPRS.println(F("MusTra XBee is SILENT. Msg:6 "));

SmsIdx 15:	 smsTxtToGPRS.println(F("BlomKa water reservoir is empty. Msg:15 "));
SmsIdx 16:	 smsTxtToGPRS.println(F("BlomKa xBee now transmitting. Msg:16 "));
SmsIdx 17: 	 smsTxtToGPRS.println(F("BlomKa xBee is SILENT. Msg:17 "));

SmsIdx 20:	 smsTxtToGPRS.println(F("A text from ARDU is SENDT no:20 "));

*/
