
char c = 'A';
int  pingPong = 1;

void setup()  {
   Serial.begin(57600);
   Serial.println( "Arduino started sending bytes via XBee" );

   // set the data rate for the SoftwareSerial port
   Serial1.begin( 19200 );
}

void loop()  {
  // send character via XBee to other XBee connected to Mac
  // via USB cable
  Serial1.print( c );
  
  //--- display the character just sent on console ---
  Serial.println( c );
  
  //--- get the next letter in the alphabet, and reset to ---
  //--- 'A' once we have reached 'Z'. 
  c = c + 1;
  if ( c>'Z' ) 
       c = 'A';
  
  //--- switch LED on Arduino board every character sent---
  if ( pingPong == 0 )
    digitalWrite(13, LOW);
  else
    digitalWrite(13, HIGH);
  pingPong = 1 - pingPong;
  delay( 1000 );
}
