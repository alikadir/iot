#include <Arduino.h>
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"

#include <U8g2lib.h>
#include <Wire.h>

#define SDA_PIN 5
#define SCL_PIN 6
#define LED_PIN 8

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.42" OLED


BLECharacteristic *txCharacteristic;

class RXCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    String received = characteristic->getValue();
    if (received.length() > 0) {
      Serial.print("Node.js'den gelen: ");
      String gelen = received.c_str();
      Serial.println(gelen);

  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,gelen.c_str());  // write something to the internal memory
  u8g2.sendBuffer();    

      // Gelen yazıya yanıt gönderelim
      txCharacteristic->setValue("Mesaj alındı");
      txCharacteristic->notify();
    }
  }
};

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();

  Serial.begin(115200);
  BLEDevice::init("ESP32C3-BLE-SERIAL");

  BLEServer *server = BLEDevice::createServer();
  BLEService *service = server->createService(SERVICE_UUID);

  // TX (ESP32 → Client)
  txCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY
  );

  // RX (Client → ESP32)
  BLECharacteristic *rxCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE
  );

  rxCharacteristic->setCallbacks(new RXCallback());

  service->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->start();

  Serial.println("ESP32-C3 BLE UART başlatıldı");
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
   digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  delay(1000);
}