void setEntranceInterrupt() {
    if(!entranceInteruptTriggered) {
        entranceInteruptTriggered = true;
        strip.SetPixelColor(5, green);
        strip.Show();
    }
}

void openEntrance() {
    Serial.println("Opening servo");
    strip.SetPixelColor(5, red);
    strip.Show();
    if (freeParkingSpaces == 0) {
        printLCDParkingLotFullMessage();
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
