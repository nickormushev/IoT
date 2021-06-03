const int pinButton = 15;

void setup() {
  pinMode(pinButton, INPUT);
  Serial.begin(115200);
}

void loop() {
  int stateButton = digitalRead(pinButton);
  if(stateButton == 1) {
     Serial.println("PRESSED"); 
  } else {
     Serial.println("RELEASED"); 
  }
  delay(20);
}
