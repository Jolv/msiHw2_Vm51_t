

void initPinsdown(int first, int last, int step)
{
	delay(100);
	for (int t=first;t>=last;t=(t+step))
	{
		pinMode(t,OUTPUT);
		delay(100);
		digitalWrite(t,HIGH);
		delay(200);
		digitalWrite(t,LOW);
//		Serial.print("  t=");
//  	Serial.print(t);
	}
//	Serial.println();
}
