#include <Wire.h>

#define EEPROM_ADDR 0x50
#define EEPROM_SIZE 700 // 8KB

uint8_t eepromReadByte(uint16_t addr) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(highByte(addr));
  Wire.write(lowByte(addr));
  Wire.endTransmission(false);

  Wire.requestFrom((uint8_t)EEPROM_ADDR, (uint8_t)1);
  if (Wire.available())
    return Wire.read();

  return 0xFF;
}

void eepromWriteByte(uint16_t addr, uint8_t data) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(highByte(addr));
  Wire.write(lowByte(addr));
  Wire.write(data);
  Wire.endTransmission();
  delay(5); // EEPROM write cycle
}

void eepromWriteString(uint16_t addr, String data) {
  for (uint16_t i = 0; i < data.length() && addr < EEPROM_SIZE - 1; i++) {
    eepromWriteByte(addr++, data[i]);
  }
  eepromWriteByte(addr, 0x00); // NULL
  dumpEEPROM(EEPROM_SIZE);
}

String eepromReadString(uint16_t addr) {
  String result = "";
  uint8_t c;

  while (addr < EEPROM_SIZE) {
    c = eepromReadByte(addr++);
    if (c == 0x00 || c == 0xFF || c == 0x0A) break;

    if (c >= 32 && c <= 126)
      result += (char)c;
    else
      break;
  }
  return result;
}

void dumpEEPROM(uint16_t size) {
  Serial.println(F("\n===== EEPROM FULL DUMP ====="));

  for (uint16_t base = 0; base < size; base += 16) {
    Serial.print(F("0x"));
    if (base < 0x1000) Serial.print('0');
    Serial.print(base, HEX);
    Serial.print(F(": "));

    char ascii[17];

    for (uint8_t i = 0; i < 16; i++) {
      uint8_t val = eepromReadByte(base + i);

      if (val < 0x10) Serial.print('0');
      Serial.print(val, HEX);
      Serial.print(' ');

      ascii[i] = (val >= 32 && val <= 126) ? val : '.';
    }

    ascii[16] = '\0';
    Serial.print(F(" |"));
    Serial.print(ascii);
    Serial.println('|');
  }

  Serial.println(F("===== DUMP SONU =====\n"));
}

uint16_t parseHexAddress(String s) {
  s.trim();
  s.replace("0x", "");
  s.replace("0X", "");
  return (uint16_t) strtol(s.c_str(), NULL, 16);
}

void handleCommand(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  int eq = cmd.indexOf('=');

  // WRITE
  if (eq > 0) {
    uint16_t addr = parseHexAddress(cmd.substring(0, eq));
    String value = cmd.substring(eq + 1);

    Serial.print(F("\n[SET] 0x"));
    Serial.print(addr, HEX);
    Serial.print(F(" <= \""));
    Serial.print(value);
    Serial.println('"');

    eepromWriteString(addr, value);
  }
  // READ
  else {
    uint16_t addr = parseHexAddress(cmd);
    String val = eepromReadString(addr);

    Serial.print(F("\n[GET] 0x"));
    Serial.print(addr, HEX);
    Serial.print(F(" => \""));
    Serial.print(val);
    Serial.println('"');
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Arduino Uno → SDA=A4, SCL=A5 (pin seçilmez!)
  Wire.begin();
  Wire.setClock(100000);

  Serial.println(F("\nArduino Uno EEPROM Console"));
  Serial.println(F("I2C: SDA=A4  SCL=A5"));
  Serial.println(F("Komutlar:"));
  Serial.println(F("  0x0030=veli  -> write"));
  Serial.println(F("  0x0030       -> read\n"));

  dumpEEPROM(EEPROM_SIZE);
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    handleCommand(cmd);
  }
}
