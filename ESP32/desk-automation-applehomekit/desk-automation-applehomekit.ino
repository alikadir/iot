
#include "HomeSpan.h"
#include "LED.h"
#include "RELAY.h"
#include "HTTPClient.h"
#include <millisDelay.h>


LED* led;

RELAY* up;
RELAY* down;
RELAY* bookshelf;
RELAY* desk;

millisDelay mDelay;
int internetCheckDelay = 300000; //5 minute

//Output is inverted
//https://github.com/HomeSpan/HomeSpan/issues/248

void setup() {

  Serial.begin(115200);
  mDelay.start(internetCheckDelay);

  homeSpan.setWifiCredentials("AKB_2.4GHz", "******");
  homeSpan.setPairingCode("46637726");
  homeSpan.setQRID("466-37-726");
  //homeSpan.setPairingCode("11122333");
  //homeSpan.setQRID("111-22-333");

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
  up = new RELAY(25, 0, 25000);


  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Characteristic::Name("Down");
  down = new RELAY(26, 0, 25000);

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Characteristic::Name("Bookshelf Light");
  bookshelf = new RELAY(27, 18);

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

  if (mDelay.justFinished()) {
    WiFiClient client;
    if (!client.connect("api.ipify.org", 80)) {
      ESP.restart();
    } else
      Serial.println("internet ok!");
    mDelay.start(internetCheckDelay);
  }
}
