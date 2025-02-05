#include <SoftwareSerial.h>
SoftwareSerial loRaSerial(4, 0);

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
