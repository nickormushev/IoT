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


void loopThingsBoard() {
  
  int err = reconnect();
  if(err != 0){
    // TODO buffer the measurement to send next time
  } else {
    String json = "{\"freeParkingSpaces\":" + String(freeParkingSpaces) + "}"; 
    Serial.println(json);

    client.print(String("POST ") + "https://thingsboard.cloud/api/v1/rT3FAKTKV5jjzuXN/telemetry HTTP/1.1\r\n"
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

