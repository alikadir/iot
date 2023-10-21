#include "DHT.h"


DHT MySensor(2, DHT11);

void setup() {
  
Serial.begin(9600); //start serial connection
Serial.println("serial connection is started");

MySensor.begin();

}

void loop() {

delay(1000);

float h = MySensor.readHumidity();

float t = MySensor.readTemperature();

float f = MySensor.readTemperature(true);

Serial.print("Humidity: ");
Serial.print(h);
Serial.println(" %");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" C ");
Serial.print(f);
Serial.println(" F");

Serial.println(" ");
}