#include <ESP32Servo.h>

Servo servo;

void setup() {
  servo.attach(22);
  Serial.begin(115200);
}

void loop() {
  Serial.println("0");
  servo.write(0);
  delay(500);
  delay(10000);
  Serial.println("90");
  servo.write(90);
  delay(500);
  delay(10000);
  Serial.println("180");
  servo.write(180);
  delay(500);
  delay(10000);
  Serial.println("90");
  servo.write(90);
  delay(500);
  delay(10000);
  Serial.println("0");
  servo.write(0);
  delay(500);
  delay(10000);
}
