#include <SoftwareSerial.h>
SoftwareSerial loRaSerial(4, 0); // LoraModule-TxD Pin => 4 Pin(ESP8268-RxD) | LoraModule-RxD Pin => 0 Pin(ESP8268-TxD)

// M0 and M1 pins connect to GND for Normal Mode
// Also Wakeup, Sleep and PowerSaving Modes adjust with M0 and M1 

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
