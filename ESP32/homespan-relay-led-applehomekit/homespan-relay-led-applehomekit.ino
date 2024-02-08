
#include "HomeSpan.h" 
#include "LED.h"
#include "RELAY.h"

#define relayPin 17

void setup() {


  // put your setup code here, to run once:
  Serial.begin(115200);

  homeSpan.setWifiCredentials("AKB_2.4GHz","******");
  homeSpan.setPairingCode("11122333");
  homeSpan.setQRID("111-22-333");

  homeSpan.begin(Category::Bridges, "HomeSpan Bridge");
       

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
    new LED(BUILTIN_LED); 

   
  // Accessory 2: Relay Switch
 
  new SpanAccessory();
    new Service::AccessoryInformation();    
      new Characteristic::Identify();                       
    new RELAY(relayPin);  // instantiates a new relay
   
}


void loop() {

/*
  digitalWrite(relayPin,HIGH);
  delay(3000);
  digitalWrite(relayPin,LOW);
  delay(3000);
*/

  // put your main code here, to run repeatedly:
  homeSpan.poll();
}