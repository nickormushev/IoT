const int lightSensorPin = 4;

void setup() {
  Serial.begin(115200);
  pinMode(lightSensorPin, INPUT);
}

void loop() {
    int ldrStatus = analogRead(lightSensorPin);
    int first = analogRead(lightSensorPin);
    int last;

    delay(100);
    last = analogRead(lightSensorPin);

    if(first - last > 500) {
        Serial.println("Object present");
    } else if(last - first > 500) {
        Serial.println("Object removed");
    }
    
    Serial.println(String("Light sensor: ") + String(ldrStatus));
}
