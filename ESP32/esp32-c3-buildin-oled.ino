#include <Arduino.h>
#include <U8g2lib.h>


#include <Wire.h>

#define SDA_PIN 5
#define SCL_PIN 6

#define LED_PIN 8

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.42" OLED

void setup(void) {
  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();
pinMode(LED_PIN,OUTPUT);
}

void loop(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,"0123456789012345");  // write something to the internal memory
  u8g2.drawStr(0,20,"0123456789012345"); 
  u8g2.drawStr(0,30,"0123456789012345"); 
  u8g2.drawStr(0,40,"0123456789012345"); 
  u8g2.drawStr(0,50,"0123456789012345"); 
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);  
}