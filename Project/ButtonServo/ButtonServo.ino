#include <ESP32Servo.h>

Servo servo;
const int entranceButtonPin = 15;

void setup() {
    pinMode(entranceButtonPin, INPUT);

    servo.attach(22);
    servo.write(90);

    Serial.begin(115200);
}

void loop() {
    int stateButton = digitalRead(entranceButtonPin);
    if(stateButton == 1) {
        servo.write(0);
        delay(3000);
        servo.write(90);
    }
}
