//Client Code 

#define TCP_MSS whatever
#define LWIP_IPV6 whatever
#define LWIP_FEATURES whatever
#define LWIP_OPEN_SRC whatever

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;


#define Sensor1  D0
#define Sensor2  D1
#define Sensor3  D2
#define Sensor4  D3
#define Door     D4

bool DoneReq = true ;

int httpGETRequest(const char* serverName) ;

const char* ssid = "Robot";
const char* password = "E12345678";

//Your IP address or domain name with URL path
const char* serverName = "http://192.168.4.1/Fun1"; // this part for room 1 only and we will change it in room 2 

void setup() 
{
  pinMode(Sensor1 , INPUT);
  pinMode(Sensor2 , INPUT);
  pinMode(Sensor3 , INPUT);
  pinMode(Sensor4 , INPUT);
  pinMode(Door , OUTPUT);
  digitalWrite(Door , LOW);

  /*//IF THIS NODE Worl as Access Point before we must include this part of code for first time only to make iit work as Station (optional)
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(1000);
  */
  
  Serial.begin(115200);
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
}

void loop() 
{
  if(!DoneReq)
  {
    delay(50000);
    DoneReq = true ;
  }
  if(digitalRead(Sensor1) == HIGH || digitalRead(Sensor2) == HIGH || digitalRead(Sensor3) == HIGH  || digitalRead(Sensor4) == HIGH && DoneReq)
  {
    if ((WiFiMulti.run() == WL_CONNECTED)) //to make sure the wifi still here (Connected)
    {
         int httpResponseCode = httpGETRequest(serverName);
         if(httpResponseCode == 200) // so the request is doneso we need to wait for robot and open the door 
         {
            DoneReq = false ;
            digitalWrite(Door , HIGH); // open the door 
         }
    }
    else
    {
          Serial.println("WiFi Disconnected");
          WiFi.begin(ssid, password);
          while (WiFi.status() != WL_CONNECTED) 
          {
              delay(500);
              Serial.print(".");
          }
    }
  }
}


int httpGETRequest(const char* serverName) 
{
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

  // Free resources
  http.end();

return httpResponseCode ;
}
