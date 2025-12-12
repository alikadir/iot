#include <Arduino.h>
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"

#include <U8g2lib.h>
#include <Wire.h>

#define SDA_PIN 5
#define SCL_PIN 6
#define LED_PIN 8

#define SERVICE_UUID            "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX  "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

BLECharacteristic *txCharacteristic;

bool deviceConnected = false;

// ----------------------------------------------------
// BAĞLANTI DURUMU TAKİBİ
// ----------------------------------------------------
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Client bağlandı");
  }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Client ayrıldı");
  }
};

// ----------------------------------------------------
// RX CALLBACK (GELEN MESAJ)
// ----------------------------------------------------
class RXCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    String received = characteristic->getValue();

    if (received.length() > 0) {
      Serial.print("Node.js'den gelen: ");
      Serial.println(received);

      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(0, 10, received.c_str());
      u8g2.sendBuffer();

      txCharacteristic->setValue("Mesaj alındı");
      txCharacteristic->notify();
    }
  }
};

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();

  Serial.begin(1155);

  BLEDevice::init("ESP32C3-BLE-SERIAL");

  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService *service = server->createService(SERVICE_UUID);

  txCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY
  );

  BLECharacteristic *rxCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE
  );
  rxCharacteristic->setCallbacks(new RXCallback());

  service->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);

  BLEDevice::setPower(ESP_PWR_LVL_P9, ESP_BLE_PWR_TYPE_ADV);
  BLEDevice::setPower(ESP_PWR_LVL_P9, ESP_BLE_PWR_TYPE_DEFAULT);

  advertising->start();

  Serial.println("ESP32-C3 BLE UART başlatıldı");
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  delay(1000);

  // ----------------------------------------------------
  // HER 5 SANİYEDE BİR TARİH+SAAT+MİLİSANİYE GÖNDER
  // ----------------------------------------------------
  static unsigned long lastSend = 0;

  if (deviceConnected) {
    if (millis() - lastSend >= 5000) {   // 5000 ms = 5 sn
      lastSend = millis();

      unsigned long ms = millis();

      // ESP32'nin kendi sistem zamanı
      time_t now = time(nullptr);
      struct tm *t = localtime(&now);

      char buffer[80];
      snprintf(buffer, sizeof(buffer),
        "%04d-%02d-%02d %02d:%02d:%02d.%03lu",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        ms % 1000
      );

      txCharacteristic->setValue(buffer);
      txCharacteristic->notify();

      Serial.print("Gönderildi -> ");
      Serial.println(buffer);
    }
  }
}
