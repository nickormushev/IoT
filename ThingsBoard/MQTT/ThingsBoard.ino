#include <WiFi.h>
#include <PubSubClient.h> //https://github.com/knolleary/pubsubclient/blob/master/src/PubSubClient.cpp

const char* mqtt_server = "tb.genevski.com";
const char* clientId = "<id>";
const char* user = "<user>";
const char* pass = "<password>";


// Replace with your network credentials
const char* ssid = "<ssid>";
const char* password = "<password>";

WiFiClient wifi;
PubSubClient client(wifi);

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
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  float temperature = random(200, 301) / 10.0;

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

