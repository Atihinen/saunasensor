 #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//SSID of your network
char ssid[] = "test"; //SSID of your Wi-Fi router
char pass[] = "password"; //Password of your Wi-Fi router

void setup()
{
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
}


  void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.0.1:80/");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
      StaticJsonDocument<200> doc;
      String payload = http.getString();   //Get the request response payload
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.println("parseObject() failed");
      }
      else {
        JsonObject root = doc.as<JsonObject>();
        float celsius = root["temperature"].as<float>();
        Serial.print("Temperature: ");
        Serial.println(celsius);
      }
 
    }
 
    http.end();   //Close connection
    delay(30000);    //Send a request every 30 seconds
  }
  else {
    Serial.println("No connection..");
    delay(5000);
  }
  
 
}
