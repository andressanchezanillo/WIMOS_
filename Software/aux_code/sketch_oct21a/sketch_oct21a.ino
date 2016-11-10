
void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);
  Serial.begin(9600);
}

void loop() {
    Serial.println("+++");
    delay(1000);
    while(Serial.available())
      SerialUSB.print(Serial.read());

}
