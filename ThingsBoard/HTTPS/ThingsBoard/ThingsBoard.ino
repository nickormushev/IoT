#include <WiFi.h>
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

const char* https_server = "tb.genevski.com";

// Replace with your network credentials
const char* ssid = "<ssid>";
const char* password = "<password>";

WiFiClientSecure client;

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
    Serial.println("HTTPS client is still connected");
    return 0;
  }
  
  Serial.print("Reconnecting to HTTPS server...");  
  if (client.connect(https_server, 443)) {
    Serial.println("connected");
    
    Serial.println("resubscribed");
    return 0;

  } else {
    Serial.println("failed");
    /*return client.state();*/
    return 1;
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  setup_wifi();
  
  client.setCACert(test_root_ca);
}

void loop() {
  
  float temperature = random(200, 301) / 10.0;
  
  int err = reconnect();
  if(err != 0){
    // TODO buffer the measurement to send next time
  } else {
    String json = "{\"temperature\":" + String(temperature,1) + "}"; 
    Serial.println(json);

    client.print(String("POST ") + "https://tb.genevski.com/api/v1/rT3FAKTKV5jjzuXN/telemetry HTTP/1.1\r\n"
            + "Host: " + https_server + "\r\n" 
            + "Connection: close\r\n"
            + "Content-Length: " + json.length() + "\r\n"
            + "Content-Type: application/json;charset=UTF-8\r\n\r\n" + json.c_str() + "\r\n");

    Serial.println("headers received: ");
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
            Serial.println(line);
            break;
        }
    }

    Serial.println("--------------");
    Serial.println("Response: ");

    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    Serial.println("--------------");
  }

  /*client.stop();*/
  delay(2000);
}

