void setup() {
  SerialUSB.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if(SerialUSB.available()){Serial.print((char)SerialUSB.read());}
  if(Serial.available()){SerialUSB.print((char)Serial.read());}
}
