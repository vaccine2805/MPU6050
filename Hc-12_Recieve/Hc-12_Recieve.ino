#include <SoftwareSerial.h>

SoftwareSerial hc12Serial(10, 11);  // HC-12 module connected to Arduino pins 10 (RX) and 11 (TX)

void setup() {
  Serial.begin(9600);
  hc12Serial.begin(9600);  // HC-12 module baud rate
}

void loop() {
  if (hc12Serial.available()) {
    String receivedData = hc12Serial.readStringUntil('\n');
    Serial.println(receivedData);
  }
}
