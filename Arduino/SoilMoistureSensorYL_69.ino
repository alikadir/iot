// Watch video here: https://www.youtube.com/watch?v=BKp4ib1UQj8

/* Connection pins:
Arduino     Soil Moisture Sensor YL-69
  A6             Analog A6
  5V               VCC
  GND              GND
*/

void setup()
{
  Serial.begin(9600);
  pinMode(A6, INPUT); //set up analog pin 0 to be input
}

void loop()
{
  int s = analogRead(A6); //take a sample
  Serial.print(s); Serial.print(" - ");
  
  if(s >= 1000) {
   Serial.println("Sensor is not in the Soil or DISCONNECTED");
  }
  if(s < 1000 && s >= 600) { 
   Serial.println("Soil is DRY");
  }
  if(s < 600 && s >= 370) {
   Serial.println("Soil is HUMID"); 
  }
  if(s < 370) {
   Serial.println("Sensor in WATER");
  }
  delay(50);
}
