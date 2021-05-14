# 1 "/home/nikolay/sketch_mar09a/sketch_mar09a.ino"
/*//#define LED_BUILTIN 2

//void setup() {
    // put your setup code here, to run once:
//    pinMode(LED_BUILTIN, OUTPUT);
//    Serial.begin(115200L);
//}

void loop() {
    // put your main code here, to run repeatedly:
    //Serial.println("Hello world");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    //digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
//}
*/
#define LED_BUILTIN 2 /* for ESP32*/

void setup() {

  pinMode(2 /* for ESP32*/, 0x02);
  Serial.begin(115200L);
}

void loop() {

  Serial.println("Blink");
  digitalWrite(2 /* for ESP32*/, 0x0); // LOW = ON on the ESP-01
  delay(1000);
  digitalWrite(2 /* for ESP32*/, 0x1);
  delay(500);
}
