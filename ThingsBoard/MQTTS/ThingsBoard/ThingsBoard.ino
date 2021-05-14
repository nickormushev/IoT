#include <WiFi.h>
#include <PubSubClient.h> //https://github.com/knolleary/pubsubclient/blob/master/src/PubSubClient.cpp
#include <WiFiClientSecure.h>

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

const char* mqtt_server = "tb.genevski.com";
const char* clientId = "<id>";
const char* user = "<user>";
const char* pass = "<password>";
const int mqtt_port = 8883;//24831;


// Replace with your network credentials
const char* ssid = "<ssid>";
const char* password = "<password>";

WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

void setup_wifi() {
    WiFi.begin(ssid, password); 
    Serial.println("Connecting to wifi ");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic [" + String(topic) + "]");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

int reconnect() {
  Serial.println("Reconnect requested");
  if(client.connected()) {
    Serial.println("MQTT client is still connected");
    return 0;
  }
  
  Serial.print("Reconnecting to MQTT server...");  
  if (client.connect(clientId, user, pass)) {
    Serial.println("connected");
    
    client.subscribe("topics/2");
    Serial.println("resubscribed");
    return 0;

  } else {
    Serial.println("failed");
    return client.state();
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  
  float temperature = random(200, 301) / 10.0;

  wifiClient.setCACert(test_root_ca);
  int err = reconnect();
  if(err != 0) {
    // TODO buffer the measurement to send next time
  } else {
    client.loop(); // process incoming messages and maintain connection to server
    
    // TODO add sequence number
    String json = "{\"temperature\":" + String(temperature,1) + "}"; 
    Serial.println(json);
    client.publish("v1/devices/me/telemetry", json.c_str());
  }
  delay(2000);
}

