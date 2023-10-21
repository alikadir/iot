#include <SPI.h>                          
#include <MFRC522.h>                     
                     

int RST_PIN = 9;                          
int SS_PIN = 10;                          
                     
                          
MFRC522 rfid(SS_PIN, RST_PIN);       
      

void setup() { 
  Serial.begin(9600);                    
  SPI.begin();                           
  rfid.PCD_Init();  
  Serial.println("Reading started");
}
 
void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())    //Yeni kartın okunmasını bekliyoruz.
    return;

  if ( ! rfid.PICC_ReadCardSerial())      //Kart okunmadığı zaman bekliyoruz.
    return;

    Serial.println("The NUID tag is:");
    Serial.print("In hex: ");
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print("In dec: ");
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.println("=======================");
    Serial.println();
    rfid.PICC_HaltA();
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
