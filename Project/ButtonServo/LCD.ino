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

void printLCDMessage(String message) {
    Serial.println("Printing hello");
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print(" " + message);
}

void printLCDParkingLotFullMessage() {
    Serial.println("Printing parking lot full");
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print(" Parking lot is");
    lcd.setCursor(0, 1);
    lcd.print(" full!");
    delay(4000);
    lcd.setBacklight(0);
    lcd.clear();
}

