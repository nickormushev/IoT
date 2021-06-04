#include <ESP32Servo.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display
Servo servo;

const int entranceButtonPin = 15;
const int lcdData = 18;
const int lcdClock = 19;

const char* test_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
     "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
     "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
     "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
     "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
     "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
     "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
     "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
     "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
     "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
     "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
     "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
     "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
     "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
     "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
     "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
     "-----END CERTIFICATE-----\n";

/*const char* https_server = "tb.genevski.com";*/
const char* https_server = "thingsboard.cloud";
bool interuptTriggered = false;
int freeParkingSpaces = 4;

/*https://:443/api/v1/rT3FAKTKV5jjzuXN/telemetry */

// Replace with your network credentials
const char* ssid = "<ssid>";
const char* password = "<password>";

WiFiClientSecure client;

void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
int reconnect();
void loopThingsBoard();


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

void openEntrance() {
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

void setInterrupt() {
    interuptTriggered = true;
}

void setupButtonsAndSerial() {
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println("Setting button pins to input...");
    pinMode(entranceButtonPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(entranceButtonPin), setInterrupt, FALLING);
}

void setupThingsBoard() {
    randomSeed(analogRead(0));
    
    setup_wifi();
    
    client.setCACert(test_root_ca);
}

void setup() {
    setupButtonsAndSerial();
    setupServo();
    setupLCD();
    setupThingsBoard();
}

void loop() {
    /*int stateButton = digitalRead(entranceButtonPin);*/
    if(interuptTriggered) {
        openEntrance();
        interuptTriggered = false;
    }

    loopThingsBoard();
}
