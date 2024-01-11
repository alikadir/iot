#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

void setup() {
  // put your setup code here, to run once:

Serial.begin(115200);

 WiFiManager wm;

   
    bool res;
    String ssID = "-";
    String password = "-";
    
    wm.setTitle("My ESP Title");

    res = wm.autoConnect(); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
        delay(5000);
        Serial.println("Restarting");
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("Connected...yeey :)");

        ssID = wm.getWiFiSSID();
        password = wm.getWiFiPass();
        Serial.println(wm.getWiFiSSID());
        Serial.println(wm.getWiFiPass());
      //  wm.disconnect();
        

        Serial.println("Connecting main wifi");
      //  WiFi.begin(ssID, password);
       
      //  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
      //    delay(500);
      //    Serial.println("Waiting for connection");
      //  }
    }
  
}



void loop() {

   //if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure); // for httpS call
    client->setInsecure();

    //WiFiClient client; // for http call
    HTTPClient http;    //Declare object of class HTTPClient
 
    // http.begin(client,"http://jsonplaceholder.typicode.com/posts");  without SSL
    http.begin(*client,"https://jsonplaceholder.typicode.com/posts");      //Specify request destination
    //http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
    int httpCode = http.GET();   //Send the request
    String payload = http.getString(); //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload

    JsonDocument doc;
    deserializeJson(doc, payload);
    const char* title = doc[0]["title"];
    Serial.println(title);
 
    http.end();  //Close connection
 
 // } else {
 //   Serial.println("Error in WiFi connection");
 // }

  delay(5000);


  // put your main code here, to run repeatedly:
  pinMode(BUILTIN_LED, OUTPUT); 
  delay(1000);
  pinMode(BUILTIN_LED, LOW);
  delay(1000);
}
