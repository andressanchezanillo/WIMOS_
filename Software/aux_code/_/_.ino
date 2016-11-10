void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if(Serial1.available()){Serial.print((char)Serial1.read());}
  if(Serial.available()){Serial1.print((char)Serial.read());}
}
