
#include "HomeSpan.h" 
#include "LED.h"
#include "RELAY.h"

LED* led;

RELAY* up;
RELAY* down;
RELAY* bookshelf;
RELAY* desk;


void setup() {

  Serial.begin(115200);
  
  homeSpan.setWifiCredentials("AKB_2.4GHz","karakuzu1453");
  homeSpan.setPairingCode("11122333");
  homeSpan.setQRID("111-22-333");

  //digitalWrite(25, HIGH);
  //digitalWrite(26, HIGH);

  homeSpan.begin(Category::Bridges, "My Desk");
       
  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify();     

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Led");
    led = new LED(BUILTIN_LED); 

  
  new SpanAccessory();
    new Service::AccessoryInformation();    
      new Characteristic::Identify(); 
      new Characteristic::Name("Up");                       
    up = new RELAY(25,25000); 
  
   
  new SpanAccessory();
    new Service::AccessoryInformation();    
      new Characteristic::Identify();    
      new Characteristic::Name("Down");                    
    down = new RELAY(26,25000);  

  new SpanAccessory();
    new Service::AccessoryInformation();    
      new Characteristic::Identify();    
      new Characteristic::Name("Bookshelf Light");                    
    bookshelf = new RELAY(27);  

  new SpanAccessory();
    new Service::AccessoryInformation();    
      new Characteristic::Identify();    
      new Characteristic::Name("Desk Light");                    
    desk = new RELAY(14);    


}


void loop() {
  
  up->print();
  homeSpan.poll();

/*
  digitalWrite(26,HIGH);
  delay(3000);
  digitalWrite(26,LOW);
  delay(3000);
*/


}