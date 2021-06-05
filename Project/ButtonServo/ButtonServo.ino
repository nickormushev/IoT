#include <ESP32Servo.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <NeoPixelBus.h>

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display
Servo entranceServo;
Servo exitServo;

const int entranceServoPin = 22;
const int exitServoPin = 12;

const uint16_t PixelCount = 6;
const uint8_t PixelPin = 13;

#define colorSaturation 60

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

const int entranceButtonPin = 15;
const int exitButtonPin = 2;
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
bool entranceInteruptTriggered = false;
bool exitInteruptTriggered = false;

const int lightDifference = 100; // the light difference is used to determine if an object is present
const int parkingCapacity = 2;
const int lightSensorPins[parkingCapacity] = {32, 36};

const int entranceLightDiode = 5;
const int exitLightDiode = 4;

bool parkingLotTaken[parkingCapacity];
int freeParkingSpaces = parkingCapacity;
int lastIterationLight[parkingCapacity] = {0, 0};

// Replace with your network credentials
const char* ssid = "TP-LINK_0B52A8";
const char* password = "12345678999";

WiFiClientSecure client;

void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
int reconnect();
void loopThingsBoard();
void setEntranceInterrupt();
void openEntrance();
void setupServo();
void setupLCD();
void setupThingsBoard();
void printLCDHelloMessage();
void checkObjectPresent();
void setupNeopixel();
void openExit();
void setExitInterrupt();

void setupPinsAndSerial() {
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println("Setting pins to input...");
    pinMode(entranceButtonPin, INPUT);

    for (int i = 0; i < parkingCapacity; ++i) {
        pinMode(lightSensorPins[i], INPUT);
    }

    attachInterrupt(digitalPinToInterrupt(entranceButtonPin), setEntranceInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(exitButtonPin), setExitInterrupt, FALLING);
}

void setup() {
    setupPinsAndSerial();
    setupServo();
    setupLCD();
    setupThingsBoard();
    setupNeopixel();


    for (int i = 0; i < parkingCapacity; ++i) {
        parkingLotTaken[i] = false;
    }
}

void loop() {
    if(entranceInteruptTriggered) {
        openEntrance();
        entranceInteruptTriggered = false;
    }

    if(exitInteruptTriggered) {
        openExit();
        exitInteruptTriggered = false;
    }

    checkObjectPresent();
    loopThingsBoard();
}

