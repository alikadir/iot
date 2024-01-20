#include <LiquidCrystal_I2C.h>

#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
int counter;
String currency = "--.----  --.----";
String progressBarChar = "-";
WiFiManager wm;


void printLcdFirstLine(String text, bool clear=true){
  if(clear) {
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print(text);
}
void printLcdSecondLine(String text, bool clear=true){
  if(clear) {
    lcd.clear();
  }
  lcd.setCursor(0,1);
  lcd.print(text);
}
void printLcdFullLine(String text){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text);
}

String getCurrency(){

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure); // for httpS call
    client->setInsecure();

    //WiFiClient client; // for http call
    HTTPClient http;    //Declare object of class HTTPClient
 
    // http.begin(client,"http://jsonplaceholder.typicode.com/posts");  without SSL
    http.begin(*client,"https://api.genelpara.com/embed/doviz.json");      //Specify request destination
    //http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
    int httpCode = http.GET();   //Send the request
    String payload = http.getString(); //Get the response payload
    http.end();  //Close connection

    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload

    JsonDocument doc;
    deserializeJson(doc, payload);
   
    String alis = String(doc["USD"]["alis"]);
    String satis = String(doc["USD"]["satis"]);
    
    return alis + "  " + satis;
}


void setup() {

  lcd.begin();
  lcd.backlight();
  printLcdFullLine(currency);
  //wm.resetSettings();

  Serial.begin(115200);

  bool res;
  String ssID = "-";
  String password = "-";

  printLcdFirstLine("Waiting wifi connection",false);
  printLcdSecondLine("w/ capative portal",false);

  //wm.setSTAStaticIPConfig(IPAddress(199,199,199,3), IPAddress(199,199,199,3), IPAddress(255,255,255,0));

  res = wm.autoConnect(); // password protected ap

  if(!res) {
      printLcdFirstLine("Failed to connect",true);
      delay(5000);
      printLcdFirstLine("Restarting...",true);
      ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      printLcdFirstLine("Connected...yeey :)");
      delay(3000);
      ssID = wm.getWiFiSSID();
      password = wm.getWiFiPass();
      printLcdFirstLine("ssID: "+ssID);
      printLcdSecondLine("pass: "+password,false);
      delay(5000);
  }
}



void loop() {


  
  counter = 0;
  currency = getCurrency();
  lcd.clear(); 

  for (;counter < 17; counter++){

    String progressBar = "";
    for (int i = 0; i < counter; i++){
      progressBar += progressBarChar;
    }
    printLcdFirstLine(currency,false);
    printLcdSecondLine(progressBar+">",false);
    delay(100);
  }




/*
  lcd.clear(); 
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("...Haci Rabis!...");
  lcd.setCursor(0, 1);
  lcd.print("----------------");
  
  delay(4000);
  lcd.noBacklight();
  delay(4000);
 
 
  lcd.clear(); 
  lcd.backlight();
  lcd.setCursor(0, 0);

  lcd.setCursor(0, 0);
  lcd.print("----------------");
  lcd.setCursor(0, 1);
  lcd.print("Nasilsin?");
  
  delay(4000);
  lcd.noBacklight();
  delay(4000);

  lcd.clear(); 
  lcd.backlight();
  lcd.setCursor(0, 0);

  lcd.setCursor(0, 0);
  lcd.print("----------------");
  lcd.setCursor(0, 1);
  lcd.print("Bende iyiyim :)");

  delay(4000);
  lcd.noBacklight();
  delay(4000);

   //if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
   
    
    lcd.clear(); 
    lcd.backlight();
    lcd.setCursor(0, 0);
   
  

    
 


  delay(5000);


  // put your main code here, to run repeatedly:
  pinMode(BUILTIN_LED, OUTPUT); 
  delay(1000);
  pinMode(BUILTIN_LED, LOW);
  delay(1000);
  */
}
