#include <millisDelay.h>

struct RELAY : Service::Switch {  // deriving from the HomeSpan Switch service

  int relayPin;
  int timeOutMilliseconds;
  SpanCharacteristic *power;
  millisDelay switchDelay;

  RELAY(int relayPin, int timeOutMilliseconds = 0) : Service::Switch() {

    power = new Characteristic::On();
    this->relayPin = relayPin;
    this->timeOutMilliseconds = timeOutMilliseconds;
    pinMode(relayPin, OUTPUT);
  }

  void loop() {
    if (switchDelay.justFinished()) {
      
      power->setVal(false);
      digitalWrite(relayPin, LOW);
      
      Serial.println("Single Delay finished.");
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

  void print(){
    //Serial.println("abc");
  }
};