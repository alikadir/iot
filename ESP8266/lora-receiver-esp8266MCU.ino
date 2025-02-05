#include <SoftwareSerial.h>
SoftwareSerial loRaSerial(4, 0);

void setup() {
  Serial.begin(9600);
  loRaSerial.begin(9600);

  Serial.println("LoRa E32 listening");
}

void loop() {
  while (loRaSerial.available()) {
    int receivedData = loRaSerial.read();
    Serial.print("from LoRa");
    Serial.println(receivedData);
  }
}
