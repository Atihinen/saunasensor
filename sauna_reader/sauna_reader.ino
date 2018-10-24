#include <ESP8266WiFi.h>

#include <WiFiClient.h>

#include <ESP8266WebServer.h>

#include <OneWire.h>
 
// OneWire DS18S20, DS18B20, DS1822 Temperature Example
 
OneWire  ds(D4);  // on pin D4 (a 4.7K resistor is necessary)


const char *ssid = "test";

const char *password = "password";
IPAddress ip(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

float read_temperature(void) 
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
 
  if ( !ds.search(addr)) 
  {
    ds.reset_search();
    delay(250);
    return -502;
  }
 
 
  if (OneWire::crc8(addr, 7) != addr[7]) 
  {
      Serial.println("CRC is not valid!");
      return -501;
  }
 
  // the first ROM byte indicates which chip
  switch (addr[0]) 
  {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return -99;
  } 
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end  
  delay(1000);
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
 
  for ( i = 0; i < 9; i++) 
  {           
    data[i] = ds.read();
  }
 
  // Convert the data to actual temperature
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) 
    {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else 
  {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
 
  }
  celsius = (float)raw / 16.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  return celsius;
}

void handleRoot() {
  float celsius = read_temperature();
  server.send(200, "application/json", "{\"temperature\": "+String(celsius, 1)+"}");

}

void setup() {

  delay(1000);
  
  Serial.begin(115200);
  
  Serial.println();
  
  Serial.print("Configuring access point...");
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  
  server.begin();
  
  Serial.println("HTTP server started");

}

void loop() {

  server.handleClient();

}
