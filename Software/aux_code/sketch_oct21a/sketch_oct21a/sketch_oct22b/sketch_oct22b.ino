void setup() {
  SerialUSB.begin(9600);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Hello WIMOS");
}
