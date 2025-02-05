#include <SoftwareSerial.h>
SoftwareSerial loRaSerial(4, 0); // LoraModule-TxD Pin => 4 Pin(ESP8268-RxD) | LoraModule-RxD Pin => 0 Pin(ESP8268-TxD)

// M0 and M1 pins connect to GND for Normal Mode
// Also Wakeup, Sleep and PowerSaving Modes adjust with M0 and M1 

void setup() {
  Serial.begin(9600);
  loRaSerial.begin(9600);

  Serial.println("LoRa E32 sending.");
}

void loop() {
  loRaSerial.write((byte)0x00);
  loRaSerial.write(0X3f);
  loRaSerial.write(0X17);

  for (int i = 0; i < 100; i++)
    loRaSerial.write(i);

  Serial.println("send.");
  delay(1000);
}
