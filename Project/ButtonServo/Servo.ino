void setEntranceInterrupt() {
    if(!entranceInteruptTriggered) {
        entranceInteruptTriggered = true;
        strip.SetPixelColor(entranceLightDiode, green);
        strip.Show();
    }
}

void setExitInterrupt() {
    if(!exitInteruptTriggered) {
        exitInteruptTriggered = true;
        strip.SetPixelColor(exitLightDiode, green);
        strip.Show();
    }
}

void openEntrance() {
    Serial.println("Opening entrance servo");
    strip.SetPixelColor(entranceLightDiode, red);
    strip.Show();

    if (freeParkingSpaces == 0) {
        printLCDParkingLotFullMessage();
        return;
    }

    freeParkingSpaces--;
    entranceServo.write(90);
    printLCDMessage("Welcome!");
    delay(3000);
    entranceServo.write(180);
    delay(2000);
    lcd.setBacklight(0);
    lcd.clear();
}

//A little bit of code duplication but I am too lazy to fix it
void openExit() {
    Serial.println("Opening exit servo");
    strip.SetPixelColor(exitLightDiode, red);
    strip.Show();

    if (freeParkingSpaces < parkingCapacity) {
        freeParkingSpaces++;
    }

    exitServo.write(90);
    printLCDMessage("Goodbye!");
    delay(3000);
    exitServo.write(180);
    delay(2000);
    lcd.setBacklight(0);
    lcd.clear();
}

void setupServo() {
    Serial.println("Setting up servo...");
    entranceServo.attach(entranceServoPin);
    exitServo.attach(exitServoPin);
    entranceServo.write(180);
    exitServo.write(180);
}
