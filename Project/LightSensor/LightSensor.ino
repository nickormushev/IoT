const int lightSensorPin = 4;

void setup() {
  Serial.begin(115200);
  pinMode(lightSensorPin, INPUT);
}

void loop() {
   int ldrStatus = analogRead(lightSensorPin);
   
   Serial.println(String("Light sensor: ") + String(ldrStatus));
}
