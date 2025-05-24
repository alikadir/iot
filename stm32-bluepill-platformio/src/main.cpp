#include <Arduino.h>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    Serial.println("Start Listening on Serial");
}

void loop() {
    while (Serial.available()) {
        String data = Serial.readString();
        Serial.print("Received: ");
        Serial.println(data);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    Serial.println("Pulse");
}
