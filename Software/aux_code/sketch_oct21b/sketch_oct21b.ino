
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if(Serial1.available())
    Serial.print(Serial1.read());
}
