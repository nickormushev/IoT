void setEntranceInterrupt() {
    entranceInteruptTriggered = true;
}

void openEntrance() {
    Serial.println("Opening servo");
    if (freeParkingSpaces == 0) {
        return;
    }

    freeParkingSpaces--;
    servo.write(90);
    printLCDHelloMessage();
    delay(3000);
    servo.write(180);
    delay(2000);
    lcd.setBacklight(0);
    lcd.clear();
}

void setupServo() {
    Serial.println("Setting up servo...");
    servo.attach(22);
    servo.write(180);
}
