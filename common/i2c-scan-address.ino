 #include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("I2C scan basliyor...");

  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Cihaz bulundu: 0x");
      Serial.println(addr, HEX);
    }
  }
}

void loop() {}
