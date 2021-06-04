void setupLCD() {
    Serial.println("Checking for LCD...");

    Wire.begin(lcdData, lcdClock);
    Wire.beginTransmission(0x27);
    int error = Wire.endTransmission();

    if (error == 0) {
        Serial.println(": LCD found.");
        lcd.begin(16, 2); // initialize the lcd
    } else {
        Serial.print("Error: ");
        Serial.print(error);
        Serial.println(": LCD not found.");
    }
}

void printLCDHelloMessage() {
    Serial.println("Printing hello");
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print(" Welcome!");
}

