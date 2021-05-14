#define LED_BUILTIN 1  // for ESP32

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200L);
}

void loop() {
  
  Serial.println("Blink");
  digitalWrite(LED_BUILTIN, HIGH);   // LOW = ON on the ESP-01
  delay(1000);                      
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(1000);
}
