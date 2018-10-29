 #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //https://www.likecircuit.com/lcd-display-i2c-wemos-d1-mini/
#include "secrets.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);
int error_count = 0;
void setup()
{
  
  Serial.begin(115200);
  delay(10);
  Serial.println("Initializing lcd led");
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Intializing...");
  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  lcd.clear();
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  lcd.setCursor(0,1);
  lcd.print("C");
}


  void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.0.1:80/");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode == 200) { //Check the returning code
      error_count = 0;
      StaticJsonDocument<200> doc;
      String payload = http.getString();   //Get the request response payload
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.println("parseObject() failed");
        error_count++;
      }
      else {
        JsonObject root = doc.as<JsonObject>();
        float celsius = root["temperature"].as<float>();
        Serial.print("Temperature: ");
        Serial.println(celsius);
        lcd.setCursor(0,0);
        lcd.print("T:");
        lcd.setCursor(3,0);
        lcd.print(celsius);
      }
 
    }
    else {
      error_count++;
    }
    lcd.setCursor(0,1);
    lcd.print("C");
    lcd.setCursor(10,1);
    lcd.print("E");
    lcd.setCursor(11,1);
    lcd.print(error_count);
    http.end();   //Close connection
    delay(30000);    //Send a request every 30 seconds
  }
  else {
    lcd.setCursor(0,1);
    lcd.print("N");
    Serial.println("No connection..");
    delay(5000);
  }
  
 
}
