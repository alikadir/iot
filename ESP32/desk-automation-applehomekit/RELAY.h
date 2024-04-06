#include "Utils.h"
#include <millisDelay.h>

struct RELAY : Service::Switch {  // deriving from the HomeSpan Switch service

  int relayPin;
  int timeOutMilliseconds;
  int buttonPin;

  int buttonLastState = LOW;
  int buttonCurrentState;

  SpanCharacteristic *power;
  millisDelay switchDelay;
  millisDelay buttonDelay;

  RELAY(int relayPin, int buttonPin = 0, int timeOutMilliseconds = 0)
    : Service::Switch() {

    power = new Characteristic::On();
    this->relayPin = relayPin;
    this->timeOutMilliseconds = timeOutMilliseconds;
    this->buttonPin = buttonPin;
    pinMode(relayPin, OUTPUT);
    buttonDelay.start(10);

    if (buttonPin != 0) {
      pinMode(buttonPin, INPUT_PULLUP);
    }
  }

  void loop() {

    if (switchDelay.justFinished()) {
      power->setVal(false);
      digitalWrite(relayPin, LOW);
      Serial.println("Single Delay finished.");
    }

    /////////////////////////////////////////////

    if (buttonPin != 0) {
      buttonCurrentState = digitalRead(buttonPin);
      delay(100);
      if (buttonLastState != buttonCurrentState) {
        Serial.println("The button clicked");
        if (buttonDelay.justFinished()) {
          int val = power->getNewVal();
          if (val == HIGH) {
            power->setVal(false);
            digitalWrite(relayPin, LOW);
            Serial.println("Button Clicked. Close.");
            buttonDelay.start(1000);
          } else {
            power->setVal(true);
            digitalWrite(relayPin, HIGH);
            Serial.println("Button Clicked. Open.");
            buttonDelay.start(1000);
          }
        }
      }
      buttonLastState = buttonCurrentState;
    }
  }

  boolean update() {

    int val = power->getNewVal();
    digitalWrite(relayPin, val);
    if (val == HIGH && timeOutMilliseconds != 0) {
      switchDelay.start(timeOutMilliseconds);
    }

    return (true);
  }

  void print() {
    //Serial.println("abc");
  }
};
