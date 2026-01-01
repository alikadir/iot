#include <Wire.h>

#define EEPROM_ADDR 0x50
#define SDA_PIN 8
#define SCL_PIN 9
#define EEPROM_SIZE 700 // 8 KB EEPROM

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
  delay(5); // write cycle
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
  Serial.println("\n===== EEPROM FULL DUMP =====\n");

  for (uint16_t base = 0; base < size; base += 16) {

    Serial.printf("0x%04X: ", base);

    char ascii[17];

    for (uint8_t i = 0; i < 16; i++) {
      uint8_t val = eepromReadByte(base + i);
      Serial.printf("%02X ", val);
      ascii[i] = (val >= 32 && val <= 126) ? val : '.';
    }

    ascii[16] = '\0';
    Serial.print(" |");
    Serial.print(ascii);
    Serial.println("|");
  }

  Serial.println("\n===== DUMP SONU =====\n");
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

  // -------- WRITE --------
  if (eq > 0) {
    uint16_t addr = parseHexAddress(cmd.substring(0, eq));
    String value = cmd.substring(eq + 1);

    Serial.printf("\n[SET] 0x%04X <= \"%s\"\n", addr, value.c_str());

    eepromWriteString(addr, value);
  }
  // -------- READ --------
  else {
    uint16_t addr = parseHexAddress(cmd);
    String val = eepromReadString(addr);

    Serial.printf("\n[GET] 0x%04X => \"%s\"\n", addr, val.c_str());
  }
}

void setup() {
  Serial.begin(115200);
  delay(800);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  Serial.println("\nESP32-C3 EEPROM Console");
  Serial.println("EEPROM Size: 8 KB (0x0000 - 0x1FFF)");
  Serial.println("Boot dump basliyor...\n");

  dumpEEPROM(EEPROM_SIZE); 

  Serial.println("Komutlar:");
  Serial.println("  0x0030=veli  -> write");
  Serial.println("  0x0030       -> read");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    handleCommand(cmd);
  }
}
