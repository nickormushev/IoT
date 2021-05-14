#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "<ssid>";
const char* password = "<password>";

void setup() {
    Serial.begin(115200);
    delay(4000);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("connecting to wifi...");
    }

    Serial.println("Connected successfully");
}

void loop() {
    if(WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://jsonplaceholder.typicode.com/comments?id=10");
        int httpCode = http.GET();
        
        if (httpCode > 0) { //Check for the returning code
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
        } else {
            Serial.println("Error on HTTP request");
        }

        http.end();
    }


    delay(10000);
}
