// Estacion Central
/*
void setup() {
Serial1.begin(9600);
Serial.begin(9600);
}

void loop() {
 // if(Serial.available()){Serial1.print((byte)Serial.read());}
  while(Serial1.available()){Serial.println((byte)Serial1.read());}
}
//*/

// Estación Host
void setup() {
Serial2.begin(9600);
}

void loop() {
  for(int i=0; i<179; i++){
    Serial2.print((char)i);  
    delay(1);
  }
  delay(10);
}
 
//*/
