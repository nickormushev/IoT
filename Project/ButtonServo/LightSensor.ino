void setupNeopixel() {
    Serial.println("Starting neopixel strip...");
    strip.Begin();
    strip.Show();

    for (int i = 0; i < parkingCapacity; ++i) {
        strip.SetPixelColor(i, green);
    }

    strip.SetPixelColor(5, red);
    strip.Show();
}

void checkObjectPresent() {
    Serial.println("Checking to see if a car has parked infront of sensor");

    int newLight, oldLight;
    
    for (int i = 0; i < parkingCapacity; ++i) {
        Serial.println(String("Reading pin: ") + String(lightSensorPins[i]));
        newLight = analogRead(lightSensorPins[i]);
        oldLight = lastIterationLight[i];

        if (oldLight - newLight > lightDifference) {
            strip.SetPixelColor(i, red);
            /*parkingLotTaken[i] = true;*/
            strip.Show();
        } else if (newLight - oldLight > lightDifference) {
            /*parkingLotTaken[i] = false;*/
            strip.SetPixelColor(i, green);
            strip.Show();
        }

        lastIterationLight[i] = newLight;
    }
}
